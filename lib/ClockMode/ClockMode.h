#ifndef CLOCKMODE_H
#define CLOCKMODE_H

#pragma once

#include <Arduino.h>
#include <LiquidCrystal_74HC595.h>
#include <DateTimeRtc.h>


struct ClockMode {

    protected:
        LiquidCrystal_74HC595 &lcd;
        DateTimeRtc &dt;
        unsigned int refInt;
        unsigned long timeEllapsed = 0;

    public:

        const uint8_t LCD_OFFSET = 4;

        // Common constructor 
        ClockMode(LiquidCrystal_74HC595 &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval);

        virtual void onRefresh(unsigned long mills) = 0; // Pure virtual function making ClockMode an abstract class
        virtual void onModeEnter() const = 0;
        virtual void onModeBtnClicked(uint8_t *mode) const = 0;
        virtual void onModeBtnHeld(uint8_t *mode) const = 0;
        virtual void onUpBtnClicked() const = 0;
        virtual void onDownBtnClicked() const = 0;
    

    virtual ~ClockMode() {} // Virtual destructor to allow proper cleanup in derived classes
};

#endif