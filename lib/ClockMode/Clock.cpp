#include "Clock.h"
#include <LcdUtils.h>
#include <DS1307M.h>

Clock::Clock(LiquidCrystal_74HC595 &liqudCristal,DateTimeRtc &dateTime,unsigned int refreshInterval) 
     : ClockMode(liqudCristal, dateTime, refreshInterval) {}

void Clock::onRefresh(unsigned long mills)
{
     if (!shouldRefresh(mills)) {
          return;
     }

     // Get time from clock and set to dt object
     dt.setValue(rtcReadToInt(REG_HOUR), 0);
     dt.setValue(rtcReadToInt(REG_MIN), 1);
     dt.setValue(rtcReadToInt(REG_SEC), 2);
     dt.setValue(rtcReadToInt(REG_DAY), 3);
     dt.setValue(rtcReadToInt(REG_MONTH), 4);
     dt.setValue(rtcReadToInt(REG_YEAR), 5);

     printClockHud(lcd, isHoodSHown, LCD_OFFSET);
     printTimeDate(lcd, dt, LCD_DT_MAP, LCD_OFFSET);
}

void Clock::onModeEnter()
{
     isHoodSHown = false;
     lcd.clear();
     lcd.noCursor();
     dt.forseMask();
}

void Clock::onModeBtnClicked(uint8_t &mode)
{
     mode++;
}

void Clock::onModeBtnHeld(uint8_t &mode)
{
}

void Clock::onUpBtnClicked()
{
}

void Clock::onDownBtnClicked()
{
}
