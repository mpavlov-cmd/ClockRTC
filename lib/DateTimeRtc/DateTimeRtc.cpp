#include "DateTimeRtc.h"

DateTimeRtc::DateTimeRtc(uint8_t hr, uint8_t mt, uint8_t sc,  uint8_t d, uint8_t m, uint8_t y) {
    year    = y;
    month   = m;
    day     = d;
    hours   = hr;
    minutes = mt;
    seconds = sc;

    changeMask = FULL_MASK;
}

DateTimeRtc::DateTimeRtc() {
    hours   = 12;
    minutes = 00;
    seconds = 00;
    day     = 01;
    month   = 01;
    year    = 20;

    changeMask = FULL_MASK;
}

void DateTimeRtc::setValue(uint8_t value, uint8_t index)
{
    uint8_t existingValue = byIndex(index);
    if (existingValue != value) {
        // Mark the value as changed
        byIndex(index) = value;
        bitSet(changeMask, index);
    }
}

uint8_t DateTimeRtc::getMask()
{
    return changeMask;
}

void DateTimeRtc::flushMask()
{
    changeMask = ZERO_MASK;
}

void DateTimeRtc::asTimeString(char *message) {
    sprintf(message, "%02d:%02d:%02d", hours, minutes, seconds);
}

void DateTimeRtc::asDateString(char *message) {
    sprintf(message, "%02d-%02d-%02d", day, month, year);
}

uint8_t& DateTimeRtc::byIndex(uint8_t idx)
{
    switch (idx)
    {
    case 0:
        return hours;
    case 1:
        return minutes;
    case 2:
        return seconds;
    case 3:
        return day;
    case 4:
        return month;
    case 5:
        return year;
    default:
        return seconds;
    }
}
