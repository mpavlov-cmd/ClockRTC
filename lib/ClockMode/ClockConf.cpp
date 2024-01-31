#include "ClockConf.h"
#include <LcdUtils.h>
#include <DS1307M.h>
// #include <Icons.h>

ClockConf::ClockConf(LiquidCrystal_74HC595 &liqudCristal,DateTimeRtc &dateTime,unsigned int refreshInterval) 
     : ClockMode(liqudCristal, dateTime, refreshInterval) {}

void ClockConf::onRefresh(unsigned long mills)
{
    // Do noting if time not ellapsed
    if (!shouldRefresh(mills))
    {
        return;
    }

    printClockHud(lcd, isHoodSHown, LCD_OFFSET);
    printTimeDate(lcd, dt, LCD_DT_MAP, LCD_OFFSET);

    lcd.setCursor(LCD_DT_MAP[confIdx][0] + LCD_OFFSET + 1, LCD_DT_MAP[confIdx][1]);
}

void ClockConf::onModeEnter()
{
    isHoodSHown = false;
    lcd.clear();
    lcd.cursor();

    // TODO: Fix icons inport
    darwIcon(lcd, 0, 0, 0);
    darwIcon(lcd, 0, 1, 1);
}

void ClockConf::onModeBtnClicked(uint8_t &mode)
{
    confIdx++;

    if (confIdx == 6) {
        confIdx = 0;
        mode++;
    }
}

void ClockConf::onModeBtnHeld(uint8_t &mode)
{
    // Write time from dt to RTC
    rtcWrite(REG_HOUR, intToBcd(dt.byIndex(0)));
    rtcWrite(REG_MIN, intToBcd(dt.byIndex(1)));
    rtcWrite(REG_SEC, intToBcd(dt.byIndex(2)));
    rtcWrite(REG_DAY, intToBcd(dt.byIndex(3)));
    rtcWrite(REG_MONTH, intToBcd(dt.byIndex(4)));
    rtcWrite(REG_YEAR, intToBcd(dt.byIndex(5)));

    confIdx = 0;
    mode++;
}

void ClockConf::onUpBtnClicked()
{
    uint8_t currentValue = dt.byIndex(confIdx);
    dt.setValue(currentValue + 1, confIdx);
}

void ClockConf::onDownBtnClicked()
{
    uint8_t currentValue = dt.byIndex(confIdx);
    dt.setValue(currentValue - 1, confIdx);
}