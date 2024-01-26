#ifndef DS1307H_H
#define DS1307H_H

#include <Arduino.h>

// Const declarations
// TODO: Modify so ADDRESS is external constant
const uint8_t RTC_ADDRESS = 0x68;

const uint8_t REG_SEC = 0x00;
const uint8_t REG_MIN = 0x01;
const uint8_t REG_HOUR = 0x02;
const uint8_t REG_DAY = 0x04;
const uint8_t REG_MONTH = 0x05;
const uint8_t REG_YEAR = 0x06;

const uint8_t REG_CONTROL = 0x07;

// Funcation declarations
void rtcWrite(uint8_t reg, uint8_t val);
uint8_t rtcRead(uint8_t reg);
uint8_t rtcReadToInt(uint8_t reg);
uint8_t intToBcd(uint8_t val);
uint8_t bcdToInt(uint8_t bcd);
void printBcd(uint8_t bcd);

#endif