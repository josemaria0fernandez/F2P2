#ifndef LCTLCD_H
#define	LCTLCD_H
//
// TAD per a manipular un display alfanumËric basat en el
// controlador HD44780 usant nomÈs 4 bits de dades.
// Aquest Ès el controlador que porten la  immensa majoria de displays.
// Tamany m?xim, 4 files per 40 columnes
//
// F. Escudero vCli v1.0 Piera, gener de 2004
//
// He provat aquest TAD amb un LCD de 2x16. Qualsevol error que observeu,
// us agraÔria que m'ho fÈssiu saber a sisco@salleurl.edu.
//
// Vcli V1.1, Sisco, a 26 de novembre de 2004. He vist que en alguns LCDs cal esperar 2ms
// desprÈs de fer un Clear, amb independËncia del que digui el Busy.
// Sembla extrany, quan tingui temps haig de mirar el bit de Busy amb l'oscil.loscop
// De moment, retardo amb timer.
//
// VCli V1.3, jnavarro, a 2013. He ampliat els temps d'inicialització (ara trigo uns
// 150 ms. però inicialitzo a la primera (sobretot en cold starts). Més info aquÌ:
// http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html
// Segueixo observant
// la mateixa anomalia del Busy, menys mal del timeout perquè sempre que li ho pregunto
// el fastigòs busy està 1...

//
// Durant el procÈs d'inicialització, es demanar? un timer al tad timer
//
// ------------------------------------HARDWARE---AREA--------------------
// La connexió serà de 4 bits de dades (D4 a D7), i els senyals
// RS, R/W i E.
// En aquest exemple, tenim connectat
//
// 	RS				a RB3
//	R/!W            a RB15
//  E				a RB5
//	D4				a RB6
//	D5				a RB7
//	D6				a RB8
//	D7				a RB9
//  ?s important que els senyals D0..D3 del LCD no es deixin a l'aire i
//  es connectin a GND mitjanÁant resistËncies de, per exemple, 4K7
//
// 	Es pot usar qualsevol altre configuraciÛ. NomÈs
//  cal ajustar els defines que venen a continuaciÛ.
//

#include <xc.h>


#define SetD4_D7Sortida()		(TRISDbits.TRISD0 = TRISDbits.TRISD1 = TRISDbits.TRISD2 = TRISDbits.TRISD3 = 0)
#define SetD4_D7Entrada()		(TRISDbits.TRISD0 = TRISDbits.TRISD1 = TRISDbits.TRISD2 = TRISDbits.TRISD3 = 1)
#define SetControlsSortida()    (TRISDbits.TRISD4 = TRISDbits.TRISD5 = TRISDbits.TRISD6 = 0)
#define SetD4(On)				(LATDbits.LATD0 = (On))
#define SetD5(On)				(LATDbits.LATD1 = (On))
#define SetD6(On)				(LATDbits.LATD2 = (On))
#define SetD7(On)				(LATDbits.LATD3 = (On))
#define GetBusyFlag()           (PORTDbits.RD3)
#define RSUp()					(LATDbits.LATD4 = 1)
#define RSDown()				(LATDbits.LATD4 = 0)
#define RWUp()					(LATDbits.LATD5 = 1)
#define RWDown()				(LATDbits.LATD5 = 0)
#define EnableUp()				(LATDbits.LATD6 = 1)
#define EnableDown()            (LATDbits.LATD6 = 0)
// -------------------------------END--HARDWARE---AREA--------------------
#define MAXCOLUMNAS 16

#define NO_MARQUESINA 0
#define SUPERIOR 1
#define INFERIOR -1
#define USERS 2
static unsigned char *cadenaEscritura[4];
static signed char marquesina; 
static unsigned char countImp, countFlecha;
static unsigned char posFila, posInicio,cadenaActual;
static unsigned int tLCD;
static __bit StopLCD, finCadena0, finCadena2, StopUsers;
__bit inMenu, finLCD;

void LcInit(char Files, char Columnes);
// Pre: Files = {1, 2, 4}  Columnes = {8, 16, 20, 24, 32, 40 }
// Pre: Hi ha un timer lliure
// Post: L'Hitachi merdÛs necessita 40ms de tranquilitat desde
// la pujada de Vcc fins cridar aquest constructor
// Post: Aquesta rutina pot trigar fins a 100ms
// Post: El display queda esborrat, el cursor apagat i a la
// posiciÛ 0, 0.

void LcEnd(void);
// El Destructor

void LcClear(void);
// Post: Esborra el display i posa el cursor a la posiciÛ zero en
// l'estat en el que estava.
// Post: La propera ordre pot trigar fins a 1.6ms

void LcCursorOn(void);
// Post: EncÈn el cursor
// Post: La propera ordre pot trigar fins a 40us

void LcCursorOff(void);
// Post: Apaga 0el cursor
// Post: La propera ordre pot trigar fins a 40us

void LcGotoXY(char Columna, char Fila);
// Pre : Columna entre 0 i 39, Fila entre 0 i 3
// Post: Posiciona el cursor en aquestes coordenades
// Post: La propera ordre pot trigar fins a 40us

void LcPutChar(char c);
// Post: Pinta C en l'actual posciciÛ del cursor i incrementa
// la seva posiciÛ. Si la columna arriba a 39, salta a 0 tot
// incrementant la fila si el LCD Ès de dues files.
// Si es de 4 files, incrementa de fila en arribar a la columna 20
// AixÌ mateix, la fila 4 passa a la zero.
// En els LCDs d'una fila, quan la columna arriba a 39, torna
// a zero. No s'incrementa mai la fila

void LcPutString(char *s);
// Post: Pinta l'string a apartir de la posiciÛ actual del cursor.
// El criteri de coordenades Ès el mateix que a LcPutChar
// Post: Pot trigar fins a 40us pel nombre de chars de s a sortir de
// la rutina


void motorLCD(void);

void LcPutMyString(unsigned char *primeraCadena,unsigned char  *segundaCadena, unsigned char  *terceraCadena, unsigned char  *cuartaCadena);

void stopWriting();

void startMarquesina(signed char estadoMarquesina);

__bit LcActMarquesina();

void LcPutCharSMS(char c);
#endif	/* LCTLCD_H */

