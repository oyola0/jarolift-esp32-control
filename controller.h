#include "config.h"
#include "Action.h"

int SHORT_PULSE = 500;
int LONG_PULSE = 4000;
int MIN_CHANNEL = 1;
int MAX_CHANNEL = 8;

long nextExecution = 0;
long resetTime = 0;
int currentChannel = MIN_CHANNEL;

int actionsLength = 0;
Action currentAction = Action(0, 0, 0);
Action actions[30] = {
      Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0),
      Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0),
      Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0),
      Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0),
      Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0),
      Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0),Action(0,0,0)};

void addAction(int pinGpio, int channel) {
    actions[actionsLength] = Action(pinGpio, channel, SHORT_PULSE);
    actionsLength ++;
}

void addActionMiddle(int channel) {
    actions[actionsLength] = Action(gpioStop, channel, LONG_PULSE);
    actionsLength ++;
}

Action shiftActions() {
    Action tmpAction = actions[0];            

    if (actionsLength > 1) {
       for (int i = 0; i < actionsLength; i++) {
            int nextIndex = i + 1;
            Action temp = actions[nextIndex];
            actions[i] = temp;          
        }
    }  

    actionsLength --;
    return tmpAction;
}

void setChannel(int channel) {
    while (currentChannel != channel) {
        currentChannel++;
        if (currentChannel > MAX_CHANNEL) {
            currentChannel = MIN_CHANNEL;
        }
        Serial.println("Current channel: " + String(currentChannel));
        currentAction.createPulsable(gpioChangeChannel, HIGH, SHORT_PULSE);
        currentAction.createPulsable(gpioChangeChannel, LOW, SHORT_PULSE);
    }
}

void loopCheckController() {
    if (millis() > nextExecution) {
        if (currentAction.hasPulsables()) {
            Pulsable pulsable = currentAction.shiftPulsable();
            Serial.println("Pulsable " + pulsable.toString());
            digitalWrite(pulsable.getGpioPin(), pulsable.getState());
            nextExecution = millis() + pulsable.getTime();
            resetTime = millis() + 60000;
        } else if (actionsLength > 0) {
            currentAction = shiftActions();
            Serial.println("Action " + currentAction.toString());
            setChannel(currentAction.getChannel());
            currentAction.createPulsable(currentAction.getGpioPin(), HIGH, currentAction.getTime());
            currentAction.createPulsable(currentAction.getGpioPin(), LOW, SHORT_PULSE);
        } else {
            if (millis() > resetTime) {
                setChannel(MIN_CHANNEL);
            }
            nextExecution = millis() + 100;
        }
    }
}
