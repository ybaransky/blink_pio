#include <arduino.h>
#include "RealTimeClock.h"
#include "Utils.h"

void RealTimeClock::init(void) {
    if (!_rtc.begin()) {
        PL("Couldn't find ds3231 RTC");
        Serial.flush();
    }
}

DateTime RealTimeClock::now(void) {
    return _rtc.now();
}

float RealTimeClock::getTemperatureF(void) {
    return 32.0 + 1.8*(_rtc.getTemperature());
}

void RealTimeClock::attachSQWInterrupt(uint8_t pin, void (*callback)(void), int mode) {
    _rtc.writeSqwPinMode(DS3231_SquareWave1Hz);
    pinMode(pin,INPUT);
    digitalWrite(pin,HIGH);
    attachInterrupt(digitalPinToInterrupt(pin), callback, mode);
}
