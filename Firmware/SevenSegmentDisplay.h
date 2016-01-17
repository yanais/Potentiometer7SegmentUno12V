#ifndef SegmentDisplay_H
#define SegmentDisplay_H

#include "arduino.h"
#include <SPI.h>

#define ClearDisplay 0x76
#define DotsControl 0x77
#define BrightnessControl 0x7A
#define OutOfRangeError 0
#define Colon 0b00010000

class SevenSegmentDisplay
{
	public:
		SevenSegmentDisplay(int csPin);
		byte write(int number);
		byte write(float number);
		byte write(String str);
		byte write(int a,int b);
		byte write(float a,float b);
		void clear();
		void setBrightness(byte value);
		
	private:
		const int csPin;
};
#endif //SegmentDisplay_H
