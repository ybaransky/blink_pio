#pragma once

#include <Arduino.h>


class Led
{
private:
  uint8_t _pin;
  uint    _mini_blinks = 0; // we do this many blinks up to max_blinks
  uint    _last_blink_time = 0;
  uint    _max_blink_interval = 1000;
  uint    _mini_blink_interval = 50;

public:
  Led(uint8_t pin=LED_BUILTIN);
  ~Led() {};

  void on(void);
  void off(void);
  void init(void);
  void flip(void);
  int  state(void);
  bool is_on(void);
  void blink(uint);
};
