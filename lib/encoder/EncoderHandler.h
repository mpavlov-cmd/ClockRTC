#ifndef ENCODERHANDLER_H
#define ENCLDERHANDLER_H

#include <RotaryEncoder.h>

void handleEncoderInput(RotaryEncoder& encoder, void (*onTurnLeft)(),  void (*onTurnRight)());

#endif