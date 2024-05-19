#include "BuzzerMillsTimer.h"


BuzzerMillsTimer::BuzzerMillsTimer(const uint8_t &buzzerPin, boolean isEnabled) : MillsTimer(isEnabled), buzzerPin(buzzerPin)
{
    
    pinMode(buzzerPin, OUTPUT);
}

void BuzzerMillsTimer::actionUp()
{
    // For Active Buzzer
    //const unsigned int NOTE_B7 = 3951;
    // tone(pin, NOTE_B7);
    digitalWrite(buzzerPin, HIGH);
}

void BuzzerMillsTimer::actionDown()
{
    // For Active Buzzer
    // noTone(pin);

    digitalWrite(buzzerPin, LOW);
}
