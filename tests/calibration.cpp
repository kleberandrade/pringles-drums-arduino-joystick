// Deliberately do not define DRUMS_ENABLE_HID: test the safe source default.
#include "fakes/Arduino.h"
#include "fakes/Keyboard.h"
#include <cassert>
#include <iostream>
uint32_t clockMs = 0;
int adc[2] = {0, 0};
int armLevel = LOW; // Even an armed board must not type in calibration mode.
int pinModes[32] = {};
SerialFake Serial;
KeyboardFake Keyboard;
#include "../src/main.cpp"
int main() {
  setup();
  for (clockMs = 0; clockMs < 200; ++clockMs) {
    adc[0] = adc[1] = clockMs % 60 < 30 ? 0 : 1023;
    loop();
  }
  assert(!Keyboard.started);
  assert(Keyboard.events.empty());
  std::cout << "Calibration default passed without USB keyboard calls\n";
}
