/* 
 * File:   uart_chat.h
 * Author: andre
 *
 * Created on November 21, 2025, 2:56 PM
 */

#ifndef UART_CHAT_H
#define	UART_CHAT_H

#ifdef	__cplusplus
extern "C" {
#endif

void UART1_init_9600(void);
void HM10_init(void);
void HM10_sendChar(char c);
void HM10_sendStr(const char *s);
int  HM10_readChar(char *c, uint16_t timeout_ms);
uint8_t HM10_testModule(void);  // returns 1 if OK


#ifdef	__cplusplus
}
#endif

#endif	/* UART_CHAT_H */

