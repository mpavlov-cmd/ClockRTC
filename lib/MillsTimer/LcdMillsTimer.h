#ifndef LCDMILLSTIMER_H
#define LCDMILLSTIMER_H

#pragma once

#include <MillsTimer.h>
#include <LiquidCrystal_I2C.h>

struct LcdMillsTimer : public MillsTimer
{

protected:

    LiquidCrystal_I2C &lcd;

    void actionUp() override;
    void actionDown() override;

public:
    LcdMillsTimer(LiquidCrystal_I2C &lcd, boolean isEnabled);
};

#endif
