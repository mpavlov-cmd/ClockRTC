#ifndef ALARMF_H
#define ALARM_H

#pragma once

#include <ClockMode.h>

struct Alarm : public ClockMode {

        private:
            const uint8_t CURSOR_ON_OFF[2] = {11, 0};

            boolean isHoodSHown = false;
            boolean enabled     = false;
            boolean editMode    = false; 
            uint8_t confIdx = 0;

            uint8_t name;

            // TODO: Remove duplicate code
            boolean switchedToEdit()
            {
                if (editMode)
                {
                    return false;
                }

                positionCursor();
                lcd.cursor();
                editMode = true;

                return true;
            }

            void positionCursor() {
                lcd.setCursor(LCD_TIME_MAP[confIdx][0] + LCD_OFFSET + 1, LCD_TIME_MAP[confIdx][1]);
            }

        public:
            const uint8_t LCD_TIME_MAP[6][2] = {{0, 1}, {3, 1}, {6, 1}, {UINT8_MAX, 0}, {UINT8_MAX, 0}, {UINT8_MAX, 0}};

            Alarm(LiquidCrystal_I2C &liqudCristal,DateTimeRtc &dateTime,unsigned int refreshInterval,uint8_t name); 

            void onRefresh(const unsigned long& mills) override;
            void onModeEnter() override;
            void onModeBtnClicked(uint8_t &mode) override;
            void onModeBtnHeld(uint8_t &mode) override;
            void onUpBtnClicked() override;
            void onDownBtnClicked() override;
};

#endif