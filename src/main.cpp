#include <Arduino.h>
#include "WifiConnection.h"
#include "mqttMessages.h"
#include "mqtt.h"
#include "alexa.h"
#include "controller.h"

void setup() {
  Serial.begin(115200);

  pinMode(gpioChangeChannel, OUTPUT);
  pinMode(gpioUp, OUTPUT);
  pinMode(gpioStop, OUTPUT);
  pinMode(gpioDown, OUTPUT);
  
  setupMQTTMessages();
  setupWifiConnection();
  setupMQTT();  
  setupController();
  setupSinricPro();
}

void loop() {
  loopSinricPro();
}