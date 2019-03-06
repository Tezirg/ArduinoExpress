# ArduinoExpress
Minimalist web framework for Arduino, inspired from Node.js Express

## Hardware compatibility
The library requires AT LEAST 4Ko of dynamic memory and AT LEAST 12Ko of program storage space.

Just tested on Teensy 3.2 but it should work with other boards

## Documentation
The library imitates Express as much as possible

### RestServer

 * [All](https://expressjs.com/en/4x/api.html#app.all): matches all HTTP verbs
 * [Get](https://expressjs.com/en/4x/api.html#app.get.method): Routes HTTP GET requests to the specified path with the specified callback functions.
 * [Post](https://expressjs.com/en/4x/api.html#app.post.method):Routes HTTP POST requests to the specified path with the specified callback functions. 
 * [Put](https://expressjs.com/en/4x/api.html#app.put): Routes HTTP PUT requests to the specified path with the specified callback functions.
 * Delete, Head, Options, Patch
 
### RestRequest
Represents the HTTP request and has properties for the request query string, parameters, body, HTTP headers, and so on.

Most of the [Request interface](https://expressjs.com/en/4x/api.html#req) is implemented.

### RestResponse
Represents the HTTP response that the RestServer sends when it gets an HTTP request.

Most of the [Response interface](https://expressjs.com/en/4x/api.html#res) is implemented.

