#ifndef _REST_SERVER_HPP_
#define _REST_SERVER_HPP_

#include <Arduino.h>
#include <Server.h>
#include <Client.h>

#include "RestUtils.h"
#include "RestRequest.hpp"
#include "RestResponse.hpp"

template <typename Server_t, typename Client_t>
class RestServer {
public:
	typedef void (*rest_callback_t)(RestRequest&, RestResponse&);
public:
	RestServer(Server_t& server) :
		_routes_size(0),
		_server(server)
	{
		// Set all buffers to 0
		memset(_field, 0, (MAX_HEADER_LEN + 1) * sizeof(_field[0]));
		memset(_value, 0, (MAX_HEADER_LEN + 1) * sizeof(_value[0]));
		memset(_routes, 0, MAX_ROUTES * sizeof(struct s_rest_route));
	}
  
	void Run() {
		_client = _server.available();
		if (_client) 
		{
			parseRequest();
			router();
			send();
			_client.stop();
			_request.reset();
			_response.reset();
		}
	}
	
	void All(const char * path, rest_callback_t callback)
	{
		addRoute(F_str(HTTP_METHOD_ALL), path, callback);
	}
	
	void Get(const char * path, rest_callback_t callback)
	{
		addRoute(F_str(HTTP_METHOD_GET), path, callback);
	}
	
	void Post(const char * path, rest_callback_t callback)
	{
		addRoute(F_str(HTTP_METHOD_POST), path, callback);
	}
	
	void Put(const char * path, rest_callback_t callback)
	{
		addRoute(F_str(HTTP_METHOD_PUT), path, callback);
	}
	
	void Delete(const char * path, rest_callback_t callback)
	{
		addRoute(F_str(HTTP_METHOD_DELETE), path, callback);
	}
	
	void Head(const char * path, rest_callback_t callback)
	{
		addRoute(F_str(HTTP_METHOD_HEAD), path, callback);
	}
	
	void Options(const char * path, rest_callback_t callback)
	{
		addRoute(F_str(HTTP_METHOD_OPTIONS), path, callback);
	}
	
	void Patch(const char * path, rest_callback_t callback)
	{
		addRoute(F_str(HTTP_METHOD_PATCH), path, callback);
	}

private:
	void 			addRoute(const char* method, const char* route, rest_callback_t callback)
	{
		// Circular buffer if too many routes
		if (_routes_size >= MAX_ROUTES)
			_routes_size = 0;

		strncpy(_routes[_routes_size].method, method, MAX_METHOD_LEN);
		strncpy(_routes[_routes_size].name, route, MAX_ROUTE_LEN);
		_routes[_routes_size].callback = callback;  
		_routes_size++;
	}

	void			readUntil(char delim)
	{
		char		c;

		while (_client.available()) 
		{
			c = _client.read();
			if (c == -1) {
				continue;
			}
			if (c == delim)
				break;
		}
	}

	void			readMethod()
	{
		char 		c;
		short		method_idx = 0;

		while (_client.available()) 
		{
			c = _client.read();
			if (c == -1) {
				continue;
			}
			if (c == ' ')// Done parsing METHOD
				break;
			_request.method[method_idx] = c;
			method_idx++;
		}
		_request.method[method_idx] = 0; // Null terminate METHOD
	}
	
	void			readRoute()
	{
		char		c;
		short		route_idx = 0;

		while (_client.available()) 
		{
			c = _client.read();
			if (c == -1) {
				continue;
			}
			if (c == ' ') // End of route
				break;
			if (route_idx < MAX_ROUTE_LEN) // Route is not long
			{
				_request.originalUrl[route_idx] = c;
				route_idx++;
			}
		}
		_request.originalUrl[route_idx] = 0; // Null terminate the route
	}
	
	bool			readHeader()
	{
		char		c;
		uint16_t	field_idx = 0;
		uint16_t	value_idx = 0;
		
		// Reset buffers
		memset(_field, 0, (MAX_HEADER_LEN + 1) * sizeof(_field[0]));
		memset(_value, 0, (MAX_HEADER_LEN + 1) * sizeof(_value[0]));
		
		// Read field
		while (_client.available()) 
		{
			c = _client.read();
			if (c == -1 || c == '\r')
				continue;
			if (c == '\n' || c == ':') // Empty line or done for field
				break;
			if (field_idx < MAX_HEADER_LEN) 
			{
				_field[field_idx] = c;
				field_idx++;
			}
		}
		_field[field_idx] = 0; // Null terminate field
		if (field_idx == 0) // Was just a new line
			return false;

		// Read space separator
		readUntil(' ');
		
		// Read value
		while (_client.available()) 
		{
			c = _client.read();
			if (c == -1 || c == '\r')
				continue;
			if (c == '\n') // End of line
				break;
			if (value_idx < MAX_HEADER_LEN) 
			{
				_value[value_idx] = c;
				value_idx++;
			}
		}
		_value[value_idx] = 0; // Null terminate value	
		return true;
	}
	
