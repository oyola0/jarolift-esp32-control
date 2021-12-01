#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

AsyncWebServer server(80);

void responseAction(AsyncWebServerRequest *request, int pinGpio) {
    Serial.println("API " + request->url() + " called");
    addAction(pinGpio, request->pathArg(0).toInt());
    request->send(200, "text/plain", "{}");
}

void startServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { 
        request->send(SPIFFS, "/index.html", "text/html"); 
    });

    server.on("^\\/api\\/button\\/up\\/channel\\/(0|1|2)$", HTTP_GET, [](AsyncWebServerRequest *request) {
        responseAction(request, gpioUp);
    });

    server.on("^\\/api\\/button\\/stop\\/channel\\/(0|1|2)$", HTTP_GET, [](AsyncWebServerRequest *request) {
        responseAction(request, gpioStop);
    });

    server.on("^\\/api\\/button\\/down\\/channel\\/(0|1|2)$", HTTP_GET, [](AsyncWebServerRequest *request) {
        responseAction(request, gpioDown);        
    });

    server.on("^\\/api\\/middle\\/channel\\/(0|1|2)$", HTTP_GET, [](AsyncWebServerRequest *request) { 
        Serial.println("API " + request->url() + " called");
        addActionMiddle(request->pathArg(0).toInt());
        request->send(200, "text/plain", "{}");
    });

    server.on("^\\/api\\/esp32\\/(.*)$", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("Funcionalidad deshabilitada: " + request->url());
        request->send(200, "text/plain", "");
    });

    server.on("^(.*).png$", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, request->pathArg(0) + ".png", "image/png");     
    });
    
    server.on("^(.*).html$", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, request->pathArg(0) + ".html", "text/html");     
    });
    
    server.on("^(.*).js$", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, request->pathArg(0) + ".js", "text/javascript");     
    });

    server.on("^(.*).css$", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, request->pathArg(0) + ".css", "text/css");     
    });

    server.on("^(.*)$", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "Not found");
    });
    
    server.begin();
}
