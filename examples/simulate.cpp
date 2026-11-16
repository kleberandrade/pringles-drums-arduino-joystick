#include "drums.hpp"
#include <iostream>

// Deterministic ADC trace: no Arduino, USB, serial port or hardware access.
int main() {
  drums::PadSensor<> pad;
  drums::KeyPulse key;
  for (uint32_t time = 0; time < 150; ++time) {
    const uint16_t raw = ((time >= 10 && time < 40) ||
                          (time >= 90 && time < 120)) ? 40 : 0;
    const bool hit = pad.update(raw, time);
    const drums::KeyEvent event = key.update(hit, time, true);
    if (event == drums::KeyEvent::Press) std::cout << time << "ms press w\n";
    if (event == drums::KeyEvent::Release) std::cout << time << "ms release w\n";
  }
}
