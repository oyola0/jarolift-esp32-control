#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

AsyncWebServer server(80);

String getMimetype(String filePath) {
    if (filePath.endsWith(".html")) {
        return "text/html";
    } else if (filePath.endsWith(".png")) {
        return "image/png";
    } else if (filePath.endsWith(".css")) {
        return "text/css";
    } else if (filePath.endsWith(".js")) {
        return "text/javascript";
    } else {
        return "text/plain";
    }
}

void startServer() {
    // Serial port for debugging purposes
    Serial.begin(115200);

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

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { 
        request->send(SPIFFS, "/index.html", "text/html"); 
    });

    server.on("^\\/api\\/button\\/([a-zA-Z]+)\\/channel\\/([0-9])$", HTTP_GET, [](AsyncWebServerRequest *request) {
        String button = request->pathArg(0);
        String channel = request->pathArg(1);
        addAction(button, channel);
        request->send(200, "text/plain", "{}");
    });

    server.on("^\\/api\\/middle\\/channel\\/([0-9])$", HTTP_GET, [](AsyncWebServerRequest *request) { 
        String channel = request->pathArg(0); 
        addAction("middle", channel);
        request->send(200, "text/plain", "{}");
    });

    server.on("^\\/api\\/esp32\\/(.*)$", HTTP_GET, [](AsyncWebServerRequest *request) {
        String args = request->pathArg(0); 
        Serial.println("Funcionalidad deshabilitada: " + args);
        request->send(200, "text/plain", "");
    });

    server.on("^(.*)$", HTTP_GET, [](AsyncWebServerRequest *request) {
        String filePath = request->pathArg(0);

        if(SPIFFS.exists(filePath)) {
            request->send(SPIFFS, filePath, getMimetype(filePath)); 
        } else {
            request->send(404, "text/plain", "Not found");
        }      
    });

    // Start server
    server.begin();
}
