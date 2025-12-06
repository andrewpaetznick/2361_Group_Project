/*
 * File:   lcd.c
 * Author: andre
 *
 * Created on December 5, 2025, 6:19 PM
 */

#include "xc.h"
#include "delay.h"  //used for reset delay

void lcd_reset();
void lcd_cmd(char command);
void lcd_printChar(char myChar);
void delay(int ms);
void lcd_printStr(const char * str);

volatile int lcdAddress;    //set in lcd_init
volatile char testChar = '0';

void _ISR _U2RXInterrupt(void)
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

void lcd_init(void) {
    lcdAddress = 0x78;  //will be used in all commands over I2C for LCD
    
    TRISBbits.TRISB8 = 1;   // SCL1
    TRISBbits.TRISB9 = 1;   // SDA1
    
    
    // Reset pin as output and pulse
    TRISBbits.TRISB6 = 0;
    delay(1);
    
    LATBbits.LATB6 = 0;
    delay(10);
    LATBbits.LATB6 = 1;
    delay(10);
    
    I2C1CONbits.I2CEN = 0;
    
    I2C1BRG = 0x009D;
    I2C1CONbits.DISSLW = 1;
    _MI2C1IF = 0;
    _MI2C1IE = 0;
    
    I2C1CONbits.I2CEN = 1;
    
    
    lcd_reset();
    

    lcd_cmd(0x3A);  // RE=1 (enter extended)
    lcd_cmd(0x09);  // NW=1 (4-line internal layout)
    lcd_cmd(0x39);  // RE=1, IS=1 (IS page)
    lcd_cmd(0x1B);  // Internal OSC
    lcd_cmd(0x6E);  // Follower control
    lcd_cmd(0x56);  // Power/contrast high bits
    lcd_cmd(0x7A);  // Contrast low bits
    lcd_cmd(0x38);  // Back to RE=0, IS=0 (basic)
    lcd_cmd(0x0C);  // Display ON, cursor OFF, blink OFF
    lcd_cmd(0x01);  // Clear
    delay(2);       //delay for clearing lcd
    lcd_cmd(0x3A);  // RE=1 (extended)
    lcd_cmd(0x09);  // NW=1
    lcd_cmd(0x1A);  // UD2=1, UD1=0 (double-height halves mapped properly)
    lcd_cmd(0x3C);  // RE=0, DH=1 (enable double-height)
}

void lcd_reset() {
    //20us minimum delay
    delay(1);   //1ms
    LATBbits.LATB6 = 0;
    delay(1);   //1ms
    LATBbits.LATB6 = 1;
    delay(5);   //5ms
}

void lcd_cmd(char command) {
    //turns on I2C for command send
    I2C1CONbits.SEN = 1;            
    while (I2C1CONbits.SEN == 1);  
    _MI2C1IF = 0;        
    
    //using blocking for the sending currently
    //address byte
    I2C1TRN = lcdAddress;      //RnW = 0
    while (I2C1STATbits.TRSTAT == 1); 
    _MI2C1IF = 0;                 

    //control byte
    I2C1TRN = 0b00000000;           
    while (I2C1STATbits.TRSTAT == 1);
    _MI2C1IF = 0;

    //command byte
    I2C1TRN = command;
    while (I2C1STATbits.TRSTAT == 1); //
    _MI2C1IF = 0;

    //turns off I2C for now
    I2C1CONbits.PEN = 1;            
    while (I2C1CONbits.PEN == 1);   
    _MI2C1IF = 0;
}

void lcd_printChar(char data)
{
    //Same basic code as lcd_cmd
    I2C1CONbits.SEN = 1;            
    while (I2C1CONbits.SEN == 1 || _MI2C1IF == 0);  
    _MI2C1IF = 0;                   

    //address
    I2C1TRN = lcdAddress;       
    while (I2C1STATbits.TRSTAT == 1 || _MI2C1IF == 0); 
    _MI2C1IF = 0;                 

    //control RnW = 1
    I2C1TRN = 0b01000000;           
    while (I2C1STATbits.TRSTAT == 1 || _MI2C1IF == 0);
    _MI2C1IF = 0;

    //char sending byte
    I2C1TRN = data;
    while (I2C1STATbits.TRSTAT == 1 || _MI2C1IF == 0); //
    _MI2C1IF = 0;

    I2C1CONbits.PEN = 1;            
    while (I2C1CONbits.PEN == 1 || _MI2C1IF == 0);   
    _MI2C1IF = 0;
}

void lcd_setCursor(char x, char y) {
    //sets I2C waiting for stop flag
    I2C1CONbits.PEN = 1;
    while(_PEN);
    
    //math for command byte send
    char pos = 0x00;
    if (y == 0x01) 
    {
        pos = 0x20;
    }
    pos += x;
    pos |= 0x80;
    
    //sending correct position for cursor
    lcd_cmd(pos);   
}

void lcd_printStr(const char * str){
    int length = 16;
    for(int i = 0; i < length; i++){
        lcd_setCursor(i, 0);   //x changes, no overflow
        lcd_printChar(str[i]);
    }
}

void delay(int ms){
    for(int i = 0; i < ms; i++){
        arp_delay_1ms();
    }
}