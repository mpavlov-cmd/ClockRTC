#include "EncoderHandler.h"

void handleEncoderInput(RotaryEncoder &encoder, void (*onTurnRight)(), void (*onTurnLeft)())
{
    static int pos = 0;
    
    encoder.tick();

	int newPos = encoder.getPosition();
	int dir = (int)(encoder.getDirection());
    
	if (pos != newPos)
	{

		if (newPos > 99 || newPos < -99)
		{
			encoder.setPosition(0);
		}

        if (newPos > pos && dir > 0) {
            onTurnRight();
        } 

        if (newPos < pos && dir < 0) {
            onTurnLeft();
        }

		pos = newPos;
	}
}