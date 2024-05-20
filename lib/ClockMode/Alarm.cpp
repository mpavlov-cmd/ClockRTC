#include <Alarm.h>
#include <LcdUtils.h>
#include <Icons.h>

Alarm::Alarm(LiquidCrystal_I2C &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval, uint8_t name) : EditableClockMode(liqudCristal, dateTime, refreshInterval)
{
    this->name = name;
}

void Alarm::onRefresh(const unsigned long &mills)
{
    // Show hood only once
    if (firstRun)
    {
        // Alarm HUD
        drawAlarmHood();
    }

    boolean printed = printTimeDate(lcd, dt, LCD_TIME_MAP, LCD_OFFSET);
    if (printed)
    {
        positionCursor();
    }

    // Indicate first run is over
    firstRun = false;
}

void Alarm::onModeEnter()
{
    firstRun = true;
    editMode = false;

    lcd.clear();
    lcd.noCursor();
    drawArrowIcons(lcd);

    dt.forseMask();
}

void Alarm::onModeBtnClicked(uint8_t &mode)
{
    if (!editMode)
    {
        mode++;
        return;
    }

    confIdx++;
    positionCursor();

    if (confIdx == 3)
    {
        confIdx = 0;
        mode++;
    }
}

void Alarm::onModeBtnHeld(uint8_t &mode)
{
    enabled = !enabled;
    editMode = false;
    confIdx = 0;

    lcd.noCursor();
    drawEnabledFlag();
}

void Alarm::onUpBtnClicked()
{
    defaultUpDownButtonHandler(1);
}

void Alarm::onDownBtnClicked()
{
    defaultUpDownButtonHandler(-1);
}

void Alarm::positionCursor()
{
    lcd.setCursor(LCD_TIME_MAP[confIdx][0] + LCD_OFFSET + 1, LCD_TIME_MAP[confIdx][1]);
}