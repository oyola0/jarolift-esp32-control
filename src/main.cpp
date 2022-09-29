#include <Arduino.h>
#include "WiFi.h"
#include "SPIFFS.h"
#include "alexa.h"
#include "CheckWifiStatus.h"

void setup() {
  Serial.begin(115200);

  pinMode(gpioChangeChannel, OUTPUT);
  pinMode(gpioUp, OUTPUT);
  pinMode(gpioStop, OUTPUT);
  pinMode(gpioDown, OUTPUT);

  WiFi.begin(WiFiSSID, WiFiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println(WiFi.localIP());
  Serial.println(WiFi.getHostname());

  setupCheckWifiStatus();
  setupController();
  setupSinricPro();
  startServer();
}

void loop() {
  loopSinricPro();
}
