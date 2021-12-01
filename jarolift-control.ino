// Import required libraries
#include "WiFi.h"
#include "SPIFFS.h"

#include "config.h"
#include "controller.h"
#include "server.h"

void setup() {
    Serial.begin(115200);

    pinMode(gpioChangeChannel, OUTPUT);
    pinMode(gpioUp, OUTPUT);
    pinMode(gpioStop, OUTPUT);
    pinMode(gpioDown, OUTPUT);

    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Connect to Wi-Fi
    WiFi.begin(WiFiSSID, WiFiPassword);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());

    startServer();
}

void loop() {
    loopCheckController();
}
