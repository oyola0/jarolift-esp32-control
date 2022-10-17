#ifndef MQTT_MESSAGES_h
#define MQTT_MESSAGES_h

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

#endif