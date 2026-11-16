# Physical validation checklist

**Status: PENDING — no checklist item has been executed or approved.**
Repository modernization is complete; this checklist tracks the separate bench
validation required before declaring the assembled hardware validated.

Work through the stages in order. Keep an item unchecked until its result and
supporting evidence have been recorded. Resolve failures and repeat affected
checks after changes. The repetition counts and session duration below are
suggested acceptance tests, not manufacturer specifications.

## Test record and equipment

- Date and operator:
- Firmware commit and any local configuration changes:
- Board manufacturer, revision, MCU marking and oscillator frequency:
- Computer, operating system and USB connection/cable:
- Game name, version and input bindings:
- Instrument models, probe settings and measurement uncertainty:
- Evidence location (photos, scope captures, serial logs and event logs):
- Result per stage, failures, corrective actions and retest results:

Prepare a multimeter, an oscilloscope with suitable probes, a USB data cable and
the [wiring diagram](wiring.svg). Read the [hardware guide](hardware.md) and
[calibration guide](calibration.md) before starting. Disconnect USB while wiring.
Connect oscilloscope ground clips only to circuit GND; use a suitable differential
measurement when measuring across components, rather than grounding a signal.

## 1. Pro Micro and assembly

- [ ] Confirm an **ATmega32U4 Pro Micro, 5V/16MHz**, matching the PlatformIO
  `sparkfun_promicro16` target. An ATmega328P Pro Mini cannot run this native USB
  Keyboard firmware; the 3.3V/8MHz variant is outside the validated build target.
- [ ] With power disconnected, inspect solder joints, insulation, shorts, exposed
  conductors, mechanical stability and strain relief on all sensor leads.
- [ ] Verify pad 1 connects to **A2**, pad 2 to **A3**, and both share board GND.
  Confirm the normally open arm switch connects **D4 to GND** when closed.
- [ ] Power through USB and measure VCC: stable, near 5V and within the operating
  specifications of the actual board and MCU. Record the measured voltage.
- [ ] Confirm USB serial recognition and successfully upload the
  `promicro-calibration` environment to the selected board port, with D4 open.

## 2. Piezo input protection

- [ ] With power disconnected, verify each channel has a **10kΩ bleed resistor
  across the piezo** and a separate **10kΩ series resistor** before the ADC node.
- [ ] Verify the upper Schottky clamp has its **anode at the ADC node and cathode
  at VCC**. Verify the lower clamp has its **anode at GND and cathode at the ADC
  node**. Use the exact component/package datasheet to identify its pins.
- [ ] Confirm the actual diode part numbers and ratings; single BAT54 and
  BAT54A/C/S dual configurations are not interchangeable without checking their
  connections. Check VCC/GND decoupling and electrolytic capacitor polarity.
- [ ] With the board powered, measure **A2 relative to GND**, **A3 relative to
  GND**, and **VCC relative to GND** with an oscilloscope. Start with light taps
  and increase gradually to the hardest intended playing strikes.
- [ ] Record positive/negative ADC-node peaks, VCC rise or droop, and estimated
  series-resistor/clamp current. Check resistor pulse dissipation, diode ratings
  and supply behavior for the measured conditions.
- [ ] Stop and correct the circuit if there is excessive voltage, heating,
  unexpected rail rise, resets or USB disconnects. Verify the normal ADC range
  and transient margins against the component datasheets before approving.

The ATmega32U4 absolute pin stress limits are **−0.5V to VCC + 0.5V**, except for
RESET/VBUS. They are not operating targets, and staying inside those limits alone
does not establish safe operation. Normal ADC conversion uses the GND-to-VCC
range. See the [MCU datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)
and [BAT54 datasheet](https://assets.nexperia.com/documents/data-sheet/BAT54.pdf).

**Do not strike piezos connected to an unpowered board:** rail clamps can
back-power it. Software clipping and serial ADC readings do not verify electrical
protection. If protection or supply behavior is uncertain, leave this stage
pending and resolve it before proceeding to playing tests.

## 3. Pad calibration and response

- [ ] In `promicro-calibration`, open the selected serial port at **115200 baud**.
  Identify the six fields: `pad1_raw pad1_filtered pad1_gate pad2_raw
  pad2_filtered pad2_gate`. Gate is 60 while active and 0 otherwise, not a hit
  counter; serial reports may miss short transients.
- [ ] Observe **60 seconds at rest**. Record the noise floor, confirm no
  spontaneous activations and verify each gate returns to zero after a tap.
- [ ] For each pad, perform **20 light, 20 medium and 20 firm strikes**. Record
  missed and duplicate activations; suggested functional acceptance is one
  activation per intended strike throughout the chosen playing range.
- [ ] Strike one pad at a time and verify the other channel does not activate.
  Correct mechanical crosstalk or wiring problems before relying on thresholds.
- [ ] Test alternating strikes, simultaneous strikes and gradually increasing
  cadence up to that required by the game. Record the fastest reliable cadence
  and any losses or repeated triggers.
- [ ] If necessary, tune attack/release thresholds, clipping, filter window and
  retrigger/hold periods using the calibration guide. Record the final values.
  Shipped defaults are a 25-sample window, attack **>10**, release **<1**, clipping
  at **40**, **30ms** retrigger protection and **20ms** nominal key hold.
- [ ] After tuning, rerun `./scripts/test.sh`, `./scripts/example.sh` and
  `pio run -e promicro-calibration -e promicro-hid`. Update expectations only for
  deliberate default changes, then repeat the affected physical tests.

## 4. USB Keyboard HID and game behavior

- [ ] Confirm calibration firmware emits **no keyboard events**, including when
  D4 is grounded. Return D4 to the open/disarmed state before changing firmware.
- [ ] Upload `promicro-hid` with **D4 open**. Confirm keyboard HID recognition by
  the OS and no pad-generated keyboard events while disarmed.
- [ ] Focus an isolated local keyboard-event viewer, then close D4 to GND.
  Confirm **A2 produces `w`** and **A3 produces `d`**, with both key-down and
  key-up events. Check the measured pulse duration against the nominal 20ms;
  actual timing can include USB/host delays.
- [ ] Verify simultaneous pads generate both keys and neither key remains held
  after the pulses finish. Check repeated and alternating strikes too.
- [ ] Open D4 during a key pulse and verify the key is released and subsequent
  strikes produce no events while disarmed. Re-arming must not replay a hit
  that happened while disarmed.
- [ ] Reconnect USB with D4 open and confirm the board starts disarmed, without
  unexpected input. Select the correct port again if it changes.
- [ ] Focus the target game, verify `w`/`d` bindings, then arm. Check missed hits,
  duplicate actions, simultaneous hits and perceived latency for **at least
  15 minutes** of representative play. Record results and any input settings.
- [ ] Disarm before changing application focus or ending the session. Verify no
  stuck key or unintended action remains after disarming.

## Completion decision

- [ ] All stages passed with measurements and evidence attached to the test
  record; failures were resolved and affected tests repeated.
- [ ] Record the exact tested firmware/configuration, board, protection circuit,
  operating conditions and known limitations, with the operator and date.
- [ ] Only then change **physical validation** from **PENDING** to **COMPLETE**
  in the work log. A successful software CI run does not complete this checklist.

For the build/upload commands and supported tool versions, see the
[root README](../README.md).
