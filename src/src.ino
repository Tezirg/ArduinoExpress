#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>

// Emulate Serial1 on pins 6/7 if not present
#if (!defined(HAVE_HWSERIAL1) && !defined(TEENSYDUINO))
  #include <SoftwareSerial.h>
  SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#include "RestServer.hpp"

WiFiEspServer                             esp_server(80);
RestServer<WiFiEspServer, WiFiEspClient>  rest_server(esp_server);

void      handleRoot(RestRequest& req, RestResponse& res)
{
  res.status(200);
  res.type("application/json");
  String body = "{ \"Test\": 42, \"array-test\": [0,1,2,3,4,5]}";
  res.send(body.c_str());
}

void      setup() 
{
  // Init Serial communications
  Serial.begin(9600);
  delay(250);
  Serial1.begin(115200);
  delay(250);

  // Init ESP WiFi server
  WiFi.init(&Serial1);
  // We'll use ip 192.168.0.10
  IPAddress localIp(192, 168, 0, 10);
  WiFi.configAP(localIp);
  // Create AP
  int status = WiFi.beginAP("esp_wifi", 10, "123456789", ENC_TYPE_WPA2_PSK);
  Serial.println("Access point started");
  esp_server.begin();
  Serial.println("Server started");

  // Add basic route
  rest_server.All("/", handleRoot);
}

void      loop() 
{
  rest_server.Run();
  delay(100);
}
