#include "TADC.h"
#include <pic18f4321.h>
//#include "TSIO.h"

void InitADCON(){
    direction = 0;
    JoyPosition = 128;
    isCentered = 1;
    
}


void motorADCON(){
    static char state = 0;
    
    switch(state){
        case 0:
            ADCON0bits.CHS0 = 1;    //el canal donde esta el eje Y del Joystick 
            ADCON0bits.GO_DONE = 1;
            state++;
            break;
        case 1:
            if(ADCON0bits.GO_DONE == 0) {
                JoyPosition = ADRESH;
                state++;
            }
            break;
        case 2:
            if(JoyPosition > VALUE_DOWN && JoyPosition < VALUE_UP ){
                isCentered = 1;
            }
            if(isCentered && (JoyPosition < VALUE_DOWN || JoyPosition > VALUE_UP)) {
                isCentered = 0;
                if(JoyPosition > VALUE_UP) direction = UP;
                if(JoyPosition < VALUE_DOWN) direction = DOWN;
                
                //if(habilitaTX == 0) {
                //    datoEnviar = direction;
                //    habilitaTX = 1;
                //}
            }
            if(!isCentered && (JoyPosition < DOWN || JoyPosition > UP)){
                state = 0;
            }
            state = 0;
            break;  
    }
}