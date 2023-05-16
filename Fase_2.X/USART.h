 
#ifndef USART_H
#define	USART_H

#include <xc.h> 
#include <stdint.h>
#include <stdbool.h>

#define BAUDRATE_HIGH   0x00
#define BAUDRATE_LOW    0x81
#define TX_PIE_POS      4
#define RX_PIE_POS      5
#define BOOL_MSK        0x01


void USART_Init(void);

bool USART_RXInterrupt(void);

bool USART_TXInterrupt(void);

uint8_t USART_GetRx(void);

void USART_SetTx(uint8_t data);

#endif