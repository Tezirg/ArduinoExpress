#ifndef _REST_UTILS_H_
#define _REST_UTILS_H_

#define HTTP_1_1                    F("HTTP/1.1")
#define CRLF						            F("\r\n")
#define HEADER_SEPARATOR			      F(": ")
#define HEADER_SERVER_FIELD         F("Server")
#define HEADER_CONTENT_TYPE_FIELD	  F("Content-Type")
#define HEADER_CONTENT_LENGTH_FIELD	F("Content-Length")
#define HEADER_CONTENT_TYPE_FIELD	  F("Content-Type")
#define HEADER_CONTENT_TYPE_VALUE	  F("text/plain")
#define HEADER_CONNECTION_FIELD		  F("Connection")
#define HEADER_CONNECTION_VALUE		  F("close")
#define HEADER_ACCEPT_FIELD			    F("Accept")
#define HEADER_HOST_FIELD			      F("Host")
#define HEADER_XHR_FIELD			      F("X-Requested-With")
#define HEADER_XHR_VALUE			      F("XMLHttpRequest")
#define HEADER_LOCATION_FIELD		    F("Location")

#define MAX_METHOD_LEN				      7
#define MAX_ROUTES 					        6
#define MAX_ROUTE_LEN				        16
#define MAX_HEADERS					        8
#define MAX_HEADER_LEN				      16
#define MAX_CONTENT_LEN				      28
#define MAX_PARAMS_LEN				      32
#define MAX_QUERY_LEN				        32
#define MAX_URL_LEN					        32
#define MAX_REQUEST_BODY_LEN				32
#define MAX_RESPONSE_BODY_LEN       32
#define MAX_STATUS_BODY_LEN			    25

#define HTTP_METHOD_ALL 			      F("*")
#define HTTP_METHOD_GET 			      F("GET")
#define HTTP_METHOD_PUT 			      F("PUT")
#define HTTP_METHOD_HEAD 			      F("HEAD")
#define HTTP_METHOD_POST 			      F("POST")
#define HTTP_METHOD_PATCH 			    F("PATCH")
#define HTTP_METHOD_DELETE 			    F("DELETE")
#define HTTP_METHOD_OPTIONS 		    F("OPTIONS")

const PROGMEM uint16_t rest_status_codes[] = {
	// 1xx Informational response
	100, 101, 102, 103,
	// 2xx Success
	200, 201, 202, 204, 205,
	// 3xx Redirection
	301, 302, 307, 308,
	//4xx Client errors
	400, 401, 402, 403,
	404, 405, 406, 407,
	// 5xx HEADER_SERVER_VALUE errors
	500, 501, 502, 503
};

const PROGMEM char rest_status_bodies[][MAX_STATUS_BODY_LEN]  = {
	//1xx Informational response
	"Continue", "Switching Protocols", 
	"Processing", "Early Hints",
	// 2xx Success
	"OK", "Created", 
	"Accepted", "No Content",
	"Reset Content",
	//3xx Redirection
	"Moved Permanently", "Found", 
	"Temporary Redirect", "Permanent Redirect", 
	//4xx Client errors
	"Bad Request", "Unauthorized",
	"Payment Required", "Forbidden",
	"Not Found", "Method Not Allowed",
	"Not Acceptable", "Authentication Required",
	// 5xx HEADER_SERVER_VALUE errors
	"Internal Error", "Not Implemented",
	"Bad Gateway", "Service Unavailable"
};

char* F_str(const __FlashStringHelper* str) {
  static char str_buffer[MAX_HEADER_LEN + 1] = {0};
  strcpy_P(str_buffer, (char*)str);
  return str_buffer;
}

const char*		get_default_body_from_status(short statusCode)
{
	static char	default_buf[MAX_STATUS_BODY_LEN + 1] = {0};
	
	for (uint32_t i = 0; i < sizeof(rest_status_codes); i++)
	{
		if (rest_status_codes[i] == statusCode) 
		{
			strcpy_P(default_buf, rest_status_bodies[i]);
			return default_buf;
		}
	}
	return 0;
}



