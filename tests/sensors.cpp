#include "drums.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>

int main() {
  drums::MovingAverage<3> average;
  assert(average.value() == 0);
  assert(average.push(30) == 30);
  assert(average.push(0) == 15);
  assert(average.push(0) == 10);
  assert(average.push(0) == 0);
  for (int i = 0; i < 100; ++i) assert(average.push(1023) <= 1023);
  assert(average.value() == 1023);

  drums::PadSensor<1> pad;
  assert(!pad.update(0, 0));
  assert(!pad.update(10, 1)); // Strict attack threshold.
  assert(pad.update(11, 2));
  assert(pad.active());
  assert(pad.level() == 11);
  assert(!pad.update(1023, 3));
  assert(pad.level() == 40); // Preserve original spike clipping.
  assert(!pad.update(1, 4)); // Strict release threshold.
  assert(pad.active());
  assert(!pad.update(0, 5));
  assert(!pad.active());
  assert(!pad.update(40, 31)); // 29ms since hit, still locked out.
  assert(pad.update(40, 32)); // Retrigger only after release and 30ms.

  drums::PadSensor<25> filtered;
  assert(!filtered.update(0, 0));
  for (uint32_t t = 1; t <= 8; ++t) assert(!filtered.update(0, t));
  assert(!filtered.update(40, 9));
  assert(!filtered.update(40, 10));
  assert(!filtered.update(40, 11)); // Average exactly 10.
  assert(filtered.update(40, 12));
  for (uint32_t t = 13; t < 100; ++t) assert(!filtered.update(40, t));
  for (uint32_t t = 100; t < 125; ++t) assert(!filtered.update(0, t));
  assert(!filtered.active());
  for (uint32_t t = 125; t < 131; ++t) assert(!filtered.update(40, t));
  assert(filtered.update(40, 131));

  drums::PadSensor<1> rollover;
  assert(rollover.update(40, UINT32_MAX - 10));
  assert(!rollover.update(0, UINT32_MAX - 9));
  assert(!rollover.update(40, 18));
  assert(rollover.update(40, 19));
  std::cout << "Sensor behavior passed\n";
}
