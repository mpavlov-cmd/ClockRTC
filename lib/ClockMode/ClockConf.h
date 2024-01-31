#ifndef CLOCKCONF_H
#define CLOCKCONF_H

#pragma once

#include <ClockMode.h>

struct ClockConf : public ClockMode {

        private:
            boolean isHoodSHown = false; 
            uint8_t confIdx = 0;

        public:
            ClockConf(LiquidCrystal_74HC595 &liqudCristal,DateTimeRtc &dateTime,unsigned int refreshInterval); 

            void onRefresh(unsigned long mills) override;
            void onModeEnter() override;
            void onModeBtnClicked(uint8_t &mode) override;
            void onModeBtnHeld(uint8_t &mode) override;
            void onUpBtnClicked() override;
            void onDownBtnClicked() override;
};

#endif