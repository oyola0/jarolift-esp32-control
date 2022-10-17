#ifndef MQTT_h
#define MQTT_h

#include "config.h"
#include "WiFi.h"
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);
TaskHandle_t TaskMQTT;

String messages[250];
int messageLength = 0;

String shiftMessages() {
  String tmpMessage = messages[0];            

  if (messageLength > 1) {
    for (int i = 0; i < messageLength; i++) {
      int nextIndex = i + 1;
      String temp = messages[nextIndex];
      messages[i] = temp;          
    }
  }  

  messageLength --;
  return tmpMessage;
}

void println(String msg) {
  messages[messageLength] = msg;
  messageLength ++;
  Serial.println(msg);
}

void connectMQTTClient() {
   while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD))
      Serial.println("MQTT connected");
    else {
      Serial.print(" MQTT failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void TaskMQTTCode(void * parameter){
  for(;;){
    if (!client.connected()) {
      connectMQTTClient();
    }
    client.loop();

    if (messageLength > 0) {
      String msg = shiftMessages();
      int msg_len = msg.length() + 1; 
      char msg_array[msg_len];
      msg.toCharArray(msg_array, msg_len);
      client.publish("ESP32/jarolift", msg_array);
      Serial.println("Published event: " + msg);
    }

    delay(500);
  }
}

void setupMQTT() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  connectMQTTClient();

  xTaskCreatePinnedToCore(
    TaskMQTTCode,
    "TaskMQTT",
    10000,
    NULL,
    1,
    &TaskMQTT,
    0); 
}

#endif