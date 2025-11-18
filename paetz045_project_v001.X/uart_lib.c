/*
 * File:   uart_lib.c
 * Author: andre
 *
 * Created on November 18, 2025, 5:13 PM
 */

#include "xc.h"
#include "delay.h"


void init_uart(){
    /**
     * RB6 is tx, RB10 rx
     * default of 9600 baud
     */
    
    //PPS Setup copied from Slides
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _RP6R = 0x0003;   // RB6->UART1:U1TX
    _U1RXR = 10;   // RB10->UART1:U1RX
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    //UART Setup
    //PPS Stuff above here!
    U1MODEbits.PDSEL = 0;   // No Parity, 8-data bits (i.e., 8N1)
    U1MODEbits.STSEL = 0;   // 1-stop bit (i.e., 8N1)
    U1MODEbits.BRGH = 0;    // Standard-Speed mode
    U1BRG = 103;            // Baud Rate setting for 9600

    U1MODEbits.UARTEN = 1;  // Enable UART
    U1STAbits.UTXEN = 1;    // Enable UART TX
    
    // Wait at least 105us = (1/9600)
    arp_delay_100us();
    arp_delay_100us();

    U1TXREG = 'a';            // Transmit one character
}
