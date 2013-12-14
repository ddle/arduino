#ifndef QRD1114_h
#define QRD1114_h

/*
 * arduino library for QRD1114 reflectance sensor
 */

#include <Arduino.h>

typedef enum { DARK, BRIGHT, DIM } reflectance_t;

unsigned int QRD1114_read();
void QRD1114_init( int input_pin );
reflective_t QRD1114_get_refectance(unsigned int dark_threshold, unsigned int bright_threshold, unsigned int dim_threshold);


#endif
