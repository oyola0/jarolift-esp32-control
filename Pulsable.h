#ifndef Pulsable_h
#define Pulsable_h

class Pulsable {
  private:
    int _state;
    int _gpioPin;
    int _time;

  public:
    Pulsable(int gpioPin, int state, int time) {    
      this->_gpioPin = gpioPin;
      this->_state = state;
      this->_time = time;
    }

    int getTime() {
      return _time;
    }

    int getGpioPin() {
      return _gpioPin;
    }

    int getState() {
      return _state;
    }

    String toString() {
      return "PIN=" + String(_gpioPin) + ", TIME=" + String(_time) + ", STATE=" + String(_state); 
    }
};

#endif