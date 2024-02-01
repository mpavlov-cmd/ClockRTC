#include "ClockConf.h"
#include <LcdUtils.h>
#include <DS1307M.h>
#include <Icons.h>

ClockConf::ClockConf(LiquidCrystal_74HC595 &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval)
    : ClockMode(liqudCristal, dateTime, refreshInterval)
{
    // Setup custom chars
    lcd.createChar(CHAR_ARROW_UP_IDX, CHAR_ARROW_UP);
    lcd.createChar(CHAR_ARROW_DOWN_IDX, CHAR_ARROW_DOWN);

    // Write initial time to the IC
    toRtc();
}

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

    darwIcon(lcd, 0, 0, CHAR_ARROW_UP_IDX);
    darwIcon(lcd, 0, 1, CHAR_ARROW_DOWN_IDX);

    dt.forseMask();
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
    toRtc();

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