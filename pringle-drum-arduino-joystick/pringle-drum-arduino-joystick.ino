#include <Keyboard.h>
#include "pad.h"

#define PIN_UP        A2
#define PIN_DOWN      A3

Pad upPad(PIN_UP, 25, 10);
Pad downPad(PIN_DOWN, 25, 10);

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {
  // Update piezo
  upPad.update();
  downPad.update();

  // Debug plotter serial
  upPad.print(false);
  downPad.print(true);

  // Key pressed
  if (upPad.press()   ) Keyboard.press('w');
  if (downPad.press() ) Keyboard.press('d');

  // Release keys
  Keyboard.releaseAll();
}
