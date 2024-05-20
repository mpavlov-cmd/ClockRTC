#include <EditableClockMode.h>

EditableClockMode::EditableClockMode(LiquidCrystal_I2C &liqudCristal, DateTimeRtc &dateTime,unsigned int refreshInterval) 
: ClockMode(liqudCristal, dateTime, refreshInterval)
{}
