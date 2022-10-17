#include <Arduino.h>
#include "WiFi.h"
#include "SPIFFS.h"
#include "alexa.h"
#include "CheckWifiStatus.h"
#include "mqtt.h"

void setup() {
  Serial.begin(115200);

  pinMode(gpioChangeChannel, OUTPUT);
  pinMode(gpioUp, OUTPUT);
  pinMode(gpioStop, OUTPUT);
  pinMode(gpioDown, OUTPUT);

  WiFi.begin(WiFiSSID, WiFiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    println("Connecting to WiFi...");
  }

  println("ESP32 IP: " + WiFi.localIP().toString());
  println(WiFi.getHostname());

  setupMQTT();
  setupCheckWifiStatus();
  setupController();
  setupSinricPro();
  startServer();
}

void loop() {
  loopSinricPro();
}