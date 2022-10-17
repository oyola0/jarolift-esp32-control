#include "SinricPro.h"
#include "SinricProBlinds.h"
#include "config.h"
#include "mqtt.h"

bool onRangeValue(int &position, int channel) {
  println("Channel " + String(channel) + " set position to " + String(position));
  if (position <= 0) {
    addRequestPosition(0, channel);
  } else if (position == 100 || position == 10) {
    addRequestPosition(100, channel);
  } else {
    addRequestPosition(position, channel);
  }
  
  return true;
}

bool onBlind1(const String &deviceId, int &position) {
  return onRangeValue(position, 1);
}

bool onBlind2(const String &deviceId, int &position) {
  return onRangeValue(position, 2);
}

bool onBlindState(bool &state, int channel) {
  String value = state ? "ON" : "OFF";
  println("Channel " + String(channel) + " turned to " + value);
  addRequest(gpioStop, channel);
  return true;
}

bool onBlindState1(const String &deviceId, bool &state) {
  return onBlindState(state, 1);
}

bool onBlindState2(const String &deviceId, bool &state) {
  return onBlindState(state, 2);
}

void setupSinricPro() {
  // get a new Blinds device from SinricPro
  SinricProBlinds &myBlinds1 = SinricPro[BLIND_ID_1];
  myBlinds1.onPowerState(onBlindState1);
  myBlinds1.onRangeValue(onBlind1);
  myBlinds1.onAdjustRangeValue(onBlind1);

  SinricProBlinds &myBlinds2 = SinricPro[BLIND_ID_2];
  myBlinds2.onPowerState(onBlindState2);
  myBlinds2.onRangeValue(onBlind2);
  myBlinds2.onAdjustRangeValue(onBlind2);

  // setup SinricPro
  SinricPro.onConnected([](){ 
    println("Connected to SinricPro");
  });

  SinricPro.onDisconnected([](){ 
    println("Disconnected from SinricPro");
    delay(10000);
    ESP.restart();
  });

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void loopSinricPro() {
  SinricPro.handle();
}