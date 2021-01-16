/**************************************************************

   Write by Bui Van Huy

 **************************************************************/

#include <ESP8266WiFi.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#define  FLASH_BUTTON 0

void setup() {
  pinMode(FLASH_BUTTON, INPUT_PULLUP);


  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("CONFIG");
  Serial.println("connected...yeey :)");

  //getToken In memory
  Serial.print("token = "); Serial.println(wifiManager.getToken());
}

void loop() {
  if (!digitalRead(FLASH_BUTTON)) {
    delay(500);
    WiFiManager wifiManager;
    wifiManager.startConfigPortal("CONFIG");
  }
}
