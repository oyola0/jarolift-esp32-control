#ifndef Request_h
#define Request_h

class Request {
  private:
    int _time;
    int _gpioPin;
    int _channel;
    int _targetPos = 0;

  public:
    Request() {}

    Request(int gpioPin, int channel, int time) {    
      this->_gpioPin = gpioPin;
      this->_channel = channel;
      this->_time = time;      
    }

    Request(int channel, int targetPos) {    
      this->_channel = channel;
      this->_targetPos = targetPos;
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

    int getTargetPos() {
      return _targetPos;
    }
};

#endif
