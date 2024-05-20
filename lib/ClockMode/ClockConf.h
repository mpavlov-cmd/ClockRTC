#ifndef CLOCKCONF_H
#define CLOCKCONF_H

#pragma once

#include <EditableClockMode.h>

struct ClockConf : public EditableClockMode {

        protected:
            void positionCursor() override;

        public:
            ClockConf(LiquidCrystal_I2C &liqudCristal, DateTimeRtc &dateTime, unsigned int refreshInterval); 

            void onRefresh(const unsigned long& mills) override;
            void onModeEnter() override;
            void onModeBtnClicked(uint8_t &mode) override;
            void onModeBtnHeld(uint8_t &mode) override;
            void onUpBtnClicked() override;
            void onDownBtnClicked() override;

            void toRtc();
};

#endif