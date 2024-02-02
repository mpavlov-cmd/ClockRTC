#ifndef BUZZER_H
#define BUZZER_H

#pragma once

#include <TimedPin.h>

struct Buzzer : public TimedPin
{

protected:
    void actionUp() override;
    void actionDown() override;

public:
    Buzzer(const uint8_t &buzzerPin, boolean isEnabled);
};

#endif