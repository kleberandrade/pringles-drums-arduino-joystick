# Calibration and troubleshooting

## Calibration without typing

Build/upload `promicro-calibration` first, following the README. It never calls
Keyboard, regardless of D4. Open the selected board port at 115200 baud with
`pio device monitor -e promicro-calibration --port YOUR_BOARD_PORT`, or a serial
plotter. A line contains six space-separated numbers, in this order:

```text
pad1_raw pad1_filtered pad1_gate pad2_raw pad2_filtered pad2_gate
```

Raw is the unmodified 0–1023 ADC reading. Filtered is the average after clipping
each sample at `SampleCeiling` (40). Gate is 60 while the pad is active, otherwise
0; it is **not** a count of hits or a one-sample pulse. At 50 reports per second,
plots can miss short transients. Use an oscilloscope for electrical peaks.
The monitor is optional: sampling does not wait for a serial connection.

1. With pads untouched, record noise and check both channels return near zero.
2. Tap one pad gently, then firmly. Observe raw/filtered peaks and whether the
   gate returns to zero. Repeat for the other channel.
3. Edit the named constants at the top of `include/drums.hpp` if needed:
   `AttackThreshold = 10`, `ReleaseThreshold = 1`, `SampleCeiling = 40`,
   `RetriggerMs = 30`, `KeyHoldMs = 20`. All thresholds use ADC counts, not volts.
   Require release < attack < ceiling. The attack comparison is strictly `>`;
   release is strictly `<`. With 5V ADC reference, one ADC count is roughly 4.9mV.
4. Choose an attack above resting noise and below filtered soft hits. Choose a
   positive release above the resting filtered baseline so the pad can rearm.
   Raise retrigger time if ringing produces repeated hits; shorten it only if
   intentional fast hits are lost after full release.
5. The filter window is the `PadSensor<>` template default, 25 samples in
   `include/drums.hpp`. At nominal 1ms sampling a full window spans about 25ms.
   Startup uses only the samples collected so far. Changing window size, sample
   period, bleed resistance or mounting changes sensitivity and latency.
6. Rerun host checks and both builds after tuning. Numeric boundary/trace tests
   intentionally lock the shipped defaults; update their expectations if you
   deliberately change those defaults.
7. With D4 open, upload `promicro-hid`. Focus the intended app before grounding
   D4. Only **new** detected hits while armed produce keys; existing sensor gates
   are not replayed when arming. Open the switch to disarm immediately.

The D4 arm switch is a level gate, not a debounced input. Contact bounce can
shorten a key hold by disarming it, but rearming does not synthesize a hit.
Keyboard pulses use elapsed time without delay(); USB library calls may still
block or delay on host conditions. No hard realtime guarantee is made.

## Troubleshooting

- **Compilation says Keyboard/HID is unavailable:** check ATmega32U4 and the
  `sparkfun_promicro16` target. An ATmega328P Pro Mini is incompatible.
- **Board is missing/upload fails:** use a data cable, select the correct port,
  close serial monitors, check OS serial permissions and follow your board's
  bootloader reset procedure. Do not change fuses or bootloader blindly.
- **No keys:** confirm the HID build, D4-to-GND switch, focused app, expected
  `w`/`d` bindings and USB keyboard enumeration. Confirm hits in calibration first.
- **Only the first hit works:** the filtered value must fall strictly below the
  release threshold. Look for floating inputs, a missing bleed/GND or too much
  mechanical ringing. Check the plotted gate before changing lockout time.
- **Repeated/cross-triggered hits:** separate mounts, secure leads, verify common
  GND and clamps, then adjust threshold/retrigger. No software crosstalk rejection
  is implemented; hitting both pads simultaneously is valid.
- **Weak/missing soft hits:** check disc polarity/mounting, bleed value and raw
  readings; clipping and the moving average can suppress very short spikes.
- **ADC stays at 1023 or resets occur on strikes:** stop playing and inspect the
  electrical protection and rail voltage. Software cannot fix overvoltage.
- **Unexpected text in another app:** open D4 immediately. Calibration mode is
  the default for setup. HID events always go to the app with keyboard focus.

Physical response, protection, USB recognition and game compatibility remain
bench tests for the builder. Host tests and successful cross-compilation do not
substitute for them.
