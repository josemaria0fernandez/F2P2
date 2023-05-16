//
// TAD per a manipular un display alfanum?ric basat en el
// controlador HD44780. Aquest ?s el controlador que porten la
// immensa majoria de displays. Tamany m?xim, 4 files per 40 columnes
//
// Durant el proc?s d'inicialitzaci?, es demanar? un timer al tad timer
// que no s'usar? per a res m?s. El constructor alliberar? aquest timer.

// Vcli V1.1, Sisco, a 26 de novembre de 2004. He vist que en alguns LCDs cal esperar
// despr?s de fer un Clear, amb independ?ncia del que digui el Busy.
// Sembla extrany, quan tingui temps haig de mirar el bit de Busy amb l'oscil.loscop
// De moment, rertardo amb timer.

// VCli V1.3, jnavarro, a 2013. He ampliat els temps d'inicialitzaci? (ara trigo uns
// 150 ms. per? inicialitzo a la primera (sobretot en cold starts). M?s info aqu?:
// http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
// Segueixo observant
// la mateixa anomalia del Busy, menys mal del timeout perqu? sempre que li ho pregunto
// el fastig?s busy est? a 1...

#include <xc.h>
#include "TITIMER.h"
#include "TLCD.h"
#include <pic18f4321.h>

//
//--------------------------------CONSTANTS---AREA-----------
//
#define FUNCTION_SET	0x20
#define BITS_8			0x10
#define DISPLAY_CONTROL	0x08
#define DISPLAY_ON		0x04
#define CURSOR_ON		0x02
#define DISPLAY_CLEAR	0x01
#define ENTRY_MODE		0x04
#define SET_DDRAM		0x80
//
//---------------------------End--CONSTANTS---AREA-----------
//


//
//--------------------------------VARIABLES---AREA-----------
//
static unsigned char Files, Columnes;
static unsigned char FilaAct, ColumnaAct;
static int Timer;

//
//---------------------------End--VARIABLES---AREA-----------
//

//
//--------------------------------PROTOTIPUS--AREA-----------
//
void Espera(int Timer, int ms);
void CantaIR(char IR);
void CantaData(char Data);
void WaitForBusy(void);
void EscriuPrimeraOrdre(char);

//
//---------------------------End--PROTOTIPUS--AREA-----------
//


//
//--------------------------------PUBLIQUES---AREA-----------
//
void LcInit(char files, char columnes) {
// Pre: Files = {1, 2, 4}  Columnes = {8, 16, 20, 24, 32, 40 }
// Pre: L'Hitachi merd?s necessita 40ms de tranquilitat desde
// la pujada de Vcc fins cridar aquest constructor
// Pre: Hi ha un timer lliure
// Post: Aquesta rutina pot trigar fins a 100ms
// Post: El display queda esborrat, el cursor apagat i a la
// posici? 0, 0.
    
   
    cadenaActual = 0;
    posInicio = 0;
    posFila = 0;
    marquesina = 0;
    
    tLCD = TiGetTimer();
    countImp = 0;
    StopLCD = 0;
    StopUsers = 0;
    finCadena0 = 0;
    finCadena2 = 0;
   
    countFlecha = 0;
    finLCD = 0;
	int i;
	Timer = TiGetTimer(); // Val m?s que n'hi hagi
	Files = files; Columnes = columnes;
	FilaAct = ColumnaAct = 0;
	SetControlsSortida();
	for (i = 0; i < 2; i++) {
		Espera(Timer, 200);
		// El manual de l'hitachi diu que s'ha de fer aquesta
		// seq??ncia...

		EscriuPrimeraOrdre(CURSOR_ON | DISPLAY_CLEAR);
		Espera(Timer, 10);
		EscriuPrimeraOrdre(CURSOR_ON | DISPLAY_CLEAR);
		Espera(Timer, 2);
		EscriuPrimeraOrdre(CURSOR_ON | DISPLAY_CLEAR);
		Espera(Timer, 2);
		// .. tres vegades. Potser ?s una penit?ncia?
		// Ara una a 4 bits
		EscriuPrimeraOrdre(CURSOR_ON);
		Espera(Timer, 2);
		CantaIR(FUNCTION_SET | DISPLAY_CONTROL); 	// 4bits, 1 files, font 5x7
		// Observo que just aqu? s'esborra la primera linia
		// Ara ja podem mirar el busy
		WaitForBusy(); 	CantaIR(DISPLAY_CONTROL);  	// Display Off
		WaitForBusy(); 	CantaIR(DISPLAY_CLEAR);	   	// Tot espais
		Espera(Timer,6); // 1.64ms V1.1
		WaitForBusy(); 	CantaIR(DISPLAY_ON | CURSOR_ON); // Auto Increment i shift
		WaitForBusy(); 	CantaIR(DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON | DISPLAY_CLEAR); 		// Display On
	}
	// El manual dir? el que vulgui per? jo he vist que despr?s
	// d'una arrencada freda, no sempre s'inicialitza correctament
	// tot i esperar els 40ms. Per aix?, he posat un bucle de dues
	// inicialitzacions i a partir d'aqu?, la inicialitzaci? m'ha funcionat
	// correctament tant si faig un reset com si apago i engego l'alimentaci?.
    
}