#if defined(TEENSYDUINO) 
    //  --------------- Teensy -----------------
    #if defined(__AVR_ATmega32U4__)
        #define HEADER_SERVER_VALUE F("Teensy 2.0")
    #elif defined(__AVR_AT90USB1286__)       
        #define HEADER_SERVER_VALUE F("Teensy++ 2.0")
    #elif defined(__MK20DX128__)       
        #define HEADER_SERVER_VALUE F("Teensy 3.0")
    #elif defined(__MK20DX256__)       
        #define HEADER_SERVER_VALUE F("Teensy 3.2")
    #elif defined(__MKL26Z64__)       
        #define HEADER_SERVER_VALUE F("Teensy LC")
    #elif defined(__MK64FX512__)
        #define HEADER_SERVER_VALUE F("Teensy 3.5")
    #elif defined(__MK66FX1M0__)
        #define HEADER_SERVER_VALUE F("Teensy 3.6")
    #else
       #error "Unknown BOARD"
    #endif
#else // --------------- Arduino ------------------
    #if   defined(ARDUINO_AVR_ADK)       
        #define HEADER_SERVER_VALUE F("Mega Adk")
    #elif defined(ARDUINO_AVR_BT)    // Bluetooth
        #define HEADER_SERVER_VALUE F("Bt")
    #elif defined(ARDUINO_AVR_DUEMILANOVE)       
        #define HEADER_SERVER_VALUE F("Duemilanove")
    #elif defined(ARDUINO_AVR_ESPLORA)       
        #define HEADER_SERVER_VALUE F("Esplora")
    #elif defined(ARDUINO_AVR_ETHERNET)       
        #define HEADER_SERVER_VALUE F("Ethernet")
    #elif defined(ARDUINO_AVR_FIO)       
        #define HEADER_SERVER_VALUE F("Fio")
    #elif defined(ARDUINO_AVR_GEMMA)
        #define HEADER_SERVER_VALUE F("Gemma")
    #elif defined(ARDUINO_AVR_LEONARDO)       
        #define HEADER_SERVER_VALUE F("Leonardo")
    #elif defined(ARDUINO_AVR_LILYPAD)
        #define HEADER_SERVER_VALUE F("Lilypad")
    #elif defined(ARDUINO_AVR_LILYPAD_USB)
        #define HEADER_SERVER_VALUE F("Lilypad Usb")
    #elif defined(ARDUINO_AVR_MEGA)       
        #define HEADER_SERVER_VALUE F("Mega")
    #elif defined(ARDUINO_AVR_MEGA2560)       
        #define HEADER_SERVER_VALUE F("Mega 2560")
    #elif defined(ARDUINO_AVR_MICRO)       
        #define HEADER_SERVER_VALUE F("Micro")
    #elif defined(ARDUINO_AVR_MINI)       
        #define HEADER_SERVER_VALUE F("Mini")
    #elif defined(ARDUINO_AVR_NANO)       
        #define HEADER_SERVER_VALUE F("Nano")
    #elif defined(ARDUINO_AVR_NG)       
        #define HEADER_SERVER_VALUE F("NG")
    #elif defined(ARDUINO_AVR_PRO)       
        #define HEADER_SERVER_VALUE F("Pro")
    #elif defined(ARDUINO_AVR_ROBOT_CONTROL)       
        #define HEADER_SERVER_VALUE F("Robot Ctrl")
    #elif defined(ARDUINO_AVR_ROBOT_MOTOR)       
        #define HEADER_SERVER_VALUE F("Robot Motor")
    #elif defined(ARDUINO_AVR_UNO)       
        #define HEADER_SERVER_VALUE F("Uno")
    #elif defined(ARDUINO_AVR_YUN)       
        #define HEADER_SERVER_VALUE F("Yun")
    #elif defined(ARDUINO_SAM_DUE)       
        #define HEADER_SERVER_VALUE F("Due")
    #elif defined(ARDUINO_SAMD_ZERO)       
        #define HEADER_SERVER_VALUE F("Zero")
    #elif defined(ARDUINO_ARC32_TOOLS)       
        #define HEADER_SERVER_VALUE F("101")
    #else
       #error "Unknown BOARD"
    #endif
#endif

#endif
