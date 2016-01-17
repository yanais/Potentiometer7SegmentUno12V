#include "Potentiometer.h"

Potentiometer::Potentiometer(int pin)
{
	analogPin = pin;
}

int Potentiometer::read()
{
	return analogRead(analogPin);
}



