#ifndef MQTT_h
#define MQTT_h

#include "config.h"
#include "WiFi.h"
#include "controller.h"
#include "mqttMessages.h"
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);
TaskHandle_t TaskMQTT;

void callback(char *topic, byte *payload, unsigned int length) {
  String topicStr = String(topic);
  String channelStr = String((char) payload[0]);
  int channel = 1;

  println("Message: " + channelStr + " arrived in topic: " + topicStr);

  if (channelStr.equals("1")) {
    channel = 1;
  } else if(channelStr.equals("2")) {
    channel = 2;
  } else {
    return;
  }

  if (topicStr.endsWith("up")) {
    addRequestPosition(100, channel);
  } else if (topicStr.endsWith("stop")) {
    addRequest(gpioStop, channel);
  } else if (topicStr.endsWith("down")) {
    addRequestPosition(0, channel);
  } else {
    addRequestMiddle(channel);
  }
}

void connectMQTTClient() {
   while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("MQTT connected");
      client.subscribe("ESP32/jarolift/up");
      client.subscribe("ESP32/jarolift/stop");
      client.subscribe("ESP32/jarolift/down");
      client.subscribe("ESP32/jarolift/middle");
    } else {
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
      client.publish("ESP32/jarolift/logs", msg_array);
      Serial.println("Published event: " + msg);
    }

    delay(100);
  }
}

void setupMQTT() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
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