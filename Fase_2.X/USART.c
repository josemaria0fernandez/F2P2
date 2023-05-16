/*
 * File:   USART.c
 * Author: Noctis
 *
 * Created on 15 de mayo de 2023, 16:42
 */

#include "USART.h"
#include <xc.h>

void USART_Init(void){
    TXSTA   &= 0x03;
    RCSTA   &= 0x29;
    BAUDCON &= 0x44;
    PIE1    &= 0xCF;//IE
    PIR1    &= 0xFF;//Contains the flag
    IPR1    &= 0xFF;
    
    TXSTA   |= 0x24;
    RCSTA   |= 0x90;
    BAUDCON |= 0x08;
    PIE1    |= 0x30;
    SPBRGH = BAUDRATE_HIGH;
    SPBRG  = BAUDRATE_LOW;
}

bool USART_RXInterrupt(void){
    return ((bool)(PIR1>>RX_PIE_POS)&BOOL_MSK);
}   

bool USART_TXInterrupt(void){
    return ((bool)(PIR1>>TX_PIE_POS)&BOOL_MSK);
}