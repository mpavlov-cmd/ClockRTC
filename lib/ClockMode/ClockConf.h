#ifndef CLOCKCONF_H
#define CLOCKCONF_H

#pragma once

#include <ClockMode.h>

struct ClockConf : public ClockMode {

        private:
            boolean firstRun = true; 
            boolean editMode = false;
            uint8_t confIdx = 0;

            boolean switchedToEdit() {
                if (editMode) {
                    return false;
                }

                positionCursor();
                lcd.cursor();
                editMode = true;

                return true;
            }

            void positionCursor() {
                lcd.setCursor(LCD_DT_MAP[confIdx][0] + LCD_OFFSET + 1, LCD_DT_MAP[confIdx][1]);
            }

        public:
            ClockConf(LiquidCrystal_74HC595 &liqudCristal,DateTimeRtc &dateTime,unsigned int refreshInterval); 

            void onRefresh(const unsigned long& mills) override;
            void onModeEnter() override;
            void onModeBtnClicked(uint8_t &mode) override;
            void onModeBtnHeld(uint8_t &mode) override;
            void onUpBtnClicked() override;
            void onDownBtnClicked() override;

            void toRtc();
};

#endif