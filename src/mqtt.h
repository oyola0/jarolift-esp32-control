#ifndef MQTT_h
#define MQTT_h

#include "config.h"
#include "WiFi.h"
#include "controller.h"
#include "mqttMessages.h"
#include "Message.h"
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);
TaskHandle_t TaskMQTT;
String ClientName = "ESP32_Jarolift_" + String(esp_random());

void callback(char *topic, byte *payload, unsigned int length) {
  String topicStr = String(topic);
  String channelStr = String((char) payload[0]);
  int channel = 1;

  println_debug("[mqtt.h] Message: " + channelStr + " arrived in topic: " + topicStr);

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
    int clientName_len = ClientName.length() + 1; 
    char clientName_array[clientName_len];
    ClientName.toCharArray(clientName_array, clientName_len);

    println("[mqtt.h] Connecting to MQTT...");
    if (client.connect(clientName_array, MQTT_USER, MQTT_PASSWORD)) {
      println("[mqtt.h] MQTT Client (" + ClientName + ") Connected");
      client.subscribe("ESP32/jarolift/up");
      client.subscribe("ESP32/jarolift/stop");
      client.subscribe("ESP32/jarolift/down");
      client.subscribe("ESP32/jarolift/middle");
    } else {
      println_error("[mqtt.h] MQTT failed with state: " + String(client.state()));
      delay(10000);
    }
  }
}

void TaskMQTTCode(void * parameter){
  for(;;){
    if (!client.connected()) {
      println_error("[mqtt.h] MQTT Client (" + ClientName + ") Disconnected");
      client.disconnect();
      connectMQTTClient();
    }

    client.loop();

    while (messageLength > 0) {
      Message message = shiftMessages();
      String msg = message.getJSON();
      int msg_len = msg.length() + 1; 
      char msg_array[msg_len];
      msg.toCharArray(msg_array, msg_len);
      client.publish("ESP32/jarolift/logs", msg_array);
      Serial.println("Published event: " + msg +  "");
      delay(10);
    }

    delay(10);
  }
}

void setupMQTT() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  // connectMQTTClient();

  xTaskCreatePinnedToCore(
    TaskMQTTCode,
    "TaskMQTT",
    10000,
    NULL,
    2,
    &TaskMQTT,
    0); 
}

#endif