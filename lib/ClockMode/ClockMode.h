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
        
        // Variables for render timeouts
        unsigned int refInt;
        unsigned long timeEllapsed = 0;

        // Common logic 
        boolean shouldRefresh(unsigned long mills);
        // To be implemeted for specific IC to store time to
        void toRtc();

    public:

        const uint8_t LCD_OFFSET = 4;
        const uint8_t LCD_DT_MAP[6][2] = {{0, 0}, {3, 0}, {6, 0}, {0, 1}, {3, 1}, {6, 1}};

        // Common constructor 
        ClockMode(LiquidCrystal_74HC595 &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval);

        virtual void onRefresh(unsigned long mills) = 0; // Pure virtual function making ClockMode an abstract class
        virtual void onModeEnter() = 0;
        virtual void onModeBtnClicked(uint8_t &mode) = 0;
        virtual void onModeBtnHeld(uint8_t &mode) = 0;
        virtual void onUpBtnClicked() = 0;
        virtual void onDownBtnClicked() = 0;
    

    virtual ~ClockMode() {} // Virtual destructor to allow proper cleanup in derived classes
};

#endif