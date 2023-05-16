#include "TSIO.h"
#include <pic18f4321.h>



void InitSIO() {
    habilitaTX = 0;
    habilitaSIO = 0;
    TXSTA = 0x26;
    RCSTAbits.CREN = 1;  //eneble del receiver 
    RCSTAbits.SPEN = 1; //enable del serial port
    BAUDCONbits.BRG16 = 1;  //16-bit Baud Rate Generator ? SPBRGH and SPBRG
    SPBRGH = 0x04;
    SPBRG= 0x10;
}

void motorSIO() {
    
    static char state = 0;
    
    switch(state){
        case 0:
            if(habilitaTX){
                if (habilitaSIO == 1) {
                    TXREG = datoEnviar;  //datoEnviar se modifica en los otros tads y se le asigna la informacion a enviar. 
                }
                    state++;
            }
            break;
        case 1:
            if(TXSTAbits.TRMT == 1 || habilitaSIO == 0) {  //controlamos si el Transfer Register esta empty (1) o si se desabilita la SIO
                habilitaTX = 0;
                state--;
            }
            break;          
    }
}

