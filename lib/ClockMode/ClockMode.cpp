#include "ClockMode.h"
#include <DS1307M.h>

ClockMode::ClockMode(
    LiquidCrystal_74HC595 &liqudCristal,
    DateTimeRtc &dateTime,
    unsigned int refreshInterval) : lcd(liqudCristal), dt(dateTime), refInt(refreshInterval)
{
}

boolean ClockMode::shouldRefresh(unsigned long mills)
{
    boolean result = mills - timeEllapsed >= refInt;
    if (result) {
        timeEllapsed = mills;
    }

    return result;
}

void ClockMode::toRtc()
{
    rtcWrite(REG_HOUR, intToBcd(dt.byIndex(0)));
    rtcWrite(REG_MIN, intToBcd(dt.byIndex(1)));
    rtcWrite(REG_SEC, intToBcd(dt.byIndex(2)));
    rtcWrite(REG_DAY, intToBcd(dt.byIndex(3)));
    rtcWrite(REG_MONTH, intToBcd(dt.byIndex(4)));
    rtcWrite(REG_YEAR, intToBcd(dt.byIndex(5)));
}
