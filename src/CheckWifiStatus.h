#include "mqttMessages.h"

TaskHandle_t TaskCheckWifiStatus;

void TaskCheckWifiStatusCode(void * parameter){
  for(;;){
    if (WiFi.status() != WL_CONNECTED) {
        println("Restarting in 90 seconds");
        delay(90000);
        ESP.restart();      
    }
    delay(5000);
  }
}

void setupCheckWifiStatus() {
    xTaskCreatePinnedToCore(
      TaskCheckWifiStatusCode,
      "TaskCheckWifiStatus",
      10000,
      NULL,
      1,
      &TaskCheckWifiStatus,
      1); 
}