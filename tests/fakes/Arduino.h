#pragma once
#include <cstdint>
#include <cstddef>
constexpr uint8_t A2 = 20, A3 = 21;
constexpr uint8_t INPUT = 0, INPUT_PULLUP = 2, LOW = 0, HIGH = 1;
extern uint32_t clockMs;
extern int adc[2];
extern int armLevel;
extern int pinModes[32];
inline uint32_t millis() { return clockMs; }
inline void pinMode(uint8_t pin, uint8_t mode) { pinModes[pin] = mode; }
inline int analogRead(uint8_t pin) { return adc[pin - A2]; }
inline int digitalRead(uint8_t pin) { return pin == 4 ? armLevel : HIGH; }
struct SerialFake {
  bool connected = false;
  int capacity = 0;
  unsigned writes = 0;
  unsigned baud = 0;
  void begin(unsigned value) { baud = value; }
  explicit operator bool() const { return connected; }
  int availableForWrite() const { return capacity; }
  template <typename T> void print(T) { ++writes; }
  void println() { ++writes; }
  template <typename T> void println(T) { ++writes; }
};
extern SerialFake Serial;
