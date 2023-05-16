#ifndef TSIO_H
#define TSIO_H

#include <xc.h>

unsigned char datoEnviar; //valor modificado en los otros tads con el valor a enviar al PC
__bit habilitaTX,habilitaSIO;

void InitSIO();
void motorSIO();

#endif