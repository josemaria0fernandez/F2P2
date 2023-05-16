#include "TTeclado.h"
#include <pic18f4321.h>
//#include "TMenu.h"

void InitTeclado(){
    teclado = TiGetTimer();
    tTecla = TiGetTimer();
    teclaPresionada = 0;
    presionada = -1;
  
}


void motorTeclado(void) {
	static char state = 0;
    
    if(TiGetTics(tTecla) >= UN_SEG && presionada != -1){
        presionada = -1;
        //newSMSMenu(*teclaActual);
      
    }
	switch(state) {
		case 0:
			if ((PORTB & 0x07)  == NOT_PRESSED) {
				filas++;
                F0 = 1;
				F1 = 0;
				
                state = 1;
			}
			else if ((PORTB & 0x07) != NOT_PRESSED) {
				TiResetTics(teclado);
				state = 4;
			}
            break;
		case 1:
			if ((PORTB & 0x07)  == NOT_PRESSED) {
				filas++;
                
				F1 = 1;
				F2 = 0;
                
				state = 2;
			}
			else if ((PORTB & 0x07) != NOT_PRESSED) {
				TiResetTics(teclado);
				state = 4;
			}
            break;
		case 2:
			if ((PORTB & 0x07) == NOT_PRESSED) {
				
                filas++;
                
				F2 = 1;
                F3 = 0;
				state = 3;
			}
			else if ((PORTB & 0x07) != NOT_PRESSED) {
				TiResetTics(teclado);
				state = 4;
			}
            break;
		case 3:
			if ((PORTB & 0x07) == NOT_PRESSED) {
				filas = 0;
                F0 = 0;
                F3 = 1;
                
				state = 0;
			}
			else if ( (PORTB & 0x07) != NOT_PRESSED) {
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
			if ((PORTB & 0x07) != NOT_PRESSED) {
				if(filas != 3){
                    if(teclaActual == ProcesaTecla(filas,(PORTB & 0x07),presionada)){
                        TiResetTics(tTecla);
                        presionada++;
                        teclaActual++;
                        if(*teclaActual == '-') {
                            presionada = 0;
                            teclaActual = teclaActual - 4;
                        } else if(presionada == 5){
                            presionada = 0;
                            teclaActual-=5;
                        }
                        
                        //smsActualizado(*teclaActual);
                    } else {
                        if(presionada != -1) {
                            //newSMSMenu(*teclaActual);
                        }
                        presionada = 0;
                        teclaActual = ProcesaTecla(filas,(PORTB & 0x07),presionada);
                        //smsActualizado(*teclaActual);
                        TiResetTics(tTecla);
                    }
                    
                } else{
                    
                    if((PORTB & 0x07) == 3){
                        //newCharMenu('#');
                        if(presionada != -1){
                                //newSMSMenu(*teclaActual);                             
                        }
                        
                    } else if((PORTB & 0x07) == 6){
                        //newCharMenu('*');
                        if(presionada != -1){
                                //newSMSMenu(*teclaActual);                             
                        }
                    } else if((PORTB & 0x07) == 5) {
                        
                        if(*teclaActual == zeros[presionada]){
                            presionada++;
                            presionada = presionada & 0x01;
                            //newCharMenu('0');
                            teclaActual = &zeros[presionada];
                            //smsActualizado(*teclaActual);
                        } else {        
                            if(presionada != -1){
                                //newSMSMenu(*teclaActual);                             
                            }
                            presionada = 0;
                            //newCharMenu('0');
                            teclaActual = &zeros[presionada];
                            //smsActualizado(*teclaActual);
                        }
                    }
                     
                    TiResetTics(tTecla);
                }
                state = 6;
			}
			else if ((PORTB & 0x07) == NOT_PRESSED) {
				F0 = 0;
				F1 = 1;
				F2 = 1;
				F3 = 1;
				state = 0;
                
			}
            break;
		case 6:
			if ((PORTB & 0x07) == NOT_PRESSED) {
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
			if ((PORTB & 0x07) != NOT_PRESSED) {
				state = 6;
			}
			else if ((PORTB & 0x07) == NOT_PRESSED) {
             
                F0 = 0;
				F1 = 1;
				F2 = 1;
				F3 = 1;
				state = 0;
			}
            break;
	}
}

unsigned char* ProcesaTecla(unsigned char filas,unsigned char columnas,signed char presionada){
    unsigned char *keyPointer;
    unsigned char columnActual = columnas;
    if(columnas == 3) columnActual++;
    teclaPresionada = (6-columnActual)+3*filas;
    keyPointer = &teclas[teclaPresionada][presionada];
    teclaPresionada = teclaPresionada + 1 + '0';
    //newCharMenu(teclaPresionada);
    return keyPointer;
}

