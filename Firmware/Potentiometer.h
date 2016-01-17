#ifndef POTENTIOMETER
#define POTENTIOMETER

#include "arduino.h"

class Potentiometer
{
	public:
		Potentiometer(int pin);
		int read(); 
		
	private:
		int analogPin;
		
};
#endif //POTENTIOMETER
