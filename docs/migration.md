# Migrating the original sketch

The historical entry point was
`pringle-drum-arduino-joystick/pringle-drum-arduino-joystick.ino`. It is replaced
by PlatformIO's `src/main.cpp`; portable headers live in `include/`. Do not keep
both sketches in a project or copy obsolete queue/pad headers into the build.
Use the commands in the root README from the repository root. Arduino IDE's
“open .ino” workflow is no longer maintained.

Preserved defaults: two passive piezos, A2 → `w`, A3 → `d`, 25-sample moving
average, 40-count clipping, attack >10 and release <1. MIT attribution and the
historical images remain. The new ring buffer uses fixed storage and explicit
initial state rather than heap allocation and Arduino-dependent constructors.

Intentional behavior changes:

- The supported target is explicitly Pro Micro ATmega32U4 5V/16MHz. The old Pro
  Mini breadboard drawing is historical and cannot provide native Keyboard HID.
- The default firmware is serial calibration; choose `promicro-hid` and add the
  normally open D4-to-GND arm switch to play. D4 open means disarmed.
- Sampling now has a nominal 1ms interval instead of being paced by serial
  printing. Telemetry is 115200 baud, at most 50 lines/second instead of 9600
  every loop. Recalibrate; identical numeric thresholds do not ensure the same
  physical response under different sampling timing.
- Each key is held for 20ms then individually released, rather than calling
  `Keyboard.releaseAll()` immediately after a press. This makes pulses visible
  to polling apps and preserves independent simultaneous pads.
- A pad must release and wait at least 30ms from its last accepted hit to trigger
  again. This suppresses ringing but caps intentional repeated hits too.
- External clamp diodes and series resistors are added to the documented circuit;
  the original bleed resistors alone do not qualify the input protection.
- Removed unused SoftwareSerial and third-party queue implementation. This does
  not change the licenses of Arduino dependencies; see THIRD_PARTY.md.

The internal `Pad(pin, period, threshold)`, `press()` and `print()` API is removed.
Use `drums::PadSensor<Window>::update(raw, millis)` for detection and
`drums::KeyPulse::update(hit, millis, enabled)` for key state. Hardware reads and
USB calls belong in the adapter, so the logic can be tested without a board.
