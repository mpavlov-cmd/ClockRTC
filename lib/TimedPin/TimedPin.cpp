#include "TimedPin.h"

TimedPin::TimedPin(const uint8_t &buzzerPin, boolean isEnabled) : pin(buzzerPin), isEnabled(isEnabled)
{
    if (pin != NOOP_PIN)
    {
        pinMode(pin, OUTPUT);
    }
}

void TimedPin::update(unsigned long mills)
{
    ellapsed = mills;
    if (ellapsed > beepUntil)
    {
        actionDown();
        beepUntil = 0;
    }
}

void TimedPin::enanled(boolean enanled)
{
    isEnabled = enanled;
}

void TimedPin::up(unsigned int durationMills)
{
    if (!isEnabled)
    {
        return;
    }

    beepUntil = ellapsed + durationMills;
    actionUp();
}

void TimedPin::mute()
{
    actionDown();
}
