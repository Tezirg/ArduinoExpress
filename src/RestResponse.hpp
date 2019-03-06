#ifndef _REST_RESPONSE_HPP_
#define _REST_RESPONSE_HPP_

#include <Arduino.h>
#include "RestUtils.h"

class RestResponse {
public:
	RestResponse():
		_status(400),
		_headers_size(0)
	{
	}
	
	char*	get(const char* field)
	{
		if (_headers_size == 0)
			return 0;
		for (int i = 0; i < _headers_size; i++)
		{
			if (strncmp(_headers_fields[i], field, MAX_HEADER_LEN) == 0)
				return _headers_values[i];
		}
		return 0;
	}
	
	void	location(const char *path)
	{
		set(F_str(HEADER_LOCATION_FIELD), path);
	}
	
	void	redirect(const char *path, int status = 302)
	{
		_status = status;
		location(path);
	}
	
	void	send(const char *body)
	{
		strncpy(_body, body, MAX_BODY_LEN);
	}
	
	void	sendStatus(int statusCode)
	{
		const char*     default_body = get_default_body_from_status(statusCode); 

		_status = statusCode;
		strncpy(_body, default_body, MAX_BODY_LEN);
	}
	
	void	set(const char *field, const char* value = 0)
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
	
	void	status(int statusCode)
	{
		_status = statusCode;
	}
	
	void	type(char* content_type)
	{
		strncpy(_content_type, content_type, MAX_CONTENT_LEN);
	}
	
	uint16_t	_status;
	uint8_t		_headers_size;
	char		_headers_fields[MAX_HEADERS][MAX_HEADER_LEN + 1];
	char		_headers_values[MAX_HEADERS][MAX_HEADER_LEN + 1];
	char		_content_type[MAX_CONTENT_LEN + 1];
	char		_body[MAX_BODY_LEN + 1];
};

#endif