void LcClear(void) {
// Post: Esborra el display i posa el cursor a la posici? zero en
// l'estat en el que estava.
// Post: La propera ordre pot trigar fins a 1.6ms
	WaitForBusy(); 	CantaIR(DISPLAY_CLEAR);	   // Tot espais
	Espera(Timer, 6); // V1.1
}

void LcCursorOn(void) {
// Post: Enc?n el cursor
// Post: La propera ordre pot trigar fins a 40us
	WaitForBusy();
	CantaIR(DISPLAY_CONTROL | DISPLAY_ON | CURSOR_ON);
}

void LcCursorOff(void) {
// Post: Apaga 0el cursor
// Post: La propera ordre pot trigar fins a 40us
	WaitForBusy();
	CantaIR(DISPLAY_CONTROL | DISPLAY_ON);
}



void LcGotoXY(char Columna, char Fila) {
// Pre : Columna entre 0 i 39, Fila entre 0 i 3
// Post: Posiciona el cursor en aquestes coordenades
// Post: La propera ordre pot trigar fins a 40us
	int Fisica;
	// d'entrada calculo la direcci? efectiva de la ram del LCD
	
	Fisica = Columna + (!Fila ? 0 : 0x40); 
		
	
	// i l'aplico amb la comanda
	WaitForBusy();
	CantaIR(SET_DDRAM | Fisica);
	// Finalment, actualitzo les imatges locals
	FilaAct    = Fila;
	ColumnaAct = Columna;
}

void LcPutChar(char c) {
// Post: Pinta C en l'actual poscici? del cursor i incrementa
// la seva posici?. Si la columna arriba a 39, salta a 0 tot
// incrementant la fila si el LCD ?s de dues files.
// Si es de 4 files, incrementa de fila en arribar a la columna 20
// Aix? mateix, la fila 4 passa a la zero.
// En els LCDs d'una fila, quan la columna arriba a 39, torna
// a zero. No s'incrementa mai la fila
	// D'entrada escric el char
	WaitForBusy(); CantaData(c);
	// i ara recalculo la posici? del cursor
	++ColumnaAct;
	
		if (ColumnaAct >= 60) {
			ColumnaAct = 0;
			if (++FilaAct >= 2) FilaAct = 0;
			LcGotoXY(ColumnaAct, FilaAct);
		}
}


//
//---------------------------End--PUBLIQUES---AREA-----------
//

//
//--------------------------------PRIVADES----AREA-----------
//

void Espera(int Timer, int ms) {
	TiResetTics(Timer);
	while(TiGetTics(Timer) < ms);
}

