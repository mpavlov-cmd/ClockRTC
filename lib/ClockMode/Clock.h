#ifndef CLOCK_H
#define CLOCK_H

#pragma once

#include <ClockMode.h>

struct Clock : public ClockMode {

        private:
            boolean isHoodSHown = false; 

        public:
            Clock(LiquidCrystal_74HC595 &liqudCristal,DateTimeRtc &dateTime,unsigned int refreshInterval); 

            void onRefresh(unsigned long mills) override;
            void onModeEnter() override;
            void onModeBtnClicked(uint8_t &mode) override;
            void onModeBtnHeld(uint8_t &mode) override;
            void onUpBtnClicked() override;
            void onDownBtnClicked() override;
};

#endif