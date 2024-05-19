#ifndef BUZZERMILLSTIMER_H
#define BUZZERMILLSTIMER_H

#pragma once

#include <MillsTimer.h>

struct BuzzerMillsTimer : public MillsTimer
{

protected:

    const uint8_t buzzerPin;

    void actionUp() override;
    void actionDown() override;

public:
    BuzzerMillsTimer(const uint8_t &buzzerPin, boolean isEnabled);
};

#endif