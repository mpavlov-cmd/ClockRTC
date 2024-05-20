#ifndef EDITABLECLOCKMODE_H
#define EDITABLECLOCKMODE_H

#include <ClockMode.h>

struct EditableClockMode : public ClockMode
{

protected:
    boolean editMode = false;
    uint8_t confIdx = 0;

    boolean switchedToEdit()
    {
        if (editMode)
        {
            return false;
        }

        positionCursor();
        lcd.cursor();
        editMode = true;

        return true;
    }

    void defaultUpDownButtonHandler(const int8_t &step)
    {
        if (switchedToEdit())
        {
            return;
        }

        uint8_t currentValue = dt.byIndex(confIdx);
        dt.setValue(currentValue + step, confIdx);
    }

    virtual void positionCursor() = 0;

public:
    EditableClockMode(LiquidCrystal_I2C &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval);
};

#endif