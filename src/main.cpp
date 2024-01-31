#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_74HC595.h>
#include <EventButton.h>

#include <DateTimeRtc.h>
#include <DS1307M.h>
#include <Clock.h>
#include <ClockConf.h>

// Function declarations
void onModeBtnClicked(EventButton &eb);
void onModeBtnHeld(EventButton &eb);
void onUpBtnClicked(EventButton &eb);
void onDownBtnClicked(EventButton &eb);

// Constat declarations
const uint8_t RTC_SQWE_1 = 0b00010000;

// Setup LCD with shift register
/*
SR -> ATMEGA
----------
DS    -> IO-11
SH_CP -> IO-12
ST_CP -> IO-10

SR -> LCD
----------
RS -> 1
E  -> 3
D4 -> 4
D5 -> 5
D6 -> 6
D7 -> 7
*/
LiquidCrystal_74HC595 lcd(11, 12, 10, 1, 3, 4, 5, 6, 7);

// Buttons
EventButton upButton(2);
EventButton downBunnon(3);
EventButton modeButton(4);

// Init Date Time Objects
// HH-MM-SS DD-MM-YY
DateTimeRtc initDt(23, 59, 55, 31, 12, 99);
DateTimeRtc currentTimeObj;

// Modes definition 
const uint8_t MODES_COUNT = 2;
ClockMode* modes[MODES_COUNT];
uint8_t modeIdx = 0;

void setup()
{
  // TODO: Remove
  Serial.begin(9600);

  // Wire
  Wire.begin();

  // Set and read control
  rtcWrite(REG_CONTROL, RTC_SQWE_1);

  // LCD Init
  lcd.begin(16, 2);

  // Clock Conf will write initial time to RTC in construtor
  modes[0] = new Clock(lcd, currentTimeObj, 500);
  modes[1] = new ClockConf(lcd, initDt, 10);

  // Buttom hand
  modeButton.setClickHandler(onModeBtnClicked);
  modeButton.setLongClickHandler(onModeBtnHeld);
  upButton.setClickHandler(onUpBtnClicked);
  downBunnon.setClickHandler(onDownBtnClicked);
}

void loop()
{
  // Get mills
  unsigned long currentMillis = millis();

  modeButton.update();
  upButton.update();
  downBunnon.update();

  modes[modeIdx]->onRefresh(currentMillis);
}

void onModeBtnClicked(EventButton &eb)
{
  // Store current mode 
  uint8_t currentMode = modeIdx;
  // Handle mode button click by delegate
  modes[modeIdx]->onModeBtnClicked(modeIdx);

  if (currentMode != modeIdx) {
    // Mode changed, check out of bounds
    if (modeIdx > MODES_COUNT - 1) {
      modeIdx = 0;
    }
    // Fire onModeEnter for the next mode
    modes[modeIdx]->onModeEnter();
  }
}

void onModeBtnHeld(EventButton &eb)
{
  modes[modeIdx]->onModeBtnHeld(modeIdx);
}

void onUpBtnClicked(EventButton &eb)
{
  modes[modeIdx]->onUpBtnClicked();
}

void onDownBtnClicked(EventButton &eb)
{
  modes[modeIdx]->onDownBtnClicked();
}
