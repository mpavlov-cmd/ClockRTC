#ifndef MILLSTIMER_H
#define MILLSTIMER_H

#pragma once

#include <Arduino.h>

struct MillsTimer
{

protected:
    boolean isEnabled = false;

    // Timing control
    unsigned long ellapsed = 0;
    unsigned long upUntil = 0;

    virtual void actionUp() = 0;
    virtual void actionDown() = 0;

public:

    MillsTimer(boolean isEnabled);
    void enanled(boolean enanled);

    void update(unsigned long mills);
    void up(unsigned int durationMills);
    void mute();
    void onceWithDelay(unsigned int durationMills);
};

#endif