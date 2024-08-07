#include "ClockMode.h"

ClockMode::ClockMode(
    LiquidCrystal_I2C &liqudCristal,
    DateTimeRtc &dateTime,
    unsigned int refreshInterval) : lcd(liqudCristal), dt(dateTime), refInt(refreshInterval)
{
}

boolean ClockMode::shouldRefresh(const unsigned long& mills)
{
    boolean result = mills - timeEllapsed >= refInt;
    if (result) {
        timeEllapsed = mills;
    }

    return result;
}
