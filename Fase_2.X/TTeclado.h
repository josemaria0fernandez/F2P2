#ifndef TTECLADO_H
#define TTECLADO_H

#include "TTIMER.h"
#include <xc.h>

#define Columnas (PORTB & 0x07) 
#define F0 LATCbits.LATC0
#define F1 LATCbits.LATC1
#define F2 LATCbits.LATC2
#define F3 LATCbits.LATC3
#define NOT_PRESSED 0x07

#define TREBOTES 0x20
#define UN_SEG 2400



 unsigned char teclas[9][5] = {"1111-",
                               "ABC2-",
                               "DEF3-",
                               "GHI4-",
                               "JKL5-",
                               "MNO6-",
                               "PQRS7",
                               "TUV8-",
                               "WXYZ9"};

const unsigned char zeros[2] = "0 ";
static unsigned char teclaPresionada;
static unsigned char teclado;
static unsigned int  tTecla;
static unsigned char filas;
static unsigned char *teclaActual;
static signed char presionada;

void motorTeclado(void);
unsigned char* ProcesaTecla(unsigned char filas,unsigned char columnas, signed char pushed);


#endif 