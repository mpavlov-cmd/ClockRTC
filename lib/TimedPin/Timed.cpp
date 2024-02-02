#include "Timed.h"

Timed::Timed(const uint8_t &buzzerPin, boolean isEnabled) : TimedPin(buzzerPin, isEnabled)
{
}

void Timed::actionUp()
{
    digitalWrite(pin, HIGH);
}

void Timed::actionDown()
{
    digitalWrite(pin, LOW);
}
