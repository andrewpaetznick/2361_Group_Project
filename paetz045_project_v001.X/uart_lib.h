/* 
 * File:   uart_lib.h
 * Author: andre
 *
 * Created on November 18, 2025, 5:13 PM
 */

#ifndef UART_LIB_H
#define	UART_LIB_H

#ifdef	__cplusplus
extern "C" {
#endif


void init_uart(void);
void send_command(const char* command);
void delay(int ms_delay);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_LIB_H */

