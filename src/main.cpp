#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_74HC595.h>
#include <EventButton.h>

#include <DateTimeRtc.h>
#include <DS1307M.h>

// Function declarations
void getTimeDs1307(DateTimeRtc &dt);
void confTimeDs1307(DateTimeRtc &dt);

void printTime(DateTimeRtc &dt, uint8_t offset);
void printClockHud(uint8_t offset);

void onModeBtnClicked(EventButton &eb);
void onUpBtnClicked(EventButton &eb);
void onDownBtnClicked(EventButton &eb);

// Constat declarations
const uint8_t RTC_SQWE_1 = 0b00010000;
const uint8_t MODE_CLOCK = 0;
const uint8_t MODE_SETUP = 1;

// Maps LCD Cursor to a given position on 16/2 screet
const uint8_t LCD_MAPPINGS[6][2] = {{0, 0}, {3, 0}, {6, 0}, {0, 1}, {3, 1}, {6, 1}};
const uint8_t LCD_OFFSET = 4;

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
unsigned long timeEllapsedClockRefresh = 0;
const uint8_t refreshIntervalClockMode = 200;

unsigned long timeEllapsedInputRead = 0;
const uint8_t refreshIntervalInputRead = 10;

uint8_t mode = 0;
uint8_t confTimeIndex = 0;

boolean hudDisplayed = false;

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

  // Refresh screen in clock mode
  if (currentMillis - timeEllapsedClockRefresh >= refreshIntervalClockMode)
  {
    // Update time ellapsed
    timeEllapsedClockRefresh = currentMillis;

    if (mode == MODE_CLOCK) {
      // Display HUD
      if (!hudDisplayed) {
        printClockHud(LCD_OFFSET);
        hudDisplayed = true;
      }

      // Get and Print current time
      getTimeDs1307(currentTimeObj);
      printTime(currentTimeObj, LCD_OFFSET);
    }
  }

  // Refresh screen in setup mode
  if (currentMillis - timeEllapsedInputRead >= refreshIntervalInputRead) {
    // Update time ellapsed
    timeEllapsedInputRead = currentMillis;

    if (mode == MODE_SETUP) {
      // Reuse init time object for configs
      printTime(initDt, LCD_OFFSET);
    }

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

void printTime(DateTimeRtc &dt, uint8_t offset) {
  uint8_t mask = dt.getMask();
  
  // printBcd(mask);
  for (int i = 0; i < 6; i++) {
    uint8_t changed = bitRead(mask, i);
    // If no changes continue 
    if (changed == 0) {
      continue;
    }
    // Changed
    char printVal[3];
    sprintf(printVal, "%02d", dt.byIndex(i));

    lcd.setCursor(LCD_MAPPINGS[i][0] + offset, LCD_MAPPINGS[i][1]);
    lcd.print(printVal);
    dt.flushMask();
  }
}

// TODO: Optimize
void printClockHud(uint8_t offset)
{
  lcd.setCursor(2 + offset, 0);
  lcd.print(":");
  lcd.setCursor(5 + offset, 0);
  lcd.print(":");

  lcd.setCursor(2 + offset, 1);
  lcd.print("-");
  lcd.setCursor(5 + offset, 1);
  lcd.print("-");
}

void onModeBtnClicked(EventButton &eb)
{ 
  // Go from mode 1 to mode 2
  if (mode == MODE_CLOCK) {
    mode++;
    return;
  }

  // If mode is confTime iterate through values
  if (mode == MODE_SETUP) {
    confTimeIndex++;

    // Switch back to clock;
    if (confTimeIndex == 6) {
      confTimeIndex = 0;
      mode = MODE_CLOCK;
      // Forse screen refresh
      currentTimeObj.forseMask();
    }
  }

  Serial.println(mode);
  Serial.println(confTimeIndex);
}

void onUpBtnClicked(EventButton &eb)
{
  if (mode == MODE_SETUP) {
    uint8_t currentValue = initDt.byIndex(confTimeIndex);
    initDt.setValue(currentValue + 1, confTimeIndex);
  }
}
void onDownBtnClicked(EventButton &eb)
{
  Serial.println("Down Btn Clicked");
}