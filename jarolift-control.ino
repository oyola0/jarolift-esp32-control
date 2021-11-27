// Import required libraries
#include "config.h"
#include "controller.h"
#include "server.h"

void setup() {
    startServer();
}

void loop() {
    loopCheckController();
}
