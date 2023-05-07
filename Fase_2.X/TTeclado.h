#ifndef TTECLADO_H
#define TTECLADO_H

#include "TITIMER.h"
#include <xc.h>

#define Columnas (PORTB & 0x07) 
#define Fila0 LATCbits.LATC0
#define Fila1 LATCbits.LATC1
#define Fila2 LATCbits.LATC2
#define Fila3 LATCbits.LATC3
#define NO_PRESIONADO 0x07

#define TREBOTES 0x20
#define ONE_SEC 2400



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
static unsigned char keyPressed;
static unsigned char teclado;
static unsigned int  tiempoTecla;
static unsigned char filas;
static unsigned char *keyActual;
static signed char pushed;

void motorTeclado(void);
unsigned char* ProcesaTecla(unsigned char filas,unsigned char columnas, signed char pushed);


#endif 