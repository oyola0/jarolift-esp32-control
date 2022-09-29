#include <Arduino.h>
#include "WiFi.h"
#include "SPIFFS.h"
#include "alexa.h"
#include "CheckWifiStatus.h"

TaskHandle_t TaskRemoteController;

void TaskRemoteControllerCode(void * parameter){
  for(;;){    
    loopCheckController();
  }
}

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

  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());

  setupCheckWifiStatus();

  xTaskCreatePinnedToCore(
                    TaskRemoteControllerCode,   /* Task function. */
                    "TaskRemoteController",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    2,           /* priority of the task */
                    &TaskRemoteController,      /* Task handle to keep track of created task */
                    1);

  setupSinricPro();
  startServer();
}

void loop() {
  loopSinricPro();
}
