#ifndef PRINGLES_DRUMS_HPP
#define PRINGLES_DRUMS_HPP

#include <stddef.h>
#include <stdint.h>

namespace drums {

constexpr uint16_t AttackThreshold = 10;
constexpr uint16_t ReleaseThreshold = 1;
constexpr uint16_t SampleCeiling = 40;
constexpr uint32_t RetriggerMs = 30;
constexpr uint32_t KeyHoldMs = 20;
static_assert(ReleaseThreshold < AttackThreshold && AttackThreshold < SampleCeiling,
              "Require release < attack < clipping ceiling");

enum class KeyEvent { None, Press, Release };

class KeyPulse {
 public:
  KeyEvent update(bool hit, uint32_t now, bool enabled) {
    if (held_ && (!enabled || uint32_t(now - pressedAt_) >= KeyHoldMs)) {
      held_ = false;
      return KeyEvent::Release;
    }
    if (!held_ && enabled && hit) {
      held_ = true;
      pressedAt_ = now;
      return KeyEvent::Press;
    }
    return KeyEvent::None;
  }
 private:
  bool held_ = false;
  uint32_t pressedAt_ = 0;
};


// Fixed storage: no heap allocation and no dependence on Arduino or USB.
template <size_t Window>
class MovingAverage {
  static_assert(Window > 0 && Window <= 64, "Window must be 1..64 samples");
 public:
  float push(uint16_t sample) {
    sum_ -= samples_[next_];
    sum_ += sample;
    samples_[next_] = sample;
    next_ = (next_ + 1) % Window;
    if (count_ < Window) ++count_;
    return value();
  }
  float value() const { return count_ ? float(sum_) / count_ : 0.0f; }
 private:
  uint16_t samples_[Window] = {};
  size_t next_ = 0;
  size_t count_ = 0;
  uint32_t sum_ = 0;
};

template <size_t Window = 25>
class PadSensor {
 public:
  // Samples are 10-bit ADC counts. Time is a monotonically wrapping millis().
  bool update(uint16_t sample, uint32_t now) {
    level_ = filter_.push(sample > SampleCeiling ? SampleCeiling : sample);
    if (level_ < ReleaseThreshold) active_ = false;
    if (active_ || level_ <= AttackThreshold) return false;
    if (hasHit_ && uint32_t(now - lastHit_) < RetriggerMs) return false;
    active_ = true;
    hasHit_ = true;
    lastHit_ = now;
    return true;
  }
  float level() const { return level_; }
  bool active() const { return active_; }
 private:
  MovingAverage<Window> filter_;
  float level_ = 0;
  bool active_ = false;
  bool hasHit_ = false;
  uint32_t lastHit_ = 0;
};

} // namespace drums
#endif
