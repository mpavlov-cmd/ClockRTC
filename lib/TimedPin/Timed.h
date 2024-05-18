#ifndef TIMED_H
#define TIMED_H

#pragma once

#include <TimedPin.h>
#include <LiquidCrystal_I2C.h>

struct Timed : public TimedPin
{

protected:

    LiquidCrystal_I2C &lcd;

    void actionUp() override;
    void actionDown() override;

public:
    Timed(const uint8_t &buzzerPin, LiquidCrystal_I2C &lcd, boolean isEnabled);
};

#endif
