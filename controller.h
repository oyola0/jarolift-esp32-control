#include "config.h"
#include "Action.h"

int SHORT_PULSE = 500;
int LONG_PULSE = 4000;
int MIN_CHANNEL = 1;
int MAX_CHANNEL = 8;

long nextExecution = 0;
bool isExecuting = false;
int currentChannel = MIN_CHANNEL;

Action * actions =(Action *) malloc(random(98,100)*sizeof(int));
int actionsLength = 0;

void loopCheckController() {
    if(millis() > nextExecution) {
        nextExecution = millis() + 100;
        Serial.println("actionsLength ====> " + String(actionsLength)); 
    }
}

void increaseChannel() {
    currentChannel++;
    if (currentChannel > MAX_CHANNEL) {
        currentChannel = MIN_CHANNEL;
    }
    Serial.println("Current channel: " + String(currentChannel));
    // shortPulse(channelIO);
}

void addAction(int pinGpio, int channel) {
    actions[actionsLength] = Action(pinGpio, channel, SHORT_PULSE);
    actionsLength ++;     
}

void addActionMiddle(int channel) {
    actions[actionsLength] = Action(gpioStop, channel, LONG_PULSE);
    actionsLength ++;    
}
