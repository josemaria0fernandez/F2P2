#include "TSpeaker.h"
#include <pic18f4321.h>



void motorSpeaker(void) {
	static char state = 0;

	switch(state) {
		case 0:
			if (speakerEnable != 0 ) {
				TiResetTics(timerNota);
				TiResetTics(timerPeriodo);
				SPEAKER = 0;
				state = 1;
			}
		break;
		case 1:
			if (TiGetTics(timerPeriodo) >= notaActual) {
				TiResetTics(timerPeriodo);
                SPEAKER = 1;
				state = 2;
			}
            else if (TiGetTics(timerNota) >=  DURACION_NOTA) {
				SPEAKER = 0;
				TiResetTics(timerNota);
				notaActual+=2;
				if(notaActual == 12){
                    notaActual = 2;
                    speakerEnable = 0;
                }
				state = 3;
			}
		break;
		case 2:
			if (TiGetTics(timerPeriodo) >= notaActual) {
				TiResetTics(timerPeriodo);
                SPEAKER = 0;
				state = 1;
			} 
		break;
		case 3:
			if (TiGetTics(timerNota) >= SILENCIO_NOTA) {
				state = 0;
			}
		break;
	}
}


void InitSpeaker(){
    notaActual = 2;
    SPEAKER = 0;
    timerNota = TiGetTimer();
    timerPeriodo = TiGetTimer();
    speakerEnable = 0;
    
}

void enableSpeaker() {
    speakerEnable = 1;
}

void disableSpeaker() {
    speakerEnable = 0;
}