#include "Timed.h"

Timed::Timed(const uint8_t &buzzerPin, LiquidCrystal_I2C &lcd, boolean isEnabled) : TimedPin(buzzerPin, isEnabled), lcd(lcd)
{
}

void Timed::actionUp()
{
    lcd.backlight();
}

void Timed::actionDown()
{
    // lcd.noBacklight();
}
