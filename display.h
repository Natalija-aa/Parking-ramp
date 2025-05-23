#ifndef DISPLEJ_H
#define DISPLEJ_H

#include "types.h"

#define LCD_EN P3_2
#define LCD_RS P3_3
	 
#define LCD_D7 P1_0
#define LCD_D6 P1_1
#define LCD_D5 P1_2
#define LCD_D4 P1_3

#define LCD_BL P1_4

void initDisplay(void);
void clearDisplay(void);
void writeChar(uint8_t);
void writeLine(uint8_t*);
void newLine(void);	

#endif