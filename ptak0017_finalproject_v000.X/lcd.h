/* 
 * File:   lcd.h
 * Author: andre
 *
 * Created on December 5, 2025, 6:41 PM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif


    void lcd_cmd(char command);
    void lcd_init();
    void lcd_setCursor(char x, char y);
    void lcd_printChar(char myChar);
    void delay(int ms);
    void lcd_printStr(const char * str);

    

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

