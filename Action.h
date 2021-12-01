#include "Pulsable.h"

#ifndef Action_h
#define Action_h

class Action {
  private:
    int _time;
    int _gpioPin;
    int _channel;    
   
    int pulsablesLength = 0;
    Pulsable pulsables[18] = {
      Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0),
      Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0),
      Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0),Pulsable(0,0,0)};

  public:
    Action(int gpioPin, int channel, int time) {    
      this->_gpioPin = gpioPin;
      this->_channel = channel;
      this->_time = time;      
    }

    void createPulsable(int gpioPin, int state, int time) {
        pulsables[pulsablesLength] = Pulsable(gpioPin, state, time);
        pulsablesLength ++;
    }

    int getTime() {
      return _time;
    }

    int getGpioPin() {
      return _gpioPin;
    }

    int getChannel() {
      return _channel;
    }

    bool hasPulsables() {
      return pulsablesLength > 0;
    }

    Pulsable shiftPulsable() {
      Pulsable tmpPulsable = pulsables[0];            

      if (pulsablesLength > 1) {
        for (int i = 0; i < pulsablesLength; i++) {
              int nextIndex = i + 1;
              Pulsable temp = pulsables[nextIndex];
              pulsables[i] = temp;
          }
      }  

      pulsablesLength --;
      return tmpPulsable;
    }

    String toString() {
      return "PIN=" + String(_gpioPin) + ", TIME=" + String(_time) + ", CHANNEL=" + String(_channel); 
    }
};

#endif
