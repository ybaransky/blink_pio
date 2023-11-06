#include "led.h"
Led::Led(uint8_t pin) : 
_pin(pin),
_mini_blinks(0),
_last_blink_time(0),
_max_blink_interval(1000),
_mini_blink_interval(50)
{}
 
void Led::init(void) { 
  pinMode(_pin, OUTPUT);
  Serial.printf("led pin: %d\n",_pin);
}

void Led::on(void)  { digitalWrite(_pin, LOW);}
void Led::off(void) { digitalWrite(_pin, HIGH);}
void Led::flip(void) { digitalWrite(_pin, !digitalRead(_pin));}
int  Led::state(void) { return digitalRead(_pin);}
bool Led::is_on(void) { return (LOW==digitalRead(_pin));}

void Led::blink(uint n) {
  // only blink if the led is actually off
  if (is_on()) return;

  // only blink if its long enough since last blink
  if ((millis() - _last_blink_time) < _max_blink_interval) return;

  // blink mini_blink times
  _mini_blinks = _mini_blinks % n;
  _mini_blinks++;
  for (uint i=0; i < _mini_blinks; i++) {
    on();  delay(_mini_blink_interval);  // need to get rid of this
    off(); delay(_mini_blink_interval);
  } 
  _last_blink_time = millis();
}
