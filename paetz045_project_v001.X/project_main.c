/*
 * File:   project_main.c
 * Author: andre
 *
 */

//different config bits, FCKSM = CSECME and JTAGEN = 0
// CONFIG2
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = ON             // IOLOCK Protection (Once IOLOCK is set, cannot be changed)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSECME           // Clock Switching and Monitor (Clock switching is enabled, Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCDIV           // Oscillator Select (Fast RC Oscillator with Postscaler (FRCDIV))
#pragma config SOSCSEL = SOSC           // Sec Oscillator Select (Default Secondary Oscillator (SOSC))
#pragma config WUTSEL = LEG             // Wake-up timer Select (Legacy Wake-up Timer)
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config ICS = PGx1               // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

#include "xc.h"
#include <stdio.h>

#include "uart_lib.h"

void setup();
void loop();

int main(void) {
    setup();
    loop();
    return 0;
}

void setup(){
    //pin setup
    AD1PCFG = 0xffff;   //all digital
    CLKDIVbits.RCDIV = 0;
    
    //UART Setup
    init_uart();
    
    //sending a basic command
    send_command("");
}

void loop(){
//    unsigned long last_call = 0;
	char ReceivedChar = 'n';
    while(1){
        
        if (U1STAbits.URXDA == 1) {
            ReceivedChar = U1RXREG;
        }
        delay(500);
        send_command("ADDR?");
    }
}
