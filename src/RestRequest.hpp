#ifndef _REST_REQUEST_HPP_
#define _REST_REQUEST_HPP_

#include <Arduino.h>
#include "RestUtils.h"

class RestRequest {
public:
	RestRequest()
	{
		reset();
	}
	
	void	    reset()
	{
		xhr = false;
    memset(originalUrl, 0, (MAX_QUERY_LEN + MAX_URL_LEN + 1) * sizeof(originalUrl[0]));
		memset(baseUrl, 0, (MAX_URL_LEN + 1) * sizeof(baseUrl[0]));
		memset(body, 0, (MAX_BODY_LEN + 1) * sizeof(body[0]));
		memset(hostname, 0, (MAX_HEADER_LEN + 1) * sizeof(_headers_fields[0][0]));
		memset(method, 0, (MAX_METHOD_LEN + 1) * sizeof(method[0]));
		memset(params, 0, (MAX_PARAMS_LEN + 1) * sizeof(params[0]));
		memset(query, 0, (MAX_QUERY_LEN + 1) * sizeof(query[0]));
		memset(_content_type, 0, (MAX_CONTENT_LEN + 1) * sizeof(_content_type[0]));
		
		_headers_size = 0;
		for (uint16_t i = 0; i < MAX_HEADERS; i++)
		{
			memset(_headers_fields[i], 0, (MAX_HEADER_LEN + 1) * sizeof(_headers_fields[0][0]));
			memset(_headers_values[i], 0, (MAX_HEADER_LEN + 1) * sizeof(_headers_values[0][0]));
		}
	}
	
	bool	    is(const char *content_type)
	{
		if (strncmp(_content_type, content_type, MAX_CONTENT_LEN) == 0)
			return true;
		return false;
	}
	
	char*	    get(char *field)
	{
		for (uint8_t i = 0; i < MAX_HEADERS; i++)
		{
			if (strncmp(_headers_fields[i], field, MAX_HEADER_LEN) == 0)
				return _headers_values[i];
		}
		return 0; // Not found
	}
	
	void	    set(const char *field, const char* value = 0)
	{
		// Circular buffering if to many headers
		if (_headers_size >= MAX_HEADERS)
			_headers_size = 0;
		// Save values
		strncpy(_headers_fields[_headers_size], field, MAX_HEADER_LEN);
		if (value)
			strncpy(_headers_values[_headers_size], value, MAX_HEADER_LEN);
		else // Empty string if no value
			_headers_values[_headers_size][0] = 0; 
		// Increment counter
		_headers_size++;
	}

  char      originalUrl[MAX_QUERY_LEN + MAX_URL_LEN + 1];
	char		  baseUrl[MAX_URL_LEN + 1];
	char		  body[MAX_BODY_LEN + 1];
	char		  hostname[MAX_HEADER_LEN + 1];
	char		  method[MAX_METHOD_LEN + 1];
	char		  params[MAX_PARAMS_LEN + 1];
	char		  query[MAX_QUERY_LEN + 1];
	bool		  xhr;

	char		  _content_type[MAX_CONTENT_LEN + 1];
	uint8_t		_headers_size;
	char		  _headers_fields[MAX_HEADERS][MAX_HEADER_LEN + 1];
	char		  _headers_values[MAX_HEADERS][MAX_HEADER_LEN + 1];
};

#endif
