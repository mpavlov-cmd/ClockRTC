#include "LcdMillsTimer.h"

LcdMillsTimer::LcdMillsTimer(LiquidCrystal_I2C &lcd, boolean isEnabled) : MillsTimer(isEnabled), lcd(lcd)
{
}

void LcdMillsTimer::actionUp()
{
    lcd.backlight();
}

void LcdMillsTimer::actionDown()
{
    // lcd.noBacklight();
}
