#include <Alarm.h>
#include <LcdUtils.h>
#include <Icons.h>


Alarm::Alarm(LiquidCrystal_I2C &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval, uint8_t name) : 
ClockMode(liqudCristal, dateTime, refreshInterval)
{
    this->name = name;
}

void Alarm::onRefresh(const unsigned long& mills)
{
    // Show hood only once
    if (firstRun) {
        // Alarm HUD
        drawAlarmHood();
    }

    boolean printed = printTimeDate(lcd, dt, LCD_TIME_MAP, LCD_OFFSET);
    if (printed) {
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
    drawEnabledFlag();
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
