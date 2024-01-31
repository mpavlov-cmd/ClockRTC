#include "LcdUtils.h"

void printTimeDate(LiquidCrystal_74HC595 &lcd, DateTimeRtc &dt, const uint8_t mappings[6][2], uint8_t offset)
{
    uint8_t mask = dt.getMask();

    // printBcd(mask);
    for (int i = 0; i < 6; i++)
    {
        uint8_t changed = bitRead(mask, i);
        // If no changes continue
        if (changed == 0)
        {
            continue;
        }
        // Changed
        char printVal[3];
        sprintf(printVal, "%02d", dt.byIndex(i));

        lcd.setCursor(mappings[i][0] + offset, mappings[i][1]);
        lcd.print(printVal);
        dt.flushMask();
    }
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
