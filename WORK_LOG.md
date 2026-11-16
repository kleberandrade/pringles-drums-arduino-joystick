# Modernization log

- Inspected a clean `master` at a82236e; all historical commits identify Kléber de Oliveira Andrade. Kept the existing MIT license unchanged.
- Confirmed actual transport is Arduino Keyboard (`w`/`d`) on A2/A3, not MIDI or joystick reports. The historical drawing shows a Pro Mini, incompatible with native USB Keyboard; the supported build will explicitly target a 5V/16MHz ATmega32U4 Pro Micro.
- Read ub-mode and test-master. Added sensor behavior tests before the independent core; observed missing-header failure, then passing assertions. Added pulse tests before the pulse implementation; observed missing-type errors, then passing assertions. These are compile-red TDD steps, not regression failures demonstrated against the old firmware.
- Replaced the dynamic queue with a bounded fixed-storage average; preserved the 25-sample window, >10 attack, <1 release, 40-count clipping and A2/A3 mapping. Added 30ms retrigger protection and 20ms nonblocking key pulses. Tests exercise startup, exact boundaries, repeated hits, clipping, independent pulses and millis rollover under AddressSanitizer/UndefinedBehaviorSanitizer.
- Checked `crapper`, `dryer` and `mutator` help. They are installed but do not support C++/Arduino; no CRAP, duplicate or mutation score is claimed.
- No physical board, upload or real USB HID event is used during validation.
