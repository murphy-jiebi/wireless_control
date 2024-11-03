#ifndef __LCD_USART3_H
#define __LCD_USART3_H

typedef struct {
	unsigned char Buff[100];
	unsigned char Count;
	unsigned char Flag;
}LCD_USART3_TypeDef;

extern LCD_USART3_TypeDef LCD_USART3;

void LCD_USART3_Init(unsigned int baudrate);
void LCD_USART3_ClearStruct(void);
void LCD_USART3_SendString(unsigned char *String);
void LCD_USART3_SendByte(unsigned char Byte);
void LCD_USART3_SendThreeBytes(unsigned char Byte1, unsigned char Byte2, unsigned char Byte3);

#endif
