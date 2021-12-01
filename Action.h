#ifndef Pulsable_h
#define Pulsable_h

class Action {
  private:
    int _time;
    int _gpioPin;
    int _channel;   

  public:
    Action(int gpioPin, int channel, int time) {    
      this->_gpioPin = gpioPin;
      this->_channel = channel;
      this->_time = time;
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
};

#endif