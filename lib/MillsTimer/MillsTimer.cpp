#include "MillsTimer.h"

MillsTimer::MillsTimer(boolean isEnabled) : isEnabled(isEnabled)
{

}

void MillsTimer::update(unsigned long mills)
{
    ellapsed = mills;
    if (ellapsed > upUntil)
    {
        actionDown();
        upUntil = 0;
    }
}

void MillsTimer::enanled(boolean enanled)
{
    isEnabled = enanled;
}

void MillsTimer::up(unsigned int durationMills)
{
    if (!isEnabled)
    {
        return;
    }

    upUntil = ellapsed + durationMills;
    actionUp();
}

void MillsTimer::mute()
{
    actionDown();
}

void MillsTimer::onceWithDelay(unsigned int durationMills)
{
    actionUp();
    delay(durationMills);
    actionDown();
}