void CantaPartAlta(char c) {
	 SetD7(c & 0x80 ? 1 : 0);
	 SetD6(c & 0x40 ? 1 : 0);
	 SetD5(c & 0x20 ? 1 : 0);
	 SetD4(c & 0x10 ? 1 : 0);
}

void CantaPartBaixa(char c) {
	 SetD7(c & 0x08 ? 1 : 0);
	 SetD6(c & 0x04 ? 1 : 0);
	 SetD5(c & 0x02 ? 1 : 0);
	 SetD4(c & 0x01 ? 1 : 0);
}

void CantaIR(char IR) {
	SetD4_D7Sortida();
	RSDown();
	RWDown();
	EnableUp();
	CantaPartAlta(IR); 		// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	EnableDown();
	EnableUp();
	CantaPartBaixa(IR); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	SetD4_D7Entrada();
}

void CantaData(char Data) {
	SetD4_D7Sortida();
	RSUp();
	RWDown();
	EnableUp();
	CantaPartAlta(Data); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	EnableDown();
	EnableUp();
	CantaPartBaixa(Data); 	// Segons hitachi Data Setup = 80ns (cap problema)
	EnableUp();				// Asseguro els 500ns de durada de pols
	EnableDown();   		// i l'amplada del pols "enable" major que 230n
	SetD4_D7Entrada();
}

void WaitForBusy(void) { char Busy;
	SetD4_D7Entrada();
	RSDown();
	RWUp();
	TiResetTics(Timer);
	do {
		EnableUp();EnableUp(); // Asseguro els 500ns de durada de pols
		Busy = GetBusyFlag();
		EnableDown();
		EnableDown();
		EnableUp();EnableUp();
		// Aqu? ve la part baixa del comptador d'adreces, que no ens interessa
		EnableDown();
		EnableDown();
		if (TiGetTics(Timer)) break; // M?s d'un ms vol dir quel LCD est? boig
	} while(Busy);
}

void EscriuPrimeraOrdre(char ordre) {
	// Escric el primer com si fossin 8 bits
	SetD4_D7Sortida();  RSDown(); RWDown();
	EnableUp(); EnableUp();
	 SetD7(ordre & 0x08 ? 1 : 0);
	 SetD6(ordre & 0x04 ? 1 : 0);
	 SetD5(ordre & 0x02 ? 1 : 0);
	 SetD4(ordre & 0x01 ? 1 : 0);
	EnableDown();
}

