#ifndef CONTROLLER_h
#define CONTROLLER_h

#include "config.h"
#include "Request.h"
#include "mqttMessages.h"

int SHORT_PULSE = 500;
int LONG_PULSE = 4000;
int MIN_CHANNEL = 1;
int MAX_CHANNEL = 8;
float MOVE_TIME = 20.0;   // Time to full up

long resetTime = 0;
int currentChannel = MIN_CHANNEL;

int requestLength = 0;
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

void addRequestPosition(int targetPosition, int channel) {
  println("DEBUG: [controller.h] Channel: " + String(channel) + ", targetPosition: " + String(targetPosition));

  if (targetPosition == 1) {
    addRequestMiddle(channel);
  } else if (targetPosition <= 0) {
    blindPositions[channel] = 0;
    addRequest(gpioDown, channel);
  } else if (targetPosition >= 100) {
    blindPositions[channel] = 100;
    addRequest(gpioUp, channel);
  } else {
    requests[requestLength] = Request(channel, targetPosition);
    requestLength ++;   
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
  println("DEBUG: [controller.h] pulse button: " + btnName + ", During: " + String(time));

  digitalWrite(gpioBtn, HIGH);
  delay(time);
  digitalWrite(gpioBtn, LOW);
  delay(SHORT_PULSE);
  resetTime = millis() + 60000;
}

void executeRequest(Request req) {
  int targetPosition = req.getTargetPos();

  if (targetPosition == 0) {
    pulseButton(req.getGpioPin(), req.getTime());    
  } else {
    int positions = 0;
    int channel = req.getChannel();
    int currentPosition = blindPositions[channel];
    float speed = 0.95;

    if (targetPosition > currentPosition) {
      positions = targetPosition - currentPosition;
      speed = 1.5;
      pulseButton(gpioUp, SHORT_PULSE);
    } else if (targetPosition < currentPosition) {
      positions = currentPosition - targetPosition;
      pulseButton(gpioDown, SHORT_PULSE);
    }

    blindPositions[channel] = targetPosition;
    float delayPercent = MOVE_TIME / 100;
    int timeDelay = (int) (speed * positions * delayPercent * 1000);
    delay(timeDelay);
    pulseButton(gpioStop, SHORT_PULSE);
  }
}

void setChannel(int channel) {
  while (currentChannel != channel) {
    currentChannel++;
    if (currentChannel > MAX_CHANNEL) {
      currentChannel = MIN_CHANNEL;
    }
    println("DEBUG: [controller.h] Current channel: " + String(currentChannel));
    pulseButton(gpioChangeChannel, SHORT_PULSE);    
  }
}

TaskHandle_t TaskRemoteController;

void TaskRemoteControllerCode(void * parameter){
  for(;;) {    
    if (requestLength > 0) {
      Request currentRequest = shiftRequests();
      setChannel(currentRequest.getChannel());
      executeRequest(currentRequest);
    } else {
      if (millis() > resetTime) {
        setChannel(MIN_CHANNEL);
      }    
    }
    delay(100);
  }
}

void setupController() {
    xTaskCreatePinnedToCore(
      TaskRemoteControllerCode,   /* Task function. */
      "TaskRemoteController",     /* name of task. */
      10000,       /* Stack size of task */
      NULL,        /* parameter of the task */
      3,           /* priority of the task */
      &TaskRemoteController,      /* Task handle to keep track of created task */
      0);
}

#endif