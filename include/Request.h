#include "Pulsable.h"

#ifndef Request_h
#define Request_h

class Request {
  private:
    int _time;
    int _gpioPin;
    int _channel;

    int _delay = 0;    
    int pulsablesLength = 0;
    Pulsable pulsables[18];

  public:
    Request() {}

    Request(int gpioPin, int channel, int time) {    
      this->_gpioPin = gpioPin;
      this->_channel = channel;
      this->_time = time;      
    }

    Request(int gpioPin, int channel, int time, int delay) {    
      this->_gpioPin = gpioPin;
      this->_channel = channel;
      this->_time = time;      
      this->_delay = delay;
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

    int getDelay() {
      return _delay;
    }

    void resetDelay() {
      this->_delay = 0;
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
