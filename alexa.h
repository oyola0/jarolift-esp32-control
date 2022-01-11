#include "SinricPro.h"
#include "SinricProBlinds.h"
#include "config.h"

bool onBlindGeneric(int &position, int channel) {
    Serial.printf("Channel %s set position to %d\r\n", String(channel), position);
    if (position == -10) {
        addRequest(gpioDown, channel);
    } else if (position == 10) {
        addRequest(gpioUp, channel);
    } else {
        addRequestMiddle(channel);
    }
    return true;
}

bool onBlind1(const String &deviceId, int &position) {
    onBlindGeneric(position, 1);
}

bool onBlind2(const String &deviceId, int &position) {
    onBlindGeneric(position, 2);
}

bool onBlindState(bool &state, int channel) {
  Serial.printf("Channel %s turned to %s \r\n", String(channel), state?"on":"off");
  addRequest(gpioStop, channel);
  return true; // request handled properly
}

bool onBlindState1(const String &deviceId, bool &state) {
    onBlindState(state, 1);
}

bool onBlindState2(const String &deviceId, bool &state) {
    onBlindState(state, 2);
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
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void loopSinricPro() {
    SinricPro.handle();
}