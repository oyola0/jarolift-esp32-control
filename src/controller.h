#include "config.h"
#include "Request.h"

int SHORT_PULSE = 500;
int LONG_PULSE = 4000;
int MIN_CHANNEL = 1;
int MAX_CHANNEL = 8;
float MOVE_TIME = 23.0;   // Time to full up

long nextExecution = 0;
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
  requests[requestLength] = Request(gpioStop, channel, LONG_PULSE);
  requestLength ++;
  blindPositions[channel] = 30;
}

void addRequestPosition(int targetPosition, int channel) {
  int currentPosition = blindPositions[channel];
  float delayPercent = MOVE_TIME / 100;

  Serial.println("CurrentPosition: " + String(currentPosition));
  Serial.println("TargetPosition: " + String(targetPosition));
  
  if (targetPosition == 1) {
    addRequestMiddle(channel);
  } else if (targetPosition <= 0) {
    addRequest(gpioDown, channel);
    blindPositions[channel] = 0;
  } else if (targetPosition >= 100) {
    addRequest(gpioUp, channel);
    blindPositions[channel] = 100;
  } else if (targetPosition > currentPosition) {
    int positions = targetPosition - currentPosition;
    int delay = (int) (positions * delayPercent * 1000);
    blindPositions[channel] = targetPosition;
    requests[requestLength] = Request(gpioUp, channel, SHORT_PULSE, delay);
    requestLength ++;
  } else if (targetPosition < currentPosition) {
    int positions = currentPosition - targetPosition;
    int delay = (int) (positions * delayPercent * 1000);
    blindPositions[channel] = targetPosition;
    requests[requestLength] = Request(gpioDown, channel, SHORT_PULSE, delay);
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

void setChannel(int channel) {
  while (currentChannel != channel) {
    currentChannel++;
    if (currentChannel > MAX_CHANNEL) {
      currentChannel = MIN_CHANNEL;
    }
    Serial.println("Current channel: " + String(currentChannel));
    currentRequest.createPulsable(gpioChangeChannel, HIGH, SHORT_PULSE);
    currentRequest.createPulsable(gpioChangeChannel, LOW, SHORT_PULSE);
  }
}

void loopCheckController() {
  if (millis() > nextExecution) {
    if (currentRequest.hasPulsables()) {
      Pulsable pulsable = currentRequest.shiftPulsable();
      Serial.println("Pulsable " + pulsable.toString());
      digitalWrite(pulsable.getGpioPin(), pulsable.getState());
      nextExecution = millis() + pulsable.getTime();
      resetTime = millis() + 60000;
    } else if (currentRequest.getDelay() > 0) {
      nextExecution = millis() + currentRequest.getDelay() - SHORT_PULSE;
      Serial.println("Delay to " + String(currentRequest.getDelay()));
      currentRequest.createPulsable(gpioStop, HIGH, SHORT_PULSE);
      currentRequest.createPulsable(gpioStop, LOW, SHORT_PULSE);
      currentRequest.resetDelay();
    } else if (requestLength > 0) {
      currentRequest = shiftRequests();
      Serial.println("Request " + currentRequest.toString());
      setChannel(currentRequest.getChannel());
      currentRequest.createPulsable(currentRequest.getGpioPin(), HIGH, currentRequest.getTime());
      currentRequest.createPulsable(currentRequest.getGpioPin(), LOW, SHORT_PULSE);
    } else {
      if (millis() > resetTime) {
        setChannel(MIN_CHANNEL);
      }
      nextExecution = millis() + 100;
    }
  }
}
