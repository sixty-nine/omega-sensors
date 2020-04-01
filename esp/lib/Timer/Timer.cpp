#include <Arduino.h>
#include "Timer.h"

Timer::Timer(unsigned long interval, TimerCallback callback) {
    _interval = interval;
    _callback = callback;
}

void Timer::update() {
    unsigned long currentMillis = millis();
    if (currentMillis - _prevMillis >= _interval) {
        _prevMillis = currentMillis;
        _callback();
    }
}

void Timer::setInterval(unsigned long interval) {
    _interval = interval;
}
