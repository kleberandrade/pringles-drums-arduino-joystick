# Pringles Drums Arduino Joystick

Simple joystick sketch for Arduino Pro Micro

<p align="center">
    <img src="https://github.com/kleberandrade/pringle-drum-arduino-joystick/blob/master/images/Prototype.jpg" width="600"/>
</p>

Electrical scheme using 2 piezoelectric 27.5mm, 2 resistors 10K and 1 arduino mini pro.

<p align="center">
    <img src="https://github.com/kleberandrade/pringle-drum-arduino-joystick/blob/master/images/Pingle%20Drums.png" width="600"/>
</p>

Captured piezoelectric signals, filtered signal using moving average, and button-pressed signal to play.

<p align="center">
    <img src="https://github.com/kleberandrade/pringle-drum-arduino-joystick/blob/master/images/Signals.png" width="600"/>
</p>

## How it works

Required libraries

```cpp
#include <Keyboard.h>
#include "pad.h"
```

Declaration of constants

```cpp
#define PIN_UP        A2
#define PIN_DOWN      A3
```

Declaration of variables

```cpp
Pad upPad(PIN_UP, 25, 10);
Pad downPad(PIN_DOWN, 25, 10);
```

Initialization serial port and keyboard

```cpp
void setup() {
  Serial.begin(9600);
  Keyboard.begin();
}
```

Main loop

```cpp
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
```

## About the game

Legend's Beats: The game is very similar to styles like Guitar Hero and Rock Band, but here you have to hit the beat of the song instead of an instrument note, the beats are represented by enemies and when you hit the beat, the character shoot down the enemy, otherwise you get damage.

-   Play game: http://bit.ly/2MnrTRU
-   Survey: http://bit.ly/2Bjvni3
-   Forum: https://riot.com/2NosvX3
-   Article in Versus: http://bit.ly/2JxCmZs

<p align="center">
    <img src="https://cdn.discordapp.com/attachments/330351041394507777/634659071034916864/unknown.png" width="600"/>
</p>

## Development Steps

Part 1 - Test Keyboard library

[![](http://img.youtube.com/vi/3Bw8QX_qXLc/0.jpg)](http://www.youtube.com/watch?v=3Bw8QX_qXLc "")

Part 2 - Test piezoelectric (piezo) sensors

[![](http://img.youtube.com/vi/wedDKFGgjJs/0.jpg)](http://www.youtube.com/watch?v=wedDKFGgjJs "")

Part 3 - Test filter and pringles

[![](http://img.youtube.com/vi/3L8o1plSuC0/0.jpg)](http://www.youtube.com/watch?v=3L8o1plSuC0 "")

## References

-   [Keyboard document](https://www.arduino.cc/reference/en/language/functions/usb/keyboard/)
-   [Queue](https://github.com/sdesalas/Arduino-Queue.h)

## License

    Copyright 2019 Kleber de Oliveira Andrade

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
