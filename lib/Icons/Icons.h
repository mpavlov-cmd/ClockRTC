#ifndef ICONS_H
#define ICONS_H

#include <Arduino.h>

const uint8_t CHAR_ARROW_UP_IDX = 0;
uint8_t CHAR_ARROW_UP[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B01110,
  B11111,
  B00000
};

const uint8_t CHAR_ARROW_DOWN_IDX = 1;
uint8_t CHAR_ARROW_DOWN[8] = {
  B00000,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000
};


#endif