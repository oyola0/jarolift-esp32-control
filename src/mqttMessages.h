#ifndef MQTT_MESSAGES_h
#define MQTT_MESSAGES_h

#include "Message.h"

TaskHandle_t TaskMQTTMessages;

Message messages[1000];
int messageLength = 0;

long current_timestamp_secs = 0;
long last_timestamp_secs  = 0;

void TaskMQTTMessagesCode(void * parameter){
  for(;;){    
    if (current_timestamp_secs > 0) {
      current_timestamp_secs += 1;
    }
    delay(1000);
  }
}

void setupMQTTMessages() {
  xTaskCreatePinnedToCore(
    TaskMQTTMessagesCode,
    "TaskMQTTMessages",
    10000,
    NULL,
    4,
    &TaskMQTTMessages,
    0); 
}

Message shiftMessages() {
  Message tmpMessage = messages[0];            

  if (messageLength > 1) {
    for (int i = 0; i < messageLength; i++) {
      int nextIndex = i + 1;
      Message temp = messages[nextIndex];
      messages[i] = temp;          
    }
  }  

  messageLength --;
  return tmpMessage;
}

void add_to_messages(String msg, String level) {
  if (messageLength >= 999) {
    shiftMessages();
  }

  Message newMessage = Message(current_timestamp_secs, level, msg);
  messages[messageLength] = newMessage;
  messageLength ++;
  Serial.println(msg);
}

void println_error(String msg) {
  add_to_messages(msg, "error");
}

void println_debug(String msg) {
  add_to_messages(msg, "debug"); 
}

void println(String msg) {
  add_to_messages(msg, "info");
}

void setTimestamp(long new_timestamp) {
  if (last_timestamp_secs != new_timestamp) {
    println_debug("Updating timestamp, old: " + String(last_timestamp_secs) + " new: " + String(new_timestamp) + "");
    last_timestamp_secs = new_timestamp;
    current_timestamp_secs = new_timestamp;
  }
}

#endif