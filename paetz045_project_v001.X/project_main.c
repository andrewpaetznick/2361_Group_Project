/*
 * File:   project_main.c
 * Author: andre
 *
 * Created on November 18, 2025, 5:12 PM
 */


#include "xc.h"
#include "uart_lib.h"
#include "delay.h"

void setup(){
    AD1PCFG = 0xffff;   //all digital
    CLKDIVbits.RCDIV = 0;
    init_uart();
}

int main(void) {
    setup();
    send_command("");
    char ReceivedChar = 'n';
    while(1){
        if (U1STAbits.URXDA == 1) {
            ReceivedChar = U1RXREG;
        }
        
        arp_delay_1ms();

    }
    return 0;
}
