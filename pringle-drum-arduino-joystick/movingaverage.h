#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <Arduino.h>
#include "queue.h"

class MovingAverage {

  private:
    Queue<int> _window;
    int _sum;
    int _period;

  public:
    MovingAverage(int period) : _window(period)
    {
      _period = period;
      _sum = 0;
    }

    void push(int value);
    float avg();
};

void MovingAverage::push(int value)
{
  if (_window.count() > _period - 1) {
    _sum -= _window.pop();
  }

  _sum += value;
  _window.push(value);
}

float MovingAverage::avg() {
  if (_window.count() == 0) {
    return 0.0f;
  }
  return _sum / (float)_window.count();
}

#endif
