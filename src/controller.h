#include "config.h"
#include "Request.h"

int SHORT_PULSE = 500;
int LONG_PULSE = 4000;
int MIN_CHANNEL = 1;
int MAX_CHANNEL = 8;
float MOVE_TIME = 23.0;   // Time to full up

long resetTime = 0;
int currentChannel = MIN_CHANNEL;

int requestLength = 0;
Request currentRequest;
Request requests[100];

int blindPositions[] = {100, 100, 100};

void addRequest(int pinGpio, int channel) {
  requests[requestLength] = Request(pinGpio, channel, SHORT_PULSE);
  requestLength ++;
}

void addRequestMiddle(int channel) {
  blindPositions[channel] = 30;
  requests[requestLength] = Request(gpioStop, channel, LONG_PULSE);
  requestLength ++;
}

void addRequestWithDelay(int gpioBtn, int channel, int pos, int targetPosition) {
    float delayPercent = MOVE_TIME / 100;
    int delay = (int) (pos * delayPercent * 1000);
    blindPositions[channel] = targetPosition;
    requests[requestLength] = Request(gpioBtn, channel, SHORT_PULSE, delay);
    requestLength ++;
}

void addRequestPosition(int targetPosition, int channel) {
  int currentPosition = blindPositions[channel];

  Serial.println("CurrentPosition: " + String(currentPosition));
  Serial.println("TargetPosition: " + String(targetPosition));
  
  if (targetPosition == 1) {
    addRequestMiddle(channel);
  } else if (targetPosition <= 0) {
    blindPositions[channel] = 0;
    addRequest(gpioDown, channel);
  } else if (targetPosition >= 100) {
    blindPositions[channel] = 100;
    addRequest(gpioUp, channel);
  } else if (targetPosition > currentPosition) {
    int positions = targetPosition - currentPosition;
    addRequestWithDelay(gpioUp, channel, positions, targetPosition);
  } else if (targetPosition < currentPosition) {
    int positions = currentPosition - targetPosition;
    addRequestWithDelay(gpioDown, channel, positions, targetPosition);
  }
}

Request shiftRequests() {
  Request tmpRequest = requests[0];            

  if (requestLength > 1) {
    for (int i = 0; i < requestLength; i++) {
      int nextIndex = i + 1;
      Request temp = requests[nextIndex];
      requests[i] = temp;          
    }
  }  

  requestLength --;
  return tmpRequest;
}

void pulseButton(int gpioBtn, int time) {
  String btnName = "stop";
  if (gpioBtn == gpioUp) {
    btnName = "up";
  } else if (gpioBtn == gpioDown) {
    btnName = "down";
  } else if (gpioBtn == gpioChangeChannel) {
    btnName = "change channel";
  }
  Serial.println("pulse button: " + btnName + ", During: " + String(time));

  digitalWrite(gpioBtn, HIGH);
  delay(time);
  digitalWrite(gpioBtn, LOW);
  delay(SHORT_PULSE);
  resetTime = millis() + 60000;
}

void executeRequest(Request req) {
  pulseButton(req.getGpioPin(), req.getTime());
  if (req.getDelay() > 0) {
    delay(req.getDelay());
    pulseButton(gpioStop, SHORT_PULSE);
  }
}

void setChannel(int channel) {
  while (currentChannel != channel) {
    currentChannel++;
    if (currentChannel > MAX_CHANNEL) {
      currentChannel = MIN_CHANNEL;
    }
    Serial.println("Current channel: " + String(currentChannel));
    pulseButton(gpioChangeChannel, SHORT_PULSE);    
  }
}

void loopCheckController() {
  if (requestLength > 0) {
    currentRequest = shiftRequests();
    Serial.println("Request " + currentRequest.toString());  
    setChannel(currentRequest.getChannel());
    executeRequest(currentRequest);
  } else {
    if (millis() > resetTime) {
      setChannel(MIN_CHANNEL);
    }    
  }
  delay(100);
}


TaskHandle_t TaskRemoteController;

void TaskRemoteControllerCode(void * parameter){
  for(;;){    
    loopCheckController();
    delay(10);
  }
}

void setupController() {
    xTaskCreatePinnedToCore(
      TaskRemoteControllerCode,   /* Task function. */
      "TaskRemoteController",     /* name of task. */
      10000,       /* Stack size of task */
      NULL,        /* parameter of the task */
      2,           /* priority of the task */
      &TaskRemoteController,      /* Task handle to keep track of created task */
      1);
}