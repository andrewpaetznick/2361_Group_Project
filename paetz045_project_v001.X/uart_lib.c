/*
 * File:   uart_lib.c
 * Author: andre
 *
 * Created on November 18, 2025, 5:13 PM
 */

#include "xc.h"
#include "delay.h"

void delay(int ms_delay);

void init_uart(){
    /**
     * RB6 is tx, RB10 rx
     * default of 9600 baud
     */
    //RB5 State Setup, pulled down on board by 10k
    TRISBbits.TRISB5 = 1;      // Set RB5 as input
    
    //PPS Setup copied from Slides
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _RP6R = 0x0003;   // RB6->UART1:U1TX
    _U1RXR = 10;   // RB10->UART1:U1RX
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    //UART Setup
    //PPS Stuff above here!
    U1MODEbits.UARTEN = 0;
    U1MODEbits.PDSEL = 0;   // No Parity, 8-data bits (i.e., 8N1)
    U1MODEbits.STSEL = 0;   // 1-stop bit (i.e., 8N1)
    U1MODEbits.BRGH = 0;    // Standard-Speed mode
    U1BRG = 103;            // Baud Rate setting for 9600

    U1MODEbits.UARTEN = 1;  // Enable UART
    U1STAbits.UTXEN = 1;    // Enable UART TX
    
    // Wait at least 500ms startup
    delay(500);
}

void send_str(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        //wait until last char is sent
        while (U1STAbits.UTXBF);
        U1TXREG = str[i];
    }
    while (!U1STAbits.TRMT);   // wait for shift reg to empty
}

void send_command(const char* command) {
    //all commands start with AT+ and are groups of chars
    send_str("AT+");
    send_str(command);
    //tells the chip the send is over
    send_str("\r\n");
}

void delay(int ms_delay) {
    //ms delay 
    for(int i = 0; i < ms_delay; i++) {
        arp_delay_1ms();
    }
}