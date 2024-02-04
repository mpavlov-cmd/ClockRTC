#ifndef ALARMF_H
#define ALARM_H

#pragma once

#include <ClockMode.h>

struct Alarm : public ClockMode {

        private:
            boolean isHoodSHown = false; 
            uint8_t confIdx = 0;
            String name;

        public:
            const uint8_t LCD_TIME_MAP[6][2] = {{0, 1}, {3, 1}, {6, 1}, {UINT8_MAX, 0}, {UINT8_MAX, 0}, {UINT8_MAX, 0}};

            Alarm(LiquidCrystal_74HC595 &liqudCristal,DateTimeRtc &dateTime,unsigned int refreshInterval, String name); 

            void onRefresh(unsigned long mills) override;
            void onModeEnter() override;
            void onModeBtnClicked(uint8_t &mode) override;
            void onModeBtnHeld(uint8_t &mode) override;
            void onUpBtnClicked() override;
            void onDownBtnClicked() override;
};

#endif