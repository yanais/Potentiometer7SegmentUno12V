#include "SevenSegmentDisplay.h"

SevenSegmentDisplay::SevenSegmentDisplay(int csPin) : csPin(csPin)
{
	pinMode(csPin, OUTPUT); 
	digitalWrite(csPin, HIGH);  //Chip select pin is idle HIGH 
	//Config SPI
	SPI.begin(); 
	SPI.setClockDivider(SPI_CLOCK_DIV64);
  
	setBrightness(255);	
	clear(); 
}

byte SevenSegmentDisplay::write(int number)	//write integer number between -999 to 9999
{
	if(number > 9999 || number < -999)
		return OutOfRangeError;

	char tempString[10];
	sprintf(tempString, "%4d", number); //create a 4 digits string for int
	write(tempString); 	//write to the 7 segment as string
	return 1;
}

byte SevenSegmentDisplay::write(float number) //write decimal number with 4 digits accuracy
{
	if(number > 9999 || number < -999)
		return OutOfRangeError;
	
	if(number > 999 || number < -99)
		write(int(number));

	char tempString[10];				
	dtostrf(number, 5, 3, tempString);	//convert number to 5 chars with 3 digits after the dot, and enter it to tempString
	int dotLocation = -1;
	for(int i=1;i<4;i++)	//the dtostrf function return char array include the dot, this loop find the dot remove it and save the location
	{
		if(tempString[i] == '.')
		{
			dotLocation = i;
			for(i++;i<5;i++)	//Move all chars one place 
				tempString[i-1] = tempString[i];
		
			break; //dot found, stop loop
		}
	}
	
	//send data 
	digitalWrite(csPin, LOW);
	SPI.transfer(ClearDisplay);
	for (int i=0; i<4; i++)
		SPI.transfer(tempString[i]);	//send string
	if(dotLocation>=0)	//if dot found, turn it on in the right positoin
	{
		SPI.transfer(DotsControl); 
		SPI.transfer(1<<(dotLocation-1));
	}
	digitalWrite(csPin, HIGH);
	
	return 1;
}

byte SevenSegmentDisplay::write(String str) //write 4 letters long string
{
	clear(); 
	digitalWrite(csPin, LOW);
	for (int i=0; i<4; i++)
		SPI.transfer(str[i]);
		
	digitalWrite(csPin, HIGH);
	return 1;
}

byte SevenSegmentDisplay::write(int a, int b) //write two integers with colon between
{
	if(a > 99 || a<-9 || b>99 || b<-9)
		return OutOfRangeError;
		
	char tempString[10];
	if(b>=0)	//if b>0 it can handle as 4 digits integer
	{
		if(a>0)
			sprintf(tempString, "%4d", a*100 + b);
		else
			sprintf(tempString, "%4d", a*100 - b);
	}
	else 
	{
		if(a<0)
		{
			tempString[0] = '-';
			tempString[1] = (byte)(-a); //0x00 - 0x09  	
		}
		else
		{
			tempString[0] = (byte)(a/10);	//the tens digit
			tempString[1] = (byte)(a%10);	//the units digit
		}
		//the b part is just one digit and the '-' char
		tempString[2] = '-';	
		tempString[3] = (byte)(-b);	//the parser parse 0x00-0x09 as well as '0'-'9'
		
	}
	
	//send data:
	digitalWrite(csPin, LOW);
	SPI.transfer(ClearDisplay);
	for (int i=0; i<4; i++)
		SPI.transfer(tempString[i]);
	SPI.transfer(DotsControl); 	 
	SPI.transfer(Colon);		//turn on the Colon
	digitalWrite(csPin, HIGH);
	return 1;
}

byte SevenSegmentDisplay::write(float a, float b) //write two decimals with colon between
{
	if(a > 99 || a<-9 || b>99 || b<-9) //check the number are presentable
		return OutOfRangeError;
	byte dots = Colon;				//dots byte will store all bytes need to be presented
	char tempPart[4];				//for parsing float numbers
	char tempString[10];
	//parsing float a:
	dtostrf(a, 3, 1, tempPart);		//parse float into char array
	if(tempPart[1] == '.') //if second char is dot, add the dot to the dots byte and replace with the third char
	{
		tempPart[1] = tempPart[2];
		dots |= 1<<0;	//turn on the first dot
	}
	tempString[0] = tempPart[0];
	tempString[1] = tempPart[1];
	
	//parsing float b:
	dtostrf(b, 3, 1, tempPart);  
	if(tempPart[1] == '.')		
  {
		tempPart[1] = tempPart[2];
		dots |= 1<<2;	//turn on the third dot
	}
	tempString[2] = tempPart[0];
	tempString[3] = tempPart[1];
	
	//send data
	digitalWrite(csPin, LOW);
	SPI.transfer(ClearDisplay);
	for (int i=0; i<4; i++)
		SPI.transfer(tempString[i]);
	SPI.transfer(DotsControl); // 0x77
	SPI.transfer(dots);
	digitalWrite(csPin, HIGH);
	
	return 1;
}

void SevenSegmentDisplay::clear()
{
	digitalWrite(csPin, LOW);
	SPI.transfer(ClearDisplay);  // 0x76 - Clear display command
	digitalWrite(csPin, HIGH);
}
void SevenSegmentDisplay::setBrightness(byte value)
{
  if(value != 118) //118 is 0x76 and it clear the display
  {
  	digitalWrite(csPin, LOW);
  	SPI.transfer(BrightnessControl);  	// 0x7A Set brightness command byte
  	SPI.transfer(value);  				// brightness data byte
  	digitalWrite(csPin, HIGH);
  }
  //TO-DO : what else - change value to 119/117/return error?
}
