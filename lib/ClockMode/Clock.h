#ifndef CLOCK_H
#define CLOCK_H

#pragma once

#include <ClockMode.h>


struct Clock : public ClockMode {

        public:
            Clock(LiquidCrystal_74HC595 &liqudCristal,DateTimeRtc &dateTime,unsigned int refreshInterval); 

            void onRefresh(unsigned long mills) override;
            void onModeEnter() const override;
            void onModeBtnClicked(uint8_t *mode) const override;
            void onModeBtnHeld(uint8_t *mode) const override;
            void onUpBtnClicked() const override;
            void onDownBtnClicked() const override;
};

#endif