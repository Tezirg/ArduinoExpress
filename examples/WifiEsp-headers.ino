#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>

// Emulate Serial1 on pins 6/7 if not present
#if (!defined(HAVE_HWSERIAL1) && !defined(TEENSYDUINO))
  #include <SoftwareSerial.h>
  SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#include "RestServer.hpp"

char ssid[] = "SSID";            // your network SSID (name)
char pass[] = "PASSWORD";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiEspServer                             esp_server(80);
RestServer<WiFiEspServer, WiFiEspClient>  rest_server(esp_server);

void      handleRoot(RestRequest& req, RestResponse& res)
{
  Serial.println(req.method);
  Serial.println(req.baseUrl);
  res.status(200);
  res.type("text/plain");
  String body = "Your browser is " + String(req.get("User-Agent"));
  res.send(body.c_str());
}

void      setup() 
{
  // Init Serial communications
  Serial.begin(100000);
  delay(250);
  Serial1.begin(115200);
  delay(250);

  // Init ESP WiFi server
  WiFi.init(&Serial1);
  // Connect to Wifi
  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
  Serial.println("Connected to WiFi");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
  esp_server.begin();
  Serial.println("Server started");

  // Add basic route
  rest_server.All("/", handleRoot);
}

void      loop() 
{
  rest_server.Run();
}
