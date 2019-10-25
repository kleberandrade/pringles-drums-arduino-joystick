#ifndef PAD_H
#define PAD_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "movingaverage.h"

class Pad {
  private:
    int _piezoPin;
    int _threshold;
    MovingAverage _filter;
    int _value;
    bool _signal;
    bool _lastSignal;
    float _avg;
    int _maxValue;

  public:

    Pad(int piezoPin, int period, int threshold) : _filter(period){
      _threshold = threshold;
      _piezoPin = piezoPin;
      _maxValue = threshold * 4;
      pinMode(piezoPin, INPUT);
    }

    void update();
    bool press();
    void print(bool newLine);
};

void Pad::update(){
  _value = analogRead(_piezoPin);
  if (_value > _maxValue)
    _value = _maxValue;
  
  _filter.push(_value);

  _avg = _filter.avg();

  _lastSignal = _signal;

  if (!_signal && _avg > _threshold){
    _signal = true;
  }

  if (_signal && _avg < _threshold * 0.1f){
    _signal = false;
  }
}

bool Pad::press(){
  return _signal && !_lastSignal;
}

void Pad::print(bool newLine){
  Serial.print(_value);
  Serial.print(" ");
  Serial.print(_avg);
  Serial.print(" ");
  Serial.print(press() ? 60 : 0);
  Serial.print(newLine ? "\n" : " ");
}

#endif
