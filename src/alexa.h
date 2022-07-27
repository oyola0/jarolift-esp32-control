#include "SinricPro.h"
#include "SinricProBlinds.h"
#include "config.h"

bool onRangeValue(int &position, int channel) {
  Serial.printf("Channel %s set position to %d\r\n", String(channel), position);
  if (position == 0) {
    addRequest(gpioDown, channel);
  } else if (position == 100) {
    addRequest(gpioUp, channel);
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

bool onAdjustRangeValue(int &positionDelta, int channel) {
  Serial.printf("Channel %s set positionDelta to %d\r\n", String(channel), String(positionDelta));
  addRequestByPositionDelta(positionDelta, channel);
  return true;
}

bool onBlindDelta1(const String &deviceId, int &positionDelta) {
  return onAdjustRangeValue(positionDelta, 1);
}

bool onBlindDelta2(const String &deviceId, int &positionDelta) {
  return onAdjustRangeValue(positionDelta, 2);
}

bool onBlindState(bool &state, int channel) {
  Serial.printf("Channel %s turned to %s \r\n", String(channel), state ? "on" : "off");
  addRequest(gpioStop, channel);
  return true; // request handled properly
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
  myBlinds1.onAdjustRangeValue(onBlindDelta1);

  SinricProBlinds &myBlinds2 = SinricPro[BLIND_ID_2];
  myBlinds2.onPowerState(onBlindState2);
  myBlinds2.onRangeValue(onBlind2);
  myBlinds2.onAdjustRangeValue(onBlindDelta2);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void loopSinricPro() {
  SinricPro.handle();
}