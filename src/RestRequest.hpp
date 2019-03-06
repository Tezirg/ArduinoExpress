#ifndef _REST_REQUEST_HPP_
#define _REST_REQUEST_HPP_

#include <Arduino.h>
#include "RestUtils.h"

class RestRequest {
public:
	RestRequest() :
		xhr(false),
		_headers_size(0)
	{}
	
	bool	is(const char *content_type)
	{
		if (strncmp(_content_type, content_type, MAX_CONTENT_LEN) == 0)
			return true;
		return false;
	}
	
	char*	get(char *field)
	{
		if (_headers_size == 0) // No headers received
			return 0;
		for (uint8_t i = 0; i < _headers_size; i++)
		{
			if (strncmp(_headers_fields[i], field, MAX_HEADER_LEN) == 0)
				return _headers_values[i];
		}
		return 0; // Not found
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

	char		baseUrl[MAX_URL_LEN + 1];
	char		body[MAX_BODY_LEN + 1];
	char		hostname[MAX_HEADER_LEN + 1];
	char		method[MAX_METHOD_LEN + 1];
	char		params[MAX_PARAMS_LEN + 1];
	char		query[MAX_QUERY_LEN + 1];
	bool		xhr;

	char		_content_type[MAX_CONTENT_LEN + 1];
	uint8_t		_headers_size;
	char		_headers_fields[MAX_HEADERS][MAX_HEADER_LEN + 1];
	char		_headers_values[MAX_HEADERS][MAX_HEADER_LEN + 1];
};

#endif