
int channelIO = 32;
int upIO = 33;
int stopIO = 25;
int downIO = 26;

int SHORT_PULSE = 500;
int LONG_PULSE = 4000;
int MIN_CHANNEL = 1;
int MAX_CHANNEL = 8;

String actions = "";
long nextExecution = 0;
bool isExecuting = false;
int currentChannel = MIN_CHANNEL;

void loopCheckController() {
    if(!isExecuting && millis() > nextExecution) {
        nextExecution = millis() + 100;
        // Serial.println("Check Controller: " + actions);
    }
}

void startGpio() {
    pinMode(channelIO, OUTPUT);
    pinMode(upIO, OUTPUT);
    pinMode(stopIO, OUTPUT);
    pinMode(downIO, OUTPUT);
}

void shortPulse(int btnIO) {
    digitalWrite(btnIO, HIGH);
    delay(SHORT_PULSE);
    digitalWrite(btnIO, LOW);
    delay(SHORT_PULSE);
}

void increaseChannel() {
    currentChannel++;
    if (currentChannel > MAX_CHANNEL) {
        currentChannel = MIN_CHANNEL;
    }
    Serial.println("Current channel: " + String(currentChannel));
    shortPulse(channelIO);
}

void addAction(String button, String channelStr) {
    int channel = channelStr.toInt();

    while (currentChannel != channel) {
        increaseChannel();
    }

    if(button == "up") {
        shortPulse(upIO);  
    } else if(button == "stop") {
        shortPulse(stopIO);
    } else if(button == "down") {
        shortPulse(downIO);
    } 
}

