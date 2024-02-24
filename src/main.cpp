#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_74HC595.h>
#include <EventButton.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include <DateTimeRtc.h>
#include <DS1307M.h>
#include <Clock.h>
#include <ClockConf.h>
#include <Alarm.h>
#include <Buzzer.h>
#include <Timed.h>
#include <LcdUtils.h>

// Function declarations
void onUpBtnClicked(EventButton &eb);
void onDownBtnClicked(EventButton &eb);
void onModeBtnClicked(EventButton &eb);
void onModeBtnHeld(EventButton &eb);
void handleModeChange(uint8_t srcMode, uint8_t &modeIdx);

void initTime();
void goToSleep(const unsigned long& mills);
void wakeUp(const unsigned long& mills); 
void stayAwake(const unsigned int& leaveFor);
void powerSaveDisplay(boolean enable, const unsigned long& mills);

// TODO: Move to lib
void enablePCI();
void disablePCI();

void lightAndBeep();

// Constat declarations
const uint8_t RTC_SQWE_1 = 0b00010000;
const uint8_t MODES_COUNT = 4;

const unsigned int ALIVE_FOR = 15000;

/*
Setup LCD with shift register
SR to ATMEGA
----------
DS    -> IO-11
SH_CP -> IO-12
ST_CP -> IO-10

SR to LCD
----------
RS -> 1, E  -> 3, D4 -> 4, D5 -> 5, D6 -> 6, D7 -> 7
*/
LiquidCrystal_74HC595 lcd(11, 12, 10, 1, 3, 4, 5, 6, 7);

// Buttons
EventButton upButton(2);
EventButton downBunnon(3);
EventButton modeButton(4);

Buzzer buzzer(5, true);
Timed backlight(6, true);

// Init Date Time Objects
// HH-MM-SS DD-MM-YY
DateTimeRtc initDt(23, 59, 55, 31, 12, 99);
DateTimeRtc alarmOne(07, 00, 00, 01, 01, 00);
DateTimeRtc alarmTwo(06, 15, 00, 01, 01, 00);
DateTimeRtc currentTimeObj;

// Modes definition
ClockMode *modes[MODES_COUNT];
uint8_t modeIdx = 0;

// Sleep mode timer
unsigned long powerUpThreshold = ALIVE_FOR;
unsigned long timeEllapsed = 0;

volatile boolean nowSleeping = false;
volatile boolean awaken = false;

volatile boolean enabledByWatchDog = false;

// Init main clock so it can be used
Clock mainClock(lcd, currentTimeObj, 500);
ClockConf clockConf(lcd, initDt, 10);

// Interrupt handler for port D
// To check what button was clicked digitalRead(PIN) is LOW;
ISR(PCINT2_vect)
{
    disablePCI();

    awaken = true;
    nowSleeping = false;
}

// Watchdog interrupt
ISR(WDT_vect)
{
    // Disable watchdog
    wdt_disable();
}

void setup()
{
    // Turn off ADC (Analog to digital converter)
    ADCSRA = 0;

    // TODO: Remove
    Serial.begin(9600);

    // Wire
    Wire.begin();
    initTime();

    // LCD Init
    setUpLcd(lcd, 16, 2);

    // Clock Conf will write initial time to RTC in construtor
    modes[0] = &mainClock;
    modes[1] = &clockConf;
    modes[2] = new Alarm(lcd, alarmOne, 10, 1);
    modes[3] = new Alarm(lcd, alarmTwo, 10, 2);

    // Button handlers
    modeButton.setClickHandler(onModeBtnClicked);
    modeButton.setLongClickHandler(onModeBtnHeld);
    upButton.setClickHandler(onUpBtnClicked);
    downBunnon.setClickHandler(onDownBtnClicked);
}

