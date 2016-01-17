#include "Global.h"

void setup() 
{
	Serial.begin(9600);
	Serial.println("start");
}

void loop() 
{	
	int sample = potentiometer.read();
	Serial.println(sample);
	ssDisplay.write(sample);
	
	delay(100);
}
