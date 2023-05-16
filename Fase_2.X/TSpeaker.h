/* Los números en un teclado numérico de teléfono antiguo suenan a una frecuencia de tono de DTMF (Dual-Tone Multi-Frequency) 
 * asociada con cada número. Estas frecuencias son las siguientes:

1: 1209 Hz
2: 1336 Hz
3: 1477 Hz
4: 1633 Hz
5: 697 Hz
6: 770 Hz
7: 852 Hz
8: 941 Hz
9: 1030 Hz
0: 941 Hz
 * 
1: 697 Hz
2: 770 Hz
3: 852 Hz
4: 941 Hz
5: 1209 Hz
6: 1336 Hz
7: 1477 Hz
8: 1633 Hz
9: 941 Hz 
0: 941 Hz
 */

#ifndef TSPEAKER_H
#define TSPEAKER_H


#include "TTIMER.h"
#include <xc.h>

#define MAXNOTAS 10
#define SPEAKER LATAbits.LATA5
#define DURACION_NOTA 1200
#define SILENCIO_NOTA 1200


static char timerPeriodo;
static int timerNota;
static char notaActual; 
__bit speakerEnable;

void motorSpeaker(void);
void InitSpeaker(void);



#endif