#include "LcdUtils.h"

void darwIcon(LiquidCrystal_74HC595 &lcd, uint8_t col, uint8_t row, uint8_t iconIdx)
{
    lcd.setCursor(col, row);
    lcd.write(iconIdx);
}

void clearCell(LiquidCrystal_74HC595 &lcd, uint8_t col, uint8_t row)
{
    lcd.setCursor(col, row);
    lcd.print(" ");
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
