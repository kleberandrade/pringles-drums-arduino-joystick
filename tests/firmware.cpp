#define DRUMS_ENABLE_HID 1
#include "fakes/Arduino.h"
#include "fakes/Keyboard.h"
#include <cassert>
#include <iostream>
uint32_t clockMs = 0;
int adc[2] = {0, 0};
int armLevel = HIGH;
int pinModes[32] = {};
SerialFake Serial;
KeyboardFake Keyboard;
#include "../src/main.cpp"

int main() {
  setup();
  assert(Keyboard.started);
  assert(Serial.baud == 115200);
  assert(pinModes[4] == INPUT_PULLUP);
  loop();
  assert(Keyboard.events.empty());
  clockMs = 1; loop(); // Populate filter with idle samples.
  armLevel = LOW;
  adc[0] = adc[1] = 40;
  clockMs = 2; loop();
  assert(Keyboard.events.size() == 2);
  assert(Keyboard.events[0].key == 'w' && Keyboard.events[0].pressed);
  assert(Keyboard.events[1].key == 'd' && Keyboard.events[1].pressed);
  clockMs = 21; loop();
  assert(Keyboard.events.size() == 2);
  clockMs = 22; loop();
  assert(Keyboard.events.size() == 4);
  assert(Keyboard.events[2].key == 'w' && !Keyboard.events[2].pressed);
  assert(Keyboard.events[3].key == 'd' && !Keyboard.events[3].pressed);
  adc[0] = adc[1] = 0;
  for (clockMs = 23; clockMs < 70; ++clockMs) loop();
  adc[0] = 40;
  for (; clockMs < 78; ++clockMs) loop();
  assert(Keyboard.events.size() == 5);
  armLevel = HIGH;
  loop(); // Disarm even with no new sample/time advance.
  assert(Keyboard.events.size() == 6);
  assert(Keyboard.events.back().key == 'w' && !Keyboard.events.back().pressed);
  Serial.connected = true;
  Serial.capacity = 0;
  clockMs = 100; loop();
  assert(Serial.writes == 0); // Never stall sampling for a closed/full monitor.
  Serial.capacity = 64;
  clockMs = 120; loop();
  assert(Serial.writes > 0);
  std::cout << "Firmware adapter passed with fake USB/ADC\n";
}
