#include "drums.hpp"
#include <cassert>
#include <iostream>
using drums::KeyEvent;
int main() {
  drums::KeyPulse pulse;
  assert(pulse.update(false, 0, true) == KeyEvent::None);
  assert(pulse.update(true, 1, false) == KeyEvent::None);
  assert(pulse.update(true, 2, true) == KeyEvent::Press);
  assert(pulse.update(true, 3, true) == KeyEvent::None);
  assert(pulse.update(false, 21, true) == KeyEvent::None);
  assert(pulse.update(false, 22, true) == KeyEvent::Release);
  assert(pulse.update(false, 23, true) == KeyEvent::None);
  assert(pulse.update(true, 24, true) == KeyEvent::Press);
  assert(pulse.update(false, 25, false) == KeyEvent::Release);
  assert(pulse.update(true, 26, false) == KeyEvent::None);
  assert(pulse.update(true, UINT32_MAX - 9, true) == KeyEvent::Press);
  assert(pulse.update(false, 9, true) == KeyEvent::None);
  assert(pulse.update(false, 10, true) == KeyEvent::Release);
  drums::KeyPulse other;
  assert(other.update(true, 10, true) == KeyEvent::Press);
  assert(pulse.update(false, 10, true) == KeyEvent::None);
  std::cout << "Key pulse behavior passed\n";
}
