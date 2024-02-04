#include "LcdUtils.h"

boolean printTimeDate(LiquidCrystal_74HC595 &lcd, DateTimeRtc &dt, const uint8_t mappings[6][2], uint8_t offset)
{
    uint8_t mask = dt.getMask();
    boolean printed = false;

    // printBcd(mask);
    for (int i = 0; i < 6; i++)
    {
        uint8_t col = mappings[i][0];
        uint8_t row = mappings[i][1];

        // Skip position to allow different mappings
        if (col == UINT8_MAX || row == UINT8_MAX) {
            continue;
        }

        uint8_t changed = bitRead(mask, i);
        // If no changes continue
        if (changed == 0)
        {
            continue;
        }
        // Changed
        printed = true;
        char printVal[3];
        sprintf(printVal, "%02d", dt.byIndex(i));

        lcd.setCursor(col + offset, row);
        lcd.print(printVal);
    }

    dt.flushMask();

    return printed;
}

void printClockHud(LiquidCrystal_74HC595 &lcd, boolean &shownHud, uint8_t offset)
{
    if (shownHud)
    {
        return;
    }

    lcd.setCursor(2 + offset, 0);
    lcd.print(":");
    lcd.setCursor(5 + offset, 0);
    lcd.print(":");

    lcd.setCursor(2 + offset, 1);
    lcd.print("-");
    lcd.setCursor(5 + offset, 1);
    lcd.print("-");

    shownHud = true;
}

void darwIcon(LiquidCrystal_74HC595 &lcd, uint8_t col, uint8_t row, uint8_t iconIdx)
{
    lcd.setCursor(col, row);
    lcd.write(iconIdx);
}
