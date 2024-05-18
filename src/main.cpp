#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EventButton.h>
#include <RotaryEncoder.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include <DateTimeRtc.h>
#include <DS1307M.h>
#include <Clock.h>
#include <ClockConf.h>
#include <Alarm.h>
#include <Buzzer.h>
#include <Timed.h>
#include <TimedPin.h>
#include <LcdUtils.h>

// Function declarations
void onUpBtnClicked();
void onDownBtnClicked();
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
const uint8_t RTC_SQWE_1  = 0b00010000;
const uint8_t MODES_COUNT = 4;
const uint8_t LCD_COLS    = 16;
const uint8_t LCD_ROWS    = 2;

const unsigned int ALIVE_FOR = 15000;

/*
    Set Up LCD with PCF8574
    Change the I2C address based on the actual value of the display
    Address is configurable based on 1, 2, 3 pins of the PCF8574

    IMPORTANT. LIB Has Been modified to disable backlight by default
*/
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

// Buttons
EventButton modeButton(PIN_PB1);
RotaryEncoder encoder(PIN_PB2, PIN_PB3, RotaryEncoder::LatchMode::FOUR3);

Buzzer buzzer(PIN_PB0, true);
Timed backlight(backlight.NOOP_PIN, lcd, true);

// Init Date Time Objects
// HH-MM-SS DD-MM-YY
DateTimeRtc initDt(23, 59, 55, 31, 12, 99);
DateTimeRtc alarmOne(07, 00, 00, 01, 01, 00);
DateTimeRtc alarmTwo(06, 15, 00, 01, 01, 00);
DateTimeRtc currentTimeObj;

// Modes definition
ClockMode* modes[MODES_COUNT];
uint8_t modeIdx = 0;

// Sleep mode timer
unsigned long powerUpThreshold = ALIVE_FOR;
unsigned long timeEllapsed = 0;

volatile boolean nowSleeping = false;
volatile boolean awaken      = false;

// Init main clock so it can be used
Clock mainClock(lcd, currentTimeObj, 500);
ClockConf clockConf(lcd, initDt, 10);

// Interrupt handler for port B
// To check what button was clicked digitalRead(PIN) is LOW;
ISR(PCINT0_vect)
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

    // Configure LCD Power PIN and LCD Init
    pinMode(PIN_PB4, OUTPUT);
    setUpLcd(lcd, LCD_COLS, LCD_ROWS);

    // Wire
    // Wire.begin(); Wire already started by LCD Lib
    initTime();

    // Clock Conf will write initial time to RTC in construtor
    modes[0] = &mainClock;
    modes[1] = &clockConf;
    modes[2] = new Alarm(lcd, alarmOne, 10, 1);
    modes[3] = new Alarm(lcd, alarmTwo, 10, 2);

    // Button handlers
    modeButton.setClickHandler(onModeBtnClicked);
    modeButton.setLongClickHandler(onModeBtnHeld);

    // Hello on Startup
    // TODO: Fix
    digitalWrite(PIN_PB0, HIGH);
    delay(50);
    digitalWrite(PIN_PB0, LOW);
}

void loop()
{
    // Get mills
    unsigned long mills = millis();
    timeEllapsed = mills;

    // Check just if woke up
    wakeUp(mills);

    // Update inputs and outputs
    modeButton.update();
    
    buzzer.update(mills);
    backlight.update(mills);

    // Encoder code
	static int pos = 0;
	encoder.tick();

	int newPos = encoder.getPosition();
	int dir = (int)(encoder.getDirection());
	if (pos != newPos)
	{

		if (newPos > 99 || newPos < -99)
		{
			encoder.setPosition(0);
		}

        if (newPos > pos && dir > 0) {
            onUpBtnClicked();
        } 

        if (newPos < pos && dir < 0) {
            onDownBtnClicked();
        }

		pos = newPos;
	}

    if (!nowSleeping) {
        modes[modeIdx]->onRefresh(mills);
    }

    goToSleep(mills);
}

void onUpBtnClicked()
{
    stayAwake(ALIVE_FOR);
    modes[modeIdx]->onUpBtnClicked();
}

void onDownBtnClicked()
{
    stayAwake(ALIVE_FOR);
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

void  goToSleep(const unsigned long &mills)
{

    if (mills < powerUpThreshold)
    {
        return;
    }

    if (!nowSleeping)
    {
        buzzer.mute();
        backlight.mute();
        powerSaveDisplay(true, mills);

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

    // Drop awaken flag
    awaken = false;

    wdt_disable();
    
    powerSaveDisplay(false, mills);
    stayAwake(ALIVE_FOR);
}

void stayAwake(const unsigned int& leaveFor)
{
    powerUpThreshold = millis() + leaveFor;
}

void powerSaveDisplay(boolean enable, const unsigned long& mills)
{
    if (enable) {
        lcd.noBacklight();
        digitalWrite(PIN_PB4, LOW);
    } else {
        setUpLcd(lcd, LCD_COLS, LCD_ROWS);
    }

    modeIdx = 0;
    mainClock.setShowSeconds(!enable);
    mainClock.onModeEnter();
    mainClock.onRefresh(mills);
}

void enablePCI()
{
    // Configure interrupts
    // Configure PCI - Pin Change Interrupt for Port B
    PCICR |= B00000001;
    // Enable PB1, PB2, PB3 for interrupts
    PCMSK0 |= B00001110;
}

void disablePCI()
{
    // Disable interrupts
    PCICR = PCICR & ~B00000001;
    PCMSK0 = PCMSK0 & ~B00001110;
}

void lightAndBeep()
{
    buzzer.up(50);
    //backlight.up(10000);
}
