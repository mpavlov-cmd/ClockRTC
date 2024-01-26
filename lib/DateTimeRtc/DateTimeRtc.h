#ifndef DATETIMERCT_H
#define DATETIMERCT_H

#include <Arduino.h>

const uint8_t ZERO_MASK = 0b000000;
const uint8_t FULL_MASK = 0b111111;

struct DateTimeRtc {
    private:
        uint8_t year, month, day, hours, minutes, seconds;
        uint8_t changeMask = ZERO_MASK;

    public:
        DateTimeRtc(uint8_t y, uint8_t m, uint8_t d, uint8_t hr, uint8_t mt, uint8_t sc);
        DateTimeRtc();

        void setValue(uint8_t value, uint8_t index);
        uint8_t getMask();
        void flushMask();

        void asTimeString(char *message);
        void asDateString(char *message);

        uint8_t& byIndex(uint8_t idx);

};

#endif  // DATETIMERCT_H