static char state = 0;
void motorLCD(void) {
	
    
	switch(state) {
		case 0:
			if (marquesina == NO_MARQUESINA) {
				posFila = 0;
                
				state = 1;
			}
            if (marquesina == SUPERIOR) {
                if(cadenaActual == 0) {
                    posFila = posInicio;
                } else {
                    posFila = 0;
                }
                state = 1;
            }
            if (marquesina == INFERIOR) {
                if(cadenaActual == 2) {
                    posFila = posInicio;
                } else {
                    posFila = 0;
                }
                state = 1;
            }
            if(marquesina == USERS){
                if(cadenaActual == 0 || cadenaActual == 2) {
                    posFila = posInicio;
                } else {
                    posFila = 0;
                }
                state = 4;
            }
		break;
		case 1:
 			if (++countImp <= MAXCOLUMNAS && cadenaEscritura[cadenaActual][posFila] != '\0') {
                LcPutChar(cadenaEscritura[cadenaActual][posFila]); 
                posFila++; 
			}
			else if (cadenaEscritura[cadenaActual][posFila] == '\0' && marquesina != USERS) {
				cadenaActual++;
				state = 2;
			}
			else if (countImp > MAXCOLUMNAS && marquesina != USERS) {
				
                if(cadenaActual < 2) {
                    
                    marquesina = SUPERIOR;
                } else {
                    marquesina = INFERIOR;
                }
                cadenaActual++;
                state = 2;
			}
            else if(cadenaEscritura[cadenaActual][posFila] == '\0' && marquesina == USERS) {
                cadenaActual++;
                state = 11;
            }
            
		break;
		case 2:
			if (cadenaActual < 4) {
                countImp = 0;
				if(cadenaActual == 2)LcGotoXY(0,1);
                state = 0;
			}
            else if (cadenaActual == 4 && ((!StopLCD || cadenaEscritura[3][posFila] != '\0'))){
                
                countImp = 0;
                TiResetTics(tLCD);
                cadenaActual = 0;
                state = 3;
            } else if (cadenaActual == 4 &&  cadenaEscritura[3][posFila] == '\0'){
                finLCD = 1;
            }
            
            
		break;
		case 3:
            if(TiGetTics(tLCD) >= 1000 && marquesina == SUPERIOR){
                posInicio++;
                if(cadenaEscritura[0][posInicio] == '\0'){
                    posInicio = 0;
                }
                LcGotoXY(0,0);
                countImp = 0;
                state = 0;
            }
            
            if(TiGetTics(tLCD) >= 1000 && marquesina == INFERIOR){
                posInicio++;
                if(cadenaEscritura[2][posInicio] == '\0'){
                    posInicio = 0;
                }
                LcGotoXY(0,0);
                countImp = 0;
                state = 0;
            }
            if(TiGetTics(tLCD) >= 40 && marquesina == NO_MARQUESINA) {
                posInicio = 0;
                LcGotoXY(0,0);
                state = 0;
            }
            
            if(marquesina == USERS){
                
                if(TiGetTics(tLCD) >= 1000 && posInicio != 0 || TiGetTics(tLCD) >= 5000  && posInicio == 0 ){
                    posInicio++;
                    if(posInicio == 18){
                        StopUsers = 1;
                    }
                    if(cadenaEscritura[0][posInicio] == '\0'){
                        finCadena0 = 1;
                    
                    }
                    if(cadenaEscritura[2][posInicio] == '\0'){
                        finCadena2 = 1;
                    
                    }
                    countImp = 0;
                    LcGotoXY(0,0);
                    state = 0; 
                }    
            }
            finLCD = 0;
		break;
        case 4:
            if(finCadena0 && cadenaActual == 0 || finCadena2 && cadenaActual == 2 || finCadena2 && cadenaActual == 2 && finCadena0 && cadenaActual == 0) {
                cadenaActual++;
                state = 11;
            } else {
                state = 1;
            }
        break;
        
       
        case 11:
            if(countImp <= MAXCOLUMNAS && posFila <= MAXCOLUMNAS){
                LcPutChar(' ');
                countImp++;
                posFila++;
            }
            if(countImp > MAXCOLUMNAS || posFila > MAXCOLUMNAS){
                state = 2;
            }
        break;
       
	}
}

void LcPutMyString(unsigned char *primeraCadena,unsigned char  *segundaCadena, unsigned char  *terceraCadena, unsigned char  *cuartaCadena){
    cadenaEscritura[0] = primeraCadena;
    cadenaEscritura[1] = segundaCadena;
    cadenaEscritura[2] = terceraCadena;
    cadenaEscritura[3] = cuartaCadena;
    cadenaActual = 0;
    posInicio = 0;
    posFila = 0;
    marquesina = 0;
    StopLCD = 0;
    finCadena0 = 0;
    finCadena2 = 0;
    countImp = 0;
    StopUsers = 0;
    finLCD = 0;
}

void stopWriting(){
    StopLCD = 1;
}

__bit LcActMarquesina(){
    return StopUsers;
}

void startMarquesina(signed char estadoMarquesina) {
    marquesina = estadoMarquesina;
    state = 0;
}

void LcPutCharSMS(char c){
    WaitForBusy(); CantaData(c);
    LcGotoXY(ColumnaAct, FilaAct);
}
//
//---------------------------End--PRIVADES----AREA-----------
//
