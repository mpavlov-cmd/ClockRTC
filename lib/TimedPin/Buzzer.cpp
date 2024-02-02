#include "Buzzer.h"


Buzzer::Buzzer(const uint8_t &buzzerPin, boolean isEnabled) : TimedPin(buzzerPin, isEnabled)
{
}

void Buzzer::actionUp()
{
    const unsigned int NOTE_B7 = 3951;
    tone(pin, NOTE_B7);
}

void Buzzer::actionDown()
{
    noTone(pin);
}
