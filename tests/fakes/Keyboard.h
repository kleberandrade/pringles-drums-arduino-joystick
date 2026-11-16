#pragma once
#include <vector>
struct KeyboardFake {
  bool started = false;
  struct Event { char key; bool pressed; };
  std::vector<Event> events;
  void begin() { started = true; }
  void press(char key) { events.push_back({key, true}); }
  void release(char key) { events.push_back({key, false}); }
};
extern KeyboardFake Keyboard;
