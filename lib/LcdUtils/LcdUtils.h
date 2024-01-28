#ifndef LCDUTILS_H
#define LCDUTILS_H

#include <LiquidCrystal_74HC595.h>

void darwIcon(LiquidCrystal_74HC595 &lcd, uint8_t col, uint8_t row, uint8_t iconIdx);
void clearCell(LiquidCrystal_74HC595 &lcd, uint8_t col, uint8_t row);
void printClockHud(LiquidCrystal_74HC595 &lcd, boolean &shownHud, uint8_t offset);

#endif