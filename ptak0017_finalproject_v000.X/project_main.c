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

#define RXBUFSIZE 20
volatile char rxbuf[RXBUFSIZE];  /* circular buffer */
volatile int rxint; /* last character written index */

//1 for master 0 for slave other is phone
#define ROLE 2

void setup();
void loop();
void AT_SLAVE();
void AT_MASTER();
void AT_PHONE();

/**
 * Interrupt needs volatiles that are accessed in main
 * ISR must be in main for full RX functionality
 */
void _ISR _U1RXInterrupt(void)
{
    while(U1STAbits.URXDA != 0)
    {
        if (rxint >= RXBUFSIZE - 1)
            rxint = -1;
        rxbuf[rxint + 1] = U1RXREG;
        rxint++; /* Do increment to be the last to avoid racing cond. */
    }
    IFS0bits.U1RXIF = 0;
}

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
    send_str("HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO HELLO ");
    //sending a basic command
    char ReceivedChar = 'n';
    send_command("");
    delay(500);
    if (U1STAbits.URXDA == 1) {
            ReceivedChar = U1RXREG;
    }
    send_command("ADDR?");
    delay(500);
    send_command("SLEEP");
    delay(500);
    if (U1STAbits.URXDA == 1) {
            ReceivedChar = U1RXREG;
    }
    if(ROLE == 1) {
        AT_MASTER();
    }
    else if(ROLE == 0) {
        AT_SLAVE();
    }
    else {
        AT_PHONE();
    }
   
}

void loop(){
//    unsigned long last_call = 0;
    
    while(1){
        
        AT_PHONE();
        //send_str("Connected?")
        delay(500);
        
    }
}


void AT_SLAVE(){
    send_command("");
    send_command("ROLE0");          // Peripheral/slave device
    send_command("RESET");      
    send_command("IMME1");          // Wait in AT mode until connected
    send_command("AT+NAMEHM_B");    //rename(not needed)
    send_command("AT+RESET");
}

void AT_MASTER(){
    //needs ADDR for SLAVE using AT+ADDR?
    send_command("ROLE1");      //Master device
    send_command("RESET");
    send_command("IMME1");      //sets to AT mode
    send_command("NAMEHM_A");   //rename(not needed)
    send_command("RESET");
    //T+CONA1B2C3D4E5F6 example address with no colons
    
    //should recieve OK+CONN
    
    //disconnect using AT+DISC
}

void AT_PHONE() {
    send_command("ROLE0");     // peripheral
    send_command("IMME1");     // start in AT mode, wait for connection
    send_command("RESET");     //resets and begins waiting
}