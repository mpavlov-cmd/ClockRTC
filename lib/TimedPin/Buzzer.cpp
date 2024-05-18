#include "Buzzer.h"


Buzzer::Buzzer(const uint8_t &buzzerPin, boolean isEnabled) : TimedPin(buzzerPin, isEnabled)
{
}

void Buzzer::actionUp()
{
    const unsigned int NOTE_B7 = 3951;
    // For Active Buzzer
    // tone(pin, NOTE_B7);

    digitalWrite(pin, HIGH);
}

void Buzzer::actionDown()
{
    // For Active Buzzer
    // noTone(pin);

    digitalWrite(pin, LOW);
}
