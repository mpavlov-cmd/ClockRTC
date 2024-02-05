#include "Alarm.h"
#include <LcdUtils.h>


Alarm::Alarm(LiquidCrystal_74HC595 &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval, uint8_t name) : 
ClockMode(liqudCristal, dateTime, refreshInterval)
{
    this->name = name;
}

void Alarm::onRefresh(unsigned long mills)
{
    // Show hood only once
    if (!isHoodSHown) {

        // Alarm HUD
        lcd.setCursor(LCD_OFFSET, 0);
        lcd.print("ALARM");
        lcd.print(name);
        lcd.print(":");
        lcd.setCursor(CURSOR_ON_OFF[0], CURSOR_ON_OFF[1]);
        lcd.print(enabled ? "Y" : "N");

        // TODO: Add column betwwen hours-minutes, minutes-seconds

        // TODO: Fix duplicate code, fix imports
        darwIcon(lcd, 0, 0, 0);
        darwIcon(lcd, 0, 1, 1);

        isHoodSHown = true;
    }

    boolean printed = printTimeDate(lcd, dt, LCD_TIME_MAP, LCD_OFFSET);
    if (printed) {
        positionCursor();
    }

    // TODO: Ring the bell
}

void Alarm::onModeEnter()
{
    isHoodSHown = false;
    editMode = false;

    lcd.clear();
    dt.forseMask();
}

void Alarm::onModeBtnClicked(uint8_t &mode)
{
    if (!editMode) {
        mode++;
        return;
    }

    confIdx++;
    positionCursor();

    if (confIdx == 3) {
        confIdx = 0;
        mode++;
    }
}

void Alarm::onModeBtnHeld(uint8_t &mode)
{
    enabled  = !enabled;
    editMode = false;
    confIdx  = 0;

    lcd.noCursor();

    // TODO: duplicate code
    lcd.setCursor(CURSOR_ON_OFF[0], CURSOR_ON_OFF[1]);
    lcd.print(enabled ? "Y" : "N");
}

// TODO: duplicated code 
void Alarm::onUpBtnClicked()
{
    if (switchedToEdit())
    {
        return;
    }

    uint8_t currentValue = dt.byIndex(confIdx);
    dt.setValue(currentValue + 1, confIdx);
}

// TODO: duplicated code
void Alarm::onDownBtnClicked()
{
    if (switchedToEdit())
    {
        return;
    }

    uint8_t currentValue = dt.byIndex(confIdx);
    dt.setValue(currentValue - 1, confIdx);
}
