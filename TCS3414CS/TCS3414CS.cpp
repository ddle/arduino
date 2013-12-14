/*
 * arduino library for TCS3414CS color sensor
 */

#include <Arduino.h>
#include "TCS3414CS.h"

/* global variables, accessible for this module only */ 
static color_t current_color = RED;
static unsigned int readingdata[20];
static unsigned int i,green,red,blue,clr,ctl;
static int integrationTime = 12;

void TCS3414CS_init()
{  	
	//setTimingReg(INTEG_MODE_FREE);//Set free mode
	setIntegrationTime(INTEGRATION_TIME_12ms);
	setGain(GAIN_1|PRESCALER_4);
	setEnableADC();//Start ADC of the color sensor
	delay(integrationTime);
}

// debug helper
void printRGB()
{
	Serial.println();
	Serial.print("red: "); 
	Serial.println(red,DEC);
	Serial.print("grean: "); 
	Serial.println(green,DEC);
	Serial.print("blue: "); 
	Serial.println(blue,DEC);
	Serial.print("clear: "); 
	Serial.println(clr,DEC); 
}
// debug helper
void printColor(color_t c)
{
	switch(c) 
	{
		case RED:
			Serial.println("RED");
			break;
		case GREEN:
			Serial.println("GREEN");
			break;
		case BLUE:
			Serial.println("BLUE");
			break;
		case ORANGE:
			Serial.println("ORANGE");
			break;
		case VIOLET:
			Serial.println("VIOLET");
			break;
		case WHITE:
			Serial.println("WHITE");
			break;	
		case CLEAR:
			//Serial.println("CLEAR");
			break;
	}
}

void TCS3414CS_test()
{
	TCS3414CS_init();
	while(1)
	{
		if(Serial.available())
		{
			char keystroke = Serial.read(); 
			readRGB();
			switch(keystroke)
			{
				case 's': // ascii "s" type in s in the serial monitor and "send" to arduino
					printRGB();							  
					break; 
				case 'c': // ascii "s" type in s in the serial monitor and "send" to arduino		 
					//current_color = getColor();	
					printColor(current_color);
					break; 
			}
			delay(integrationTime);
		}
	}   
}
   
/************************************/
/*
 * set of sensor's configuration functions 
 */
void setTimingReg(int x)
{
   Wire.beginTransmission(COLOR_SENSOR_ADDR);
   Wire.write(REG_TIMING);
   Wire.write(x);
   Wire.endTransmission();  
   //delay(100); 
}
void setIntegrationTime(int x)
{
	Wire.beginTransmission(COLOR_SENSOR_ADDR);
	Wire.write(REG_TIMING);
	Wire.write(x);
	Wire.endTransmission();  
	switch(x) 
	{
		case INTEGRATION_TIME_12ms:
			integrationTime = 12;
			break;
		case INTEGRATION_TIME_100ms:
			integrationTime = 100;
			break;
		case INTEGRATION_TIME_400ms:
			integrationTime = 400;
			break;
		default:
			integrationTime = 12;
			break;
	}
	//delay(100); 
}

void setGain(int x)
{
   Wire.beginTransmission(COLOR_SENSOR_ADDR);
   Wire.write(REG_GAIN);
   Wire.write(x);
   Wire.endTransmission();
}
void setEnableADC()
{
 
   Wire.beginTransmission(COLOR_SENSOR_ADDR);
   Wire.write(REG_CTL);
   Wire.write(CTL_DAT_INIITIATE);
   Wire.endTransmission();  
   //delay(100);  
}

/************************************/
/*
 * Read the current sensor reading and store to our current color bin.
 * An integration time is assumed (not included in this routine) between 
 * reading.
 */
void readRGB()
{
	Wire.beginTransmission(COLOR_SENSOR_ADDR);
	Wire.write(REG_BLOCK_READ);
	Wire.endTransmission();

	Wire.beginTransmission(COLOR_SENSOR_ADDR);
	Wire.requestFrom(COLOR_SENSOR_ADDR, 8); // 8-byte packet
	//delay(500);
	if(8 <= Wire.available())   
	{ 
		for(i=0;i<8;i++)
		{
			readingdata[i]=Wire.read();
		}
	}
	green=readingdata[1]*256+readingdata[0];
	red=readingdata[3]*256+readingdata[2];
	blue=readingdata[5]*256+readingdata[4];
	clr=readingdata[7]*256+readingdata[6];	
}
/*
 * determine color using the clear channel (no filter).
 * Color order:
 * violet	668–789 THz	380–450 nm
 * blue		606–668 THz	450–495 nm
 * green	526–606 THz	495–570 nm
 * yellow	508–526 THz	570–590 nm
 * orange	484–508 THz	590–620 nm
 * red		400–484 THz	620–750 nm
 * 
 * return color type of red, green, blue, orange, violet
 */
color_t getColor(unsigned int r, unsigned int o, unsigned int g, unsigned int b, unsigned int v)
{	
	readRGB();
	// determine color here
	if (clr < r)
		return RED;
	else if (clr < o)
		return ORANGE;
	else if (clr < g)
		return GREEN;
	else if (clr < b)
		return BLUE;
	else 
		return VIOLET;
}
/*
 * Use single threshold for each color (red, green, blue)
 * return only color type of red, green, blue
 */
color_t getColorSingle(unsigned int r, unsigned int g, unsigned int b)
{
	readRGB();	
	
	// determine color here
	/* ALGORITHM I */
	if (red > r)
		return RED;
	if (green > g)
		return GREEN;
	if (blue > b)
		return BLUE;	
				
}
/*
 * This provides ability to identify combinational colors along with the basics
 * ( RED, GREEN, BLUE).
 */
color_t getColorCombo(unsigned int r, unsigned int g, unsigned int b)
{
	int r_count, g_count, b_count = 0;
	readRGB();	
	
	// determine color here			
	/* ALGORITHM II */
	if (red > r)
		r_count = 1;
	if (green > g)
		g_count = 1;
	if (blue > b)
		b_count = 1;	
	// use this for adjusting outcome
	Serial.print("r:");
	Serial.println(r_count);
	Serial.print("g:");
	Serial.println(r_count);
	Serial.print("b:");
	Serial.println(b_count);
	//if (r_count + g_count
	
	// algorithm incompleted
	return WHITE;	
}

/************************************/
/* get current bin info */
unsigned int getRed()
{
	return red;
}
unsigned int getGreen()
{
	return green;
}

unsigned int getBlue()
{
	return blue;
}

int getIntegrationTime()
{
	return integrationTime;
}
