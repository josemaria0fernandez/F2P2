#ifndef TADC_H
#define TADC_H

#define VALUE_UP 250 
#define VALUE_DOWN 5
#define UP 'U'
#define DOWN 'D'
#include <xc.h>

static unsigned char JoyPosition;
signed char direction;
__bit isCentered;
//__bit isCentered, eje;


void InitADCON();
void motorADCON();
#endif

