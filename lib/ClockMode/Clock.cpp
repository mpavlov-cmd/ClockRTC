#include "Clock.h"
#include <LcdUtils.h>
#include <DS1307M.h>

Clock::Clock(LiquidCrystal_74HC595 &liqudCristal,DateTimeRtc &dateTime,unsigned int refreshInterval) 
     : ClockMode(liqudCristal, dateTime, refreshInterval) {}

void Clock::onRefresh(unsigned long mills)
{
     if (!shouldRefresh(mills) && !firstRun)
     {
          return;
     }

     // Get time from clock and set to dt object
     dt.setValue(rtcReadToInt(REG_HOUR), 0);
     dt.setValue(rtcReadToInt(REG_MIN), 1);
     dt.setValue(rtcReadToInt(REG_SEC), 2);
     dt.setValue(rtcReadToInt(REG_DAY), 3);
     dt.setValue(rtcReadToInt(REG_MONTH), 4);
     dt.setValue(rtcReadToInt(REG_YEAR), 5);

     if (firstRun)
     {
          uint8_t timeOffset = showSeconds ? LCD_OFFSET : LCD_OFFSET + 3;
          printClockHud(lcd, timeOffset, LCD_OFFSET, showSeconds);
     }

     // Define the parameter variable
     const uint8_t(*lcdMap)[2];
     if (showSeconds)
     {
          lcdMap = LCD_DT_MAP;
     }
     else
     {
          lcdMap = LCD_DT_MAP_NS;
     }

     printTimeDate(lcd, dt, lcdMap, LCD_OFFSET);
   
     // Indicate first run ended 
     firstRun = false;
}

void Clock::onModeEnter()
{
     firstRun = true;
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

void Clock::setShowSeconds(boolean value)
{
    showSeconds = value;
}
