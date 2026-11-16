# Third-party notices

The project code and original documentation are covered by the root MIT
[LICENSE](LICENSE), copyright Kléber de Oliveira Andrade. That license does not
replace dependency licenses or grant rights in third-party marks.

- [Arduino Keyboard](https://github.com/arduino-libraries/Keyboard/tree/1.0.7),
  Arduino LLC and Peter Barrett: source headers state LGPL-2.1-or-later; the
  upstream distribution also includes its own LICENSE file. Both remain intact
  in PlatformIO's downloaded dependency. See that release for complete notices.
- [Arduino AVR framework](https://github.com/arduino/ArduinoCore-avr): distributed
  by PlatformIO as `framework-arduino-avr@5.4.0`, with upstream LGPL and individual
  file notices. The build downloads these files without modifying their licenses.
- PlatformIO Core and Atmel AVR platform: Apache-2.0; the packaged AVR GCC toolchain
  has its own GPL/runtime exception and bundled component notices.
- The historical `queue.h` credited Steven de Salas and linked to
  [Arduino-Queue.h](https://github.com/sdesalas/Arduino-Queue.h). That implementation
  is no longer distributed in the current source tree; its original attribution
  remains in Git history. The new fixed-size filter does not copy it.
- Historical prototype/wiring/signal images are preserved from the original
  repository. Their presence is not a license grant for embedded third-party
  logos, product designs or Fritzing artwork. The new `docs/wiring.svg` is original
  project artwork, under the project MIT license.

No dependency source or compiled firmware is vendored. If distributing a compiled
firmware, retain upstream notices and satisfy the applicable library linking and
source/relinking obligations; the root MIT license covers only project-owned work.
Pringles and other names mentioned here belong to their respective owners; this
is an independent hobby project.
