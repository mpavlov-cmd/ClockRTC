#ifndef TIMEDPIN_H
#define TIMEDPIN_H

#pragma once

#include <Arduino.h>

struct TimedPin
{

protected:
    const uint8_t pin;
    boolean isEnabled = false;

    // Timing control
    unsigned long ellapsed = 0;
    unsigned long beepUntil = 0;

    virtual void actionUp() = 0;
    virtual void actionDown() = 0;

public:
    const uint8_t NOOP_PIN = 255;

    TimedPin(const uint8_t &buzzerPin, boolean isEnabled);
    void enanled(boolean enanled);

    void update(unsigned long mills);
    void up(unsigned int durationMills);
    void mute();
};

#endif