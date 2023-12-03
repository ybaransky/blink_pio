#include <arduino.h>
#include "RealTimeClock.h"
#include "Utils.h"

bool RealTimeClock::init(void) {
    if (!_rtc.begin()) {
        PL("Couldn't find ds3231 RTC");
        Serial.flush();
    }
    
    bool resettedTime = _rtc.lostPower();
    if (resettedTime) {
        Serial.println("RTC lost power, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        _rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    return resettedTime;
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