	void			readBody()
	{
		char		c;
		short		body_idx = 0;

		while (_client.available()) 
		{
			c = _client.read();
			if (c == -1) {
				continue;
			}
			if (body_idx < MAX_REQUEST_BODY_LEN) 
			{
				_request.body[body_idx] = c;
				body_idx++;
			}
		}
		_request.body[body_idx] = 0;
	}

	void			parseRoute()
	{
		bool		base = true;
		uint16_t	base_idx = 0;
		uint16_t	query_idx = 0;
		
		for (uint16_t i = 0; i < MAX_QUERY_LEN + MAX_URL_LEN; i++)
		{
			if (_request.originalUrl[i] == '?') 
			{
				base = false;
				continue;
			}
			if (base && base_idx < MAX_URL_LEN)
			{
				_request.baseUrl[base_idx] = _request.originalUrl[i];
				base_idx++;
			}
			else if (query_idx < MAX_QUERY_LEN)
			{
				_request.query[query_idx] = _request.originalUrl[i];
				query_idx++;
			}
		}
		_request.baseUrl[base_idx] = 0;
		_request.query[query_idx] = 0; 
	}
  
	void			parseRequest()
	{
		readMethod();
		readRoute();	
		readUntil('\n');
		parseRoute();
		while (readHeader()) {
			// Look for special headers	
			if (strncmp(F_str(HEADER_XHR_FIELD), _field, MAX_HEADER_LEN) == 0 &&
				strncmp(F_str(HEADER_XHR_VALUE), _value, MAX_HEADER_LEN) == 0)
				_request.xhr = true;
			else if (strncmp(F_str(HEADER_HOST_FIELD), _field, MAX_HEADER_LEN) == 0)
				strncpy(_request.hostname, _value, MAX_HEADER_LEN);
			else if (strncmp(F_str(HEADER_ACCEPT_FIELD), _field, MAX_HEADER_LEN) == 0)
				strncpy(_request._content_type, _value, MAX_CONTENT_LEN);
			else
				_request.set(_field, _value);
		}
		readBody();
	}

	void 			router()
	{
		for(int i = 0; i < _routes_size; i++)
		{
			// Check if the routes names matches
			if(strncmp(_request.baseUrl, _routes[i].name, MAX_ROUTE_LEN) != 0)
				continue;

			// Check if the HTTP METHOD matters for this route
			if(strncmp(_routes[i].method, F_str(HTTP_METHOD_ALL), MAX_METHOD_LEN) != 0) 
			{
				// If it matters, check if the methods matches
				if(strncmp(_request.method, _routes[i].method, MAX_METHOD_LEN) != 0)
				  continue;
			}
			// Route callback (function)
			_routes[i].callback(_request, _response);
		}
	}

	void 			send()
	{
		int content_length = strlen(_response._body);

		_client.print(HTTP_1_1);
		_client.print(" ");
		_client.print(_response._status);
		_client.print(CRLF);
		
		_client.print(HEADER_SERVER_FIELD);
		_client.print(HEADER_SEPARATOR);
		_client.print(HEADER_SERVER_VALUE);
		_client.print(CRLF);
		
		_client.print(HEADER_CONNECTION_FIELD);
		_client.print(HEADER_SEPARATOR);
		_client.print(HEADER_CONNECTION_VALUE);
		_client.print(CRLF);

		for (short i = 0; i < _response._headers_size; i++)
		{
			_client.print(_response._headers_fields[i]);
			_client.print(HEADER_SEPARATOR);
			_client.print(_response._headers_values[i]);
			_client.print(CRLF);			
		}
		
		if (content_length > 0)
		{
			_client.print(HEADER_CONTENT_TYPE_FIELD);
			_client.print(HEADER_SEPARATOR);
			_client.print(_response._content_type);
			_client.print(CRLF);
			
			_client.print(HEADER_CONTENT_LENGTH_FIELD);
			_client.print(HEADER_SEPARATOR);
			_client.print(content_length);
			_client.print(CRLF);
			
			_client.print("\n");
			_client.print(_response._body);
		}
		
	}
private:
	struct s_rest_route {
		char			method[MAX_METHOD_LEN + 1];
		char			name[MAX_ROUTE_LEN + 1];
		rest_callback_t		callback;
	};
	
	struct s_rest_route		_routes[MAX_ROUTES];
	uint8_t				_routes_size;
  
	Server_t&			_server;
	Client_t			_client;
	RestRequest			_request;
	RestResponse			_response;

	char				_field[MAX_HEADER_LEN + 1];
	char				_value[MAX_HEADER_LEN + 1];
};

#endif
