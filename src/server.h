#include "ESPAsyncWebServer.h"
#include "config.h"
#include "controller.h"
#include "mqtt.h"

AsyncWebServer server(80);

void startServer() {
  server.on("^\\/api\\/keepalive$", HTTP_GET, [](AsyncWebServerRequest *request) {
    println("API " + request->url() + " called");
    request->send(200, "text/plain", "{}");
  });

  server.on("^\\/api\\/up\\/(1|2)$", HTTP_POST, [](AsyncWebServerRequest *request) {
    println("API " + request->url() + " called");
    int channel = request->pathArg(0).toInt();
    addRequestPosition(100, channel);
    request->send(200, "text/plain", "{}");
  });

  server.on("^\\/api\\/down\\/(1|2)$", HTTP_POST, [](AsyncWebServerRequest *request) {
    println("API " + request->url() + " called");
    int channel = request->pathArg(0).toInt();
    addRequestPosition(0, channel);
    request->send(200, "text/plain", "{}");
  });

  server.on("^\\/api\\/stop\\/(1|2)$", HTTP_POST, [](AsyncWebServerRequest *request) {
    println("API " + request->url() + " called");
    int channel = request->pathArg(0).toInt();
    addRequest(gpioStop, channel);
    request->send(200, "text/plain", "{}");
  });

  server.on("^\\/api\\/middle\\/(1|2)$", HTTP_POST, [](AsyncWebServerRequest *request) {
    println("API " + request->url() + " called");
    int channel = request->pathArg(0).toInt();
    addRequestMiddle(channel);
    request->send(200, "text/plain", "{}");
  });

  server.begin();

  println("Server initialized");
}
