#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>

// Emulate Serial1 on pins 6/7 if not present
#if (!defined(HAVE_HWSERIAL1) && !defined(TEENSYDUINO))
  #include <SoftwareSerial.h>
  SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#include "RestServer.hpp" 

char ssid[] = "";              // your network SSID (name)
char pass[] = "";              // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiEspServer                             esp_server(80);
RestServer<WiFiEspServer, WiFiEspClient>  rest_server(esp_server);

void      handleRedirect(RestRequest& req, RestResponse& res)
{
  res.redirect("https://google.com");
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
  // Connect to Wifi
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

  // Add redirection handler
  rest_server.All("/redirect", handleRedirect);
}

void      loop() 
{
  rest_server.Run();
  delay(100);
}
