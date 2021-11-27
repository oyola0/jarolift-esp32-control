String actions = "";

long nextExecution = 0;

void loopCheckController() {
    if(millis() > nextExecution) {
        nextExecution = millis() + 1000;
        Serial.println("Check Controller: " + actions);
    }    
}

void addAction(String button, String channel) {
    actions += "[" + button + "_" + channel + "]";
}