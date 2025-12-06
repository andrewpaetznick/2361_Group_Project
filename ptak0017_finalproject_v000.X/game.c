/*
 * File:   game.c
 * Author: andre
 *
 * Created on December 5, 2025, 6:39 PM
 */


#include "xc.h"
#include <stdio.h>
#include "lcd.h"


void run_game(){
    
}

void print_scores(int score1, int score2){
    lcd_setCursor(0,0);
    lcd_printStr("Player 1:" + printf(score1));
    lcd_setCursor(0,1);
    lcd_printStr("Player 2:" + printf(score2));
}