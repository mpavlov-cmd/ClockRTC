#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_74HC595.h>
#include <EventButton.h>

#include <DateTimeRtc.h>
#include <DS1307M.h>

// Function declarations
void getTimeDs1307(DateTimeRtc &dt);
void confTimeDs1307(DateTimeRtc &dt);
void printTime(DateTimeRtc &dt);

// TODO: Replace EventButton with Bounce2 lib
void onModeBtnClicked(EventButton &eb);
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

// Init Date Time Objects
// HH-MM-SS DD-MM-YY 
DateTimeRtc initDt(12, 00, 00, 01, 01, 20);
DateTimeRtc currentTimeObj;

// Buttons
EventButton upButton(2);
EventButton downBunnon(3);
EventButton modeButton(4);

// Store milliseconds
unsigned long timeEllapsed = 0;
const unsigned long lcdRefreshIntervalMills = 200;

/* Modes
  0 - clock
  1 - set clock
  1 - alarm 1
*/


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

  modeButton.setClickHandler(onModeBtnClicked);
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

  if (currentMillis - timeEllapsed >= lcdRefreshIntervalMills)
  {
    // Update time Ellapsed
    timeEllapsed = currentMillis;

    // Print time to LCD
    getTimeDs1307(currentTimeObj);

    printTime(currentTimeObj);
  }
}

void getTimeDs1307(DateTimeRtc &dt)
{
  dt.setValue(rtcReadToInt(REG_HOUR),  0);
  dt.setValue(rtcReadToInt(REG_MIN),   1);
  dt.setValue(rtcReadToInt(REG_SEC),   2);
  dt.setValue(rtcReadToInt(REG_DAY),   3);
  dt.setValue(rtcReadToInt(REG_MONTH), 4);
  dt.setValue(rtcReadToInt(REG_YEAR),  5); 
}

void confTimeDs1307(DateTimeRtc &dt)
{
  rtcWrite(REG_HOUR,  intToBcd(dt.byIndex(0)));
  rtcWrite(REG_MIN,   intToBcd(dt.byIndex(1)));
  rtcWrite(REG_SEC,   intToBcd(dt.byIndex(2)));
  rtcWrite(REG_DAY,   intToBcd(dt.byIndex(3)));
  rtcWrite(REG_MONTH, intToBcd(dt.byIndex(4)));
  rtcWrite(REG_YEAR,  intToBcd(dt.byIndex(5)));
}

void printTime(DateTimeRtc &dt) {
  uint8_t mask = dt.getMask();
  printBcd(mask);
  for (int i = 0; i<6; i++) {
    uint8_t changed = bitRead(mask, i);
    // If no changes continue 
    if (changed == 0) {
      continue;
    }
    // Changed
    // Hours 
    char printVal[3];
    sprintf(printVal, "%02d", dt.byIndex(i));

    switch (i)
    {
    case 0: // Hour
      lcd.setCursor(0, 0);
      break;
    case 1: // Min 
      lcd.setCursor(3, 0);
      break;
    case 2: // Sec
      lcd.setCursor(6, 0);
      break;
    case 3: // Year
      lcd.setCursor(0, 1);
      break;
    case 4:
      lcd.setCursor(3, 1);
      break;
    case 5:
      lcd.setCursor(6, 1);
      break;
    default:
      break;
    }

    lcd.print(printVal);
    dt.flushMask();
  }
}

void onModeBtnClicked(EventButton &eb)
{
  Serial.println("Mode Btn Clicked");
}
void onUpBtnClicked(EventButton &eb)
{
  Serial.println("Up Btn Clicked");
}
void onDownBtnClicked(EventButton &eb)
{
  Serial.println("Down Btn Clicked");
}