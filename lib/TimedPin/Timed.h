#ifndef TIMED_H
#define TIMED_H

#pragma once

#include <TimedPin.h>

struct Timed : public TimedPin
{

protected:
    void actionUp() override;
    void actionDown() override;

public:
    Timed(const uint8_t &buzzerPin, boolean isEnabled);
};

#endif
