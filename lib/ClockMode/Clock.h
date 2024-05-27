#ifndef CLOCK_H
#define CLOCK_H

#pragma once

#include <ClockMode.h>

struct Clock : public ClockMode {

        private:

            const uint8_t LCD_DT_MAP_NS[6][2] = {{3, 0}, {6, 0}, {UINT8_MAX, 0}, {0, 1}, {3, 1}, {6, 1}};

            boolean showSeconds   = true;
            boolean forsedRefresh = false;
            boolean outputAllowed = true;

        public:
            Clock(LiquidCrystal_I2C &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval); 

            void onRefresh(const unsigned long& mills) override;
            void onModeEnter() override;
            void onModeBtnClicked(uint8_t &mode) override;
            void onModeBtnHeld(uint8_t &mode) override;
            void onUpBtnClicked() override;
            void onDownBtnClicked() override;

            void setShowSeconds(boolean value);
            void forceNextRefresh();
            void setOutputAllowed(boolean value);
};

#endif