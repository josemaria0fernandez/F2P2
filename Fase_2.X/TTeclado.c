#include "TTeclado.h"
#include <pic18f4321.h>
//#include "TMenu.h"

void InitTeclado(){
    teclado = TiGetTimer();
    tiempoTecla = TiGetTimer();
    keyPressed = 0;
    pushed = -1;
  
}


void motorTeclado(void) {
	static char state = 0;
    
    if(TiGetTics(tiempoTecla) >= ONE_SEC && pushed != -1){
        pushed = -1;
        //newSMSMenu(*keyActual);
      
    }
	switch(state) {
		case 0:
			if ((PORTB & 0x07)  == NO_PRESIONADO) {
				filas++;
                Fila0 = 1;
				Fila1 = 0;
				
                state = 1;
			}
			else if ((PORTB & 0x07) != NO_PRESIONADO) {
				TiResetTics(teclado);
				state = 4;
			}
		break;
		case 1:
			if ((PORTB & 0x07)  == NO_PRESIONADO) {
				filas++;
                
				Fila1 = 1;
				Fila2 = 0;
                
				state = 2;
			}
			else if ((PORTB & 0x07) != NO_PRESIONADO) {
				TiResetTics(teclado);
				state = 4;
			}
		break;
		case 2:
			if ((PORTB & 0x07) == NO_PRESIONADO) {
				
                filas++;
                
				Fila2 = 1;
                Fila3 = 0;
				state = 3;
			}
			else if ((PORTB & 0x07) != NO_PRESIONADO) {
				TiResetTics(teclado);
				state = 4;
			}
		break;
		case 3:
			if ((PORTB & 0x07) == NO_PRESIONADO) {
				filas = 0;
                Fila0 = 0;
                Fila3 = 1;
                
				state = 0;
			}
			else if ( (PORTB & 0x07) != NO_PRESIONADO) {
				TiResetTics(teclado);
				state = 4;
			}
		break;
		case 4:
            if (TiGetTics(teclado) >= TREBOTES){
                
                state = 5;
            }
		break;
		case 5:
			if ((PORTB & 0x07) != NO_PRESIONADO) {
				if(filas != 3){
                    if(keyActual == ProcesaTecla(filas,(PORTB & 0x07),pushed)){
                        TiResetTics(tiempoTecla);
                        pushed++;
                        keyActual++;
                        if(*keyActual == '-') {
                            pushed = 0;
                            keyActual = keyActual - 4;
                        } else if(pushed == 5){
                            pushed = 0;
                            keyActual-=5;
                        }
                        
                        //smsActualizado(*keyActual);
                    } else {
                        if(pushed != -1) {
                            //newSMSMenu(*keyActual);
                        }
                        pushed = 0;
                        keyActual = ProcesaTecla(filas,(PORTB & 0x07),pushed);
                        //smsActualizado(*keyActual);
                        TiResetTics(tiempoTecla);
                    }
                    
                } else{
                    
                    if((PORTB & 0x07) == 3){
                        //newCharMenu('#');
                        if(pushed != -1){
                                //newSMSMenu(*keyActual);                             
                        }
                        
                    } else if((PORTB & 0x07) == 6){
                        //newCharMenu('*');
                        if(pushed != -1){
                                //newSMSMenu(*keyActual);                             
                        }
                    } else if((PORTB & 0x07) == 5) {
                        
                        if(*keyActual == zeros[pushed]){
                            pushed++;
                            pushed = pushed & 0x01;
                            //newCharMenu('0');
                            keyActual = &zeros[pushed];
                            //smsActualizado(*keyActual);
                        } else {        
                            if(pushed != -1){
                                //newSMSMenu(*keyActual);                             
                            }
                            pushed = 0;
                            //newCharMenu('0');
                            keyActual = &zeros[pushed];
                            //smsActualizado(*keyActual);
                        }
                    }
                     
                    TiResetTics(tiempoTecla);
                }
                state = 6;
			}
			else if ((PORTB & 0x07) == NO_PRESIONADO) {
				Fila0 = 0;
				Fila1 = 1;
				Fila2 = 1;
				Fila3 = 1;
				state = 0;
                
			}
		break;
		case 6:
			if ((PORTB & 0x07) == NO_PRESIONADO) {
				TiResetTics(teclado);
				state = 7;
			}
		break;
		case 7:
            
            
            if (TiGetTics(teclado) >= TREBOTES){
                
                state = 8;
            }
		
		break;
		case 8:
			if ((PORTB & 0x07) != NO_PRESIONADO) {
				state = 6;
			}
			else if ((PORTB & 0x07) == NO_PRESIONADO) {
             
                Fila0 = 0;
				Fila1 = 1;
				Fila2 = 1;
				Fila3 = 1;
				state = 0;
			}
		break;
	}
}

unsigned char* ProcesaTecla(unsigned char filas,unsigned  char columnas,signed char pushed){
    unsigned char *keyPointer;
    unsigned char columnActual = columnas;
     if(columnas == 3) columnActual++;
    keyPressed = (6-columnActual)+3*filas;
    keyPointer = &teclas[keyPressed][pushed];
    keyPressed = keyPressed +1 + '0';
    //newCharMenu(keyPressed);
    return keyPointer;
}

