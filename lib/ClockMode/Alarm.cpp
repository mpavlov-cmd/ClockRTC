#include "Alarm.h"
#include <LcdUtils.h>

Alarm::Alarm(LiquidCrystal_74HC595 &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval, String name) : 
ClockMode(liqudCristal, dateTime, refreshInterval)
{
    this->name = name;
}

void Alarm::onRefresh(unsigned long mills)
{
    // lcd.setCursor(LCD_OFFSET, 0);
    // lcd.print("ALARM ");
    // lcd.print(name);

    boolean printed = printTimeDate(lcd, dt, LCD_TIME_MAP, LCD_OFFSET);
    if (printed) {
        lcd.setCursor(LCD_TIME_MAP[confIdx][0] + LCD_OFFSET + 1, LCD_TIME_MAP[confIdx][1]);
    }
}

void Alarm::onModeEnter()
{
    lcd.clear();
    lcd.cursor();
    dt.forseMask();
}

void Alarm::onModeBtnClicked(uint8_t &mode)
{

    confIdx++;

    lcd.setCursor(LCD_TIME_MAP[confIdx][0] + LCD_OFFSET + 1, LCD_TIME_MAP[confIdx][1]);

    if (confIdx == 3) {
        confIdx = 0;
        mode++;
    }
}

void Alarm::onModeBtnHeld(uint8_t &mode)
{
}

void Alarm::onUpBtnClicked()
{
}

void Alarm::onDownBtnClicked()
{
}
