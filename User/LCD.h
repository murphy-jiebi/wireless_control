#ifndef __LCD_H
#define __LCD_H

#include "main.h"

extern unsigned char LCD_Sleep_State;
extern unsigned char LCD_Wired_Receiver_Select_Status[6];
extern unsigned char LCD_Ignition_Window_Status;

extern unsigned char LCD_Select_Control_Status[4];

void LCD_Rest(void);
unsigned char* LCD_Get_RTC_Time(void);
void Parsing_LCD_Response_Data(unsigned char *LCD_USART3_DATA);

void BatLevelRefresh(void);
void LcdRefresh(void);
void LcdRecHandle(void);

#endif
