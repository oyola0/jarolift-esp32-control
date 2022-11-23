#include "config.h"
#include "WiFi.h"
#include "mqttMessages.h"

TaskHandle_t TaskWifiConnection;

void TaskWifiConnectionCode(void * parameter){
  for(;;){
    if (WiFi.status() != WL_CONNECTED) {
        println("ERROR: [WifiConnection.h] Reconnecting to WiFi in 10 seconds...");
        delay(10000);
        WiFi.disconnect();
        WiFi.reconnect();  
    }
    delay(10000);
  }
}

void setupWifiConnection() {
  WiFi.begin(WiFiSSID, WiFiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    println("[WifiConnection.h] Connecting to WiFi...");
  }

  println("[WifiConnection.h] Wifi Connected, ESP32 Jarolift IP: " + WiFi.localIP().toString());

  xTaskCreatePinnedToCore(
    TaskWifiConnectionCode,
    "TaskWifiConnection",
    10000,
    NULL,
    1,
    &TaskWifiConnection,
    0); 
}