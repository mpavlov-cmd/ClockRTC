#include "ClockConf.h"
#include <LcdUtils.h>
#include <DS1307M.h>
#include <Icons.h>

ClockConf::ClockConf(LiquidCrystal_74HC595 &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval)
    : ClockMode(liqudCristal, dateTime, refreshInterval)
{
    // TODO: Needed in different modes, move to common code
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
    editMode    = false;
    firstRun    = true;

    lcd.clear();

    darwIcon(lcd, 0, 0, CHAR_ARROW_UP_IDX);
    darwIcon(lcd, 0, 1, CHAR_ARROW_DOWN_IDX);

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
    if (switchedToEdit())
    {
        return;
    }

    uint8_t currentValue = dt.byIndex(confIdx);
    dt.setValue(currentValue + 1, confIdx);
}

void ClockConf::onDownBtnClicked()
{
    if (switchedToEdit())
    {
        return;
    }

    uint8_t currentValue = dt.byIndex(confIdx);
    dt.setValue(currentValue - 1, confIdx);
}