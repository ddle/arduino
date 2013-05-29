/*
 * arduino library for QRD1114 reflectance sensor
 * Dung Le, 2013
 */
#include <Arduino.h>

#include "QRD1114.h"
static int analog_pin = 0;

/*
 * get the current ADC reading  
 */
unsigned int QRD1114_read()
{
	return analogRead(analog_pin);
}
/*
 * set the desired analog input pin (0-5)
 */
void QRD1114_init(int input)
{
	analog_pin = input;
}
/*
 * This function reads the current sensor reading and tells what range it is.
 * The current design supports 3 different ranges of reflectance: BRIGHT - DIM - DARK, corresponding 
 * to the range of 0-1024 (10 bit ADC). 
 */
reflective_t QRD1114_get_refectance(unsigned int dark_threshold, unsigned int bright_threshold, unsigned int dim_threshold)
{
	unsigned int current = QRD1114_read();
	if (current < bright_threshold)
		return BRIGHT;
	else if (current < dim_threshold)
		return DIM;
	else
		return DARK;
}


