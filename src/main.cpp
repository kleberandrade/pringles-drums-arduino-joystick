#include <Arduino.h>
#include "board_config.hpp"
#include "drums.hpp"

#ifndef DRUMS_ENABLE_HID
#define DRUMS_ENABLE_HID 0
#endif
#if DRUMS_ENABLE_HID
#include <Keyboard.h>
#endif

namespace {
drums::PadSensor<> pads[board::PadCount];
drums::KeyPulse pulses[board::PadCount];
uint16_t raw[board::PadCount] = {};
uint32_t sampledAt = 0;
uint32_t reportedAt = 0;

void sendKey(drums::KeyEvent event, char key) {
#if DRUMS_ENABLE_HID
  if (event == drums::KeyEvent::Press) Keyboard.press(key);
  if (event == drums::KeyEvent::Release) Keyboard.release(key);
#else
  (void)event;
  (void)key;
#endif
}

void report(uint32_t now) {
  // A line is < 40 bytes. Skip instead of blocking on a full/disconnected USB CDC.
  if (uint32_t(now - reportedAt) < board::ReportMs) return;
  if (!Serial || Serial.availableForWrite() < 40) return;
  reportedAt = now;
  for (size_t i = 0; i < board::PadCount; ++i) {
    Serial.print(raw[i]); Serial.print(' ');
    Serial.print(pads[i].level()); Serial.print(' ');
    Serial.print(pads[i].active() ? 60 : 0);
    if (i + 1 < board::PadCount) Serial.print(' ');
  }
  Serial.println();
}
} // namespace

void setup() {
  for (size_t i = 0; i < board::PadCount; ++i) pinMode(board::PadPins[i], INPUT);
  pinMode(board::ArmPin, INPUT_PULLUP);
  Serial.begin(115200);
#if DRUMS_ENABLE_HID
  Keyboard.begin();
#endif
}

void loop() {
  const uint32_t now = millis();
  const bool sampleDue = uint32_t(now - sampledAt) >= board::SampleMs;
  if (sampleDue) sampledAt = now;
  const bool enabled = DRUMS_ENABLE_HID && digitalRead(board::ArmPin) == LOW;
  for (size_t i = 0; i < board::PadCount; ++i) {
    bool hit = false;
    if (sampleDue) {
      raw[i] = analogRead(board::PadPins[i]);
      hit = pads[i].update(raw[i], now);
    }
    sendKey(pulses[i].update(hit, now, enabled), board::Keys[i]);
  }
  report(now);
}
