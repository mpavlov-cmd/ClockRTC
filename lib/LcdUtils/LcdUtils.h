#ifndef LCDUTILS_H
#define LCDUTILS_H

#pragma once

#include <LiquidCrystal_74HC595.h>
#include <DateTimeRtc.h>

boolean printTimeDate(LiquidCrystal_74HC595 &lcd, DateTimeRtc &dt, const uint8_t mappings[6][2], uint8_t offset);
void printClockHud(LiquidCrystal_74HC595 &lcd, boolean &shownHud, uint8_t offset);
void darwIcon(LiquidCrystal_74HC595 &lcd, uint8_t col, uint8_t row, uint8_t iconIdx);


#endif