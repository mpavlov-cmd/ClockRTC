#ifndef LCDUTILS_H
#define LCDUTILS_H

#pragma once

#include <LiquidCrystal_I2C.h>
#include <DateTimeRtc.h>
#include <Icons.h>

void setUpLcd(LiquidCrystal_I2C& lcd, uint8_t cols, uint8_t rows);
boolean printTimeDate(LiquidCrystal_I2C& lcd, DateTimeRtc& dt, const uint8_t mappings[6][2], uint8_t offset);
void printClockHud(LiquidCrystal_I2C& lcd, uint8_t offsetClock, uint8_t offsetCal, boolean showSeconds);
void printClockHud(LiquidCrystal_I2C& lcd, uint8_t offsetClock, boolean showSeconds);
void darwIcon(LiquidCrystal_I2C& lcd, uint8_t col, uint8_t row, uint8_t iconIdx);

#endif