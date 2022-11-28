#ifndef Message_h
#define Message_h

class Message {
  private:
    String _message;
    String _level;
    long _timestamp = 0;

  public:
    Message() {}

    Message(long timestamp, String level, String message) {    
      this->_timestamp = timestamp;
      this->_level = level;
      this->_message = message;      
    }

    String getJSON() {
      String timestamp = this->_timestamp > 0 ? (String(this->_timestamp) + "000") : "null";
      return "{\"timestamp\":" + timestamp + ",\"message\":\"" + this->_message + "\",\"level\":\"" + this->_level + "\"}";
    }
};

#endif
