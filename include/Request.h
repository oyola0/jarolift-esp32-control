#ifndef Request_h
#define Request_h

class Request {
  private:
    int _time;
    int _gpioPin;
    int _channel;
    int _delay = 0;    

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

    String toString() {
      return "PIN=" + String(_gpioPin) + ", TIME=" + String(_time) + ", CHANNEL=" + String(_channel); 
    }
};

#endif