void loop()
{
    // Get mills
    unsigned long mills = millis();
    timeEllapsed = mills;

    // Check just if woke up
    wakeUp(mills);

    modeButton.update();
    upButton.update();
    downBunnon.update();

    buzzer.update(mills);
    backlight.update(mills);

    modes[modeIdx]->onRefresh(mills);

    goToSleep(mills);
}

void onUpBtnClicked(EventButton &eb)
{
    stayAwake(ALIVE_FOR);
    lightAndBeep();
    modes[modeIdx]->onUpBtnClicked();
}

void onDownBtnClicked(EventButton &eb)
{
    stayAwake(ALIVE_FOR);
    lightAndBeep();
    modes[modeIdx]->onDownBtnClicked();
}

void onModeBtnClicked(EventButton &eb)
{
    stayAwake(ALIVE_FOR);
    lightAndBeep();
    // Store current mode
    uint8_t currentMode = modeIdx;
    modes[modeIdx]->onModeBtnClicked(modeIdx);
    handleModeChange(currentMode, modeIdx);
}

void onModeBtnHeld(EventButton &eb)
{
    stayAwake(ALIVE_FOR);
    lightAndBeep();
    // Store current mode
    uint8_t currentMode = modeIdx;
    modes[modeIdx]->onModeBtnHeld(modeIdx);
    handleModeChange(currentMode, modeIdx);
}

void handleModeChange(uint8_t srcMode, uint8_t &modeIdx)
{
    if (srcMode != modeIdx)
    {
        // Mode changed, check out of bounds
        if (modeIdx > MODES_COUNT - 1)
        {
            modeIdx = 0;
        }
        // Fire onModeEnter for the next mode
        modes[modeIdx]->onModeEnter();
    }
}

void initTime()
{
    if (rtcRead(REG_CONTROL) == RTC_SQWE_1) {
        return;
    }

    rtcWrite(REG_CONTROL, RTC_SQWE_1);
    clockConf.toRtc();
}

void goToSleep(const unsigned long &mills)
{

    if (mills < powerUpThreshold)
    {
        return;
    }

    if (!nowSleeping)
    {
        powerSaveDisplay(true, mills);
        buzzer.mute();
        backlight.mute();

        nowSleeping = true;
    }

    // Force refresh clock on the next wakeup
    mainClock.forceNextRefresh();

    // ------ WDT Configuration Start ------
    // Feed the dog
    wdt_reset();

    MCUSR &= ~_BV(WDRF);
    // Start the WDT Config change sequence.
    WDTCSR |= _BV(WDCE) | _BV(WDE);
    // Configure the prescaler and the WDT for interrupt mode only 
    WDTCSR = _BV(WDP0) | _BV(WDP3) | _BV(WDIE);
    // ------ WDT Configuration End ------

    // Configure sleep mode
    sleep_enable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    
    enablePCI();
    sleep_cpu();
}

void wakeUp(const unsigned long& mills)
{
    if (!awaken)
    {
        return;
    }

    wdt_disable();
    
    powerSaveDisplay(false, mills);
    stayAwake(ALIVE_FOR);

    // Drop awaken flag
    awaken = false;
}

void stayAwake(const unsigned int& leaveFor)
{
    powerUpThreshold = millis() + leaveFor;
}

void powerSaveDisplay(boolean enable, const unsigned long& mills)
{
    modeIdx = 0;
    mainClock.setShowSeconds(!enable);
    mainClock.onModeEnter();
    mainClock.onRefresh(mills);
}

void enablePCI()
{
    // Configure interrupts
    // Configure PCI - Pin Change Interrupt for Port D
    PCICR |= B00000100;
    // Enable Pins 4,5,6 of ATMEGA - 2,3,4 of arduino for interrupts
    PCMSK2 |= B00011100;
}

void disablePCI()
{
    // Disable interrupts
    PCICR = PCICR & ~B00000100;
    PCMSK2 = PCMSK2 & ~B00011100;
}

void lightAndBeep()
{
    buzzer.up(50);
    backlight.up(10000);
}
