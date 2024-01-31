#include "Clock.h"
#include <LcdUtils.h>

Clock::Clock(LiquidCrystal_74HC595 &liqudCristal,DateTimeRtc &dateTime,unsigned int refreshInterval) 
     : ClockMode(liqudCristal, dateTime, refreshInterval) {}

void Clock::onRefresh(unsigned long mills)
{
     // Do noting if time not ellapsed
     if (mills - timeEllapsed < refInt) {
          return;
     }

     timeEllapsed = mills;
     
     // print once in refInt 
     lcd.setCursor(0, 0);
     lcd.print("Hello");
     lcd.print(mills);
}

void Clock::onModeEnter() const
{
     boolean shownHood = false;
     printClockHud(lcd, shownHood, LCD_OFFSET);
}

void Clock::onModeBtnClicked(uint8_t *mode) const
{
     mode++;
}

void Clock::onModeBtnHeld(uint8_t *mode) const
{
}

void Clock::onUpBtnClicked() const
{
}

void Clock::onDownBtnClicked() const
{
}
