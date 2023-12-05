#pragma once

#include <RTClib.h>

class RealTimeClock {
    private:
        RTC_DS3231  _rtc;
        RTC_Millis* _softRTC;

    public:

        RealTimeClock() {}

        bool        init(void);
        DateTime    now(void);
        float       getTemperatureF(void);
        void        attachSQWInterrupt(uint8_t pin, void (*callback)(void), int mode);
};

extern RealTimeClock gRealTimeClock;