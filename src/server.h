#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "config.h"
#include "controller.h"

AsyncWebServer server(80);

void responseRequest(AsyncWebServerRequest *request, int position) {
  Serial.println("API " + request->url() + " called");

  int channel = request->pathArg(0).toInt();

  if(channel == 0) {
    for (int i = 1; i <= availableChannels; i++) {
      addRequestPosition(position, i);        
    }
  } else {
    addRequestPosition(position, channel);
  }    

  request->send(200, "text/plain", "{}");
}

void startServer() {
  server.on("^\\/api\\/middle\\/channel\\/(0|1|2)$", HTTP_GET, [](AsyncWebServerRequest *request) { 
    Serial.println("API " + request->url() + " called");
    int channel = request->pathArg(0).toInt();

    if(channel == 0) {
      for (int i = 1; i <= availableChannels; i++) {
        addRequestMiddle(i);        
      }
    } else {
      addRequestMiddle(channel);
    }    

    request->send(200, "text/plain", "{}");
  });

  server.on("^\\/api\\/button\\/up\\/channel\\/(0|1|2)$", HTTP_GET, [](AsyncWebServerRequest *request) {
    responseRequest(request, 100);
  });

  server.on("^\\/api\\/button\\/stop\\/channel\\/(0|1|2)$", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("API " + request->url() + " called");

    int channel = request->pathArg(0).toInt();

    if(channel == 0) {
      for (int i = 1; i <= availableChannels; i++) {
        addRequest(gpioStop, i);        
      }
    } else {
      addRequest(gpioStop, channel);
    }    

    request->send(200, "text/plain", "{}");
  });

  server.on("^\\/api\\/button\\/down\\/channel\\/(0|1|2)$", HTTP_GET, [](AsyncWebServerRequest *request) {
    responseRequest(request, 0);        
  });

  server.on("^\\/api\\/esp32\\/(.*)$", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Funcionalidad deshabilitada: " + request->url());
    request->send(200, "text/plain", "");
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { 
    request->send(SPIFFS, "/index.html", "text/html"); 
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
  
  server.on("^(.*).png$", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, request->pathArg(0) + ".png", "image/png");     
  });

  server.on("^(.*)$", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
  });
  
  server.begin();

  Serial.println("Server initialized");
}
