#include <Arduino.h>
#include "WiFi.h"
#include "SPIFFS.h"
#include "alexa.h"

IPAddress staticIP(192, 168, 1, 201);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(80, 58, 61, 250);
IPAddress secondaryDNS(80, 58, 61, 254);

void setup() {
  Serial.begin(115200);

  pinMode(gpioChangeChannel, OUTPUT);
  pinMode(gpioUp, OUTPUT);
  pinMode(gpioStop, OUTPUT);
  pinMode(gpioDown, OUTPUT);
  
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  if (!WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Configuration failed.");
  }

  WiFi.begin(WiFiSSID, WiFiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println(WiFi.localIP());

  setupSinricPro();
  startServer();
}

void loop() {
  loopSinricPro();
  
  loopCheckController();
}
