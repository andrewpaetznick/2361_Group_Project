#include "uart_lib.h"   // for __delay_ms() and __delay_us()
#include "xc.h"

//----------------------------------------------------
// UART1 @ 9600 baud (HM10 default) on PIC24FJ64GA002
//----------------------------------------------------
void UART1_init_9600(void) {
    /**
     * RB6 = U1TX
     * RB10 = U1RX
     * RB5  = STATE pin (input only)
     * HM-10 default: 9600 baud, 8N1
     */

    // STATE pin as input
    TRISBbits.TRISB5 = 1;

    //----------------------------------------------------
    // PPS Configuration (Matches your working framework)
    //----------------------------------------------------
    __builtin_write_OSCCONL(OSCCON & 0xbf);   // unlock PPS

    _RP6R = 0x0003;   // RB6/RP6 ? UART1 TX
    _U1RXR = 10;      // RP10 (RB10) ? UART1 RX

    __builtin_write_OSCCONL(OSCCON | 0x40);   // lock PPS

    //----------------------------------------------------
    // UART1 Setup
    //----------------------------------------------------
    U1MODEbits.UARTEN = 0;     // disable UART during config

    U1MODEbits.PDSEL = 0;      // 8-bit, no parity
    U1MODEbits.STSEL = 0;      // 1 stop bit
    U1MODEbits.BRGH = 0;       // standard speed mode

    // Baud = 9600 @ 16 MHz FCY ? BRG = 103
    U1BRG = 103;

    U1MODEbits.UARTEN = 1;     // enable UART
    U1STAbits.UTXEN  = 1;      // enable UART transmitter

    delay(500);                // HM-10 boot time (your working code did this)
}

//----------------------------------------------------
// HM10 UART initialization
//----------------------------------------------------
void HM10_init(void)
{
    UART1_init_9600();
}

//----------------------------------------------------
// Send one character
//----------------------------------------------------
void HM10_sendChar(char c)
{
    while (U1STAbits.UTXBF);    // Wait for room
    U1TXREG = c;
}

//----------------------------------------------------
// Send a string
//----------------------------------------------------
void HM10_sendStr(const char *s)
{
    while (*s)
    {
        HM10_sendChar(*s++);
    }
}

//----------------------------------------------------
// Read a char with timeout (ms)
// Returns:  1 = char received
//           0 = timeout
//----------------------------------------------------
int HM10_readChar(char* c, uint16_t timeout_ms)
{
    while (timeout_ms--)
    {
        if (U1STAbits.URXDA)    // Data ready
        {
            *c = U1RXREG;
            return 1;
        }
        delay(1);
    }
    return 0;
}

//----------------------------------------------------
// Test module: send "AT", expect "OK"
// Returns 1 if OK, 0 if fail
//----------------------------------------------------
uint8_t HM10_testModule(void)
{
    char c;
    uint8_t foundO = 0;
    uint8_t foundK = 0;

    // Flush RX buffer
    while (U1STAbits.URXDA)
        c = U1RXREG;

    HM10_sendStr("AT\r\n");

    // Look for "OK"
    uint16_t timeout = 200; // 200ms total
    while (timeout--)
    {
        if (HM10_readChar(&c, 1))
        {
            if (c == 'O')
                foundO = 1;
            else if (foundO && c == 'K')
                foundK = 1;

            if (foundO && foundK)
                return 1; // SUCCESS
        }
        delay(1);
    }

    return 0; // FAILED
}
