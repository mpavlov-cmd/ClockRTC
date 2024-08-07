
#include "ClockConf.h"
#include <LcdUtils.h>
#include <DS1307M.h>
#include <Icons.h>


ClockConf::ClockConf(LiquidCrystal_I2C &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval)
    : EditableClockMode(liqudCristal, dateTime, refreshInterval)
{}

void ClockConf::toRtc()
{
    rtcWrite(REG_HOUR, intToBcd(dt.byIndex(0)));
    rtcWrite(REG_MIN, intToBcd(dt.byIndex(1)));
    rtcWrite(REG_SEC, intToBcd(dt.byIndex(2)));
    rtcWrite(REG_DAY, intToBcd(dt.byIndex(3)));
    rtcWrite(REG_MONTH, intToBcd(dt.byIndex(4)));
    rtcWrite(REG_YEAR, intToBcd(dt.byIndex(5)));
}

void ClockConf::onRefresh(const unsigned long& mills)
{
    // Do noting if time not ellapsed
    if (!shouldRefresh(mills))
    {
        return;
    }

    if (firstRun) {
        printClockHud(lcd, LCD_OFFSET, true);
    }
    
    boolean printed = printTimeDate(lcd, dt, LCD_DT_MAP, LCD_OFFSET);
    if (printed) {
        positionCursor();
    }

    // Indicate first run ended 
    firstRun = false;
}

void ClockConf::onModeEnter()
{
    // Unset edit mode on mode enter
    editMode = false;
    firstRun = true;

    lcd.clear();
    drawArrowIcons(lcd);

    dt.forseMask();
}

void ClockConf::onModeBtnClicked(uint8_t &mode)
{
    // Next mode if not editing
    if (!editMode) {
        mode++;
        return;
    }

    confIdx++;
    // Set cursor to the new position
    positionCursor();

    if (confIdx == 6) {positionCursor();
        confIdx = 0;
        mode++;
    }
}

void ClockConf::onModeBtnHeld(uint8_t &mode)
{
    // Do nothing if not editing
    if (!editMode)
    {
        return;
    }

    // Write time from dt to RTC
    toRtc();

    confIdx = 0;

    // Back to clock
    mode = 0;
}

void ClockConf::onUpBtnClicked()
{
    defaultUpDownButtonHandler(1);
}

void ClockConf::onDownBtnClicked()
{
    defaultUpDownButtonHandler(-1);
}

void ClockConf::positionCursor()
{
    lcd.setCursor(LCD_DT_MAP[confIdx][0] + LCD_OFFSET + 1, LCD_DT_MAP[confIdx][1]);        
}