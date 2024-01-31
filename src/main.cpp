#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_74HC595.h>
#include <EventButton.h>

#include <DateTimeRtc.h>
#include <DS1307M.h>
#include <Icons.h>

#include <Clock.h>
#include <ClockConf.h>

// Function declarations
void confTimeDs1307(DateTimeRtc &dt);

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

ClockMode* clock  = new Clock(lcd, currentTimeObj, 500);
ClockMode* clock2 = new ClockConf(lcd, initDt, 10);

ClockMode* modes[] = {clock, clock2};
uint8_t modeIdx = 0;

void setup()
{
  // TODO: Remove
  Serial.begin(9600);

  // Wire
  Wire.begin();

  // Set and read control
  rtcWrite(REG_CONTROL, RTC_SQWE_1);

  // Write initial time
  confTimeDs1307(initDt);

  // LCD Init
  lcd.begin(16, 2);
  // Setup custom chars
  lcd.createChar(CHAR_ARROW_UP_IDX, CHAR_ARROW_UP);
  lcd.createChar(CHAR_ARROW_DOWN_IDX, CHAR_ARROW_DOWN);

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

void confTimeDs1307(DateTimeRtc &dt)
{
  rtcWrite(REG_HOUR, intToBcd(dt.byIndex(0)));
  rtcWrite(REG_MIN, intToBcd(dt.byIndex(1)));
  rtcWrite(REG_SEC, intToBcd(dt.byIndex(2)));
  rtcWrite(REG_DAY, intToBcd(dt.byIndex(3)));
  rtcWrite(REG_MONTH, intToBcd(dt.byIndex(4)));
  rtcWrite(REG_YEAR, intToBcd(dt.byIndex(5)));
}


void onModeBtnClicked(EventButton &eb)
{
  // Store current mode 
  uint8_t currentMode = modeIdx;
  // Handle mode button click by delegate
  modes[modeIdx]->onModeBtnClicked(modeIdx);

  if (currentMode != modeIdx) {
    // Mode changed, check out of bounds
    if (modeIdx > sizeof(modes)/sizeof(modes[0]) - 1) {
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
