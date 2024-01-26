#include <Arduino.h>
#include <Wire.h>
#include <DS1307M.h>

//TODO: Make it struct instead of plaie functions
void rtcWrite(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

uint8_t rtcRead(uint8_t reg)
{

  // Point chip to address we want to read from
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();

  // We can read all bytes at once if we want - will be more efficient
  uint8_t numBuyes = 1;
  Wire.requestFrom(RTC_ADDRESS, numBuyes);

  uint8_t bcd = Wire.read();

  return bcd;
}

uint8_t rtcReadToInt(uint8_t reg)
{
  uint8_t bcd = rtcRead(reg);
  return bcdToInt(bcd);
}

// Convert int to bcd representation e.g 0000 0001
uint8_t intToBcd(uint8_t val)
{

  uint8_t high = (val / 10) << 4;
  uint8_t low = val % 10;

  uint8_t result = high | low;

  return result;
}

// Convert bcd to int
uint8_t bcdToInt(uint8_t bcd)
{
  uint8_t low = bcd & 0x0F;
  uint8_t high = bcd >> 4;

  uint8_t result = (high * 10) + low;

  return result;
}

void printBcd(uint8_t bcd)
{
  for (int i = 7; i >= 0; i--)
  {
    // Check if the i-th bit is set using the bitwise AND operator (&)
    unsigned int currentBit = bitRead(bcd, i);
    Serial.print(currentBit);
  }
  Serial.println();
}