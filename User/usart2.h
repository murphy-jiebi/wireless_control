#ifndef _USART2_H
#define _USART2_H

#include "stm32f10x.h"

typedef struct {
	unsigned char rxbuf[32];
	unsigned char rxlen;
	unsigned char rxflag;
}USART2_TypeDef;

extern USART2_TypeDef usart2_rx;

void USART2_Init(unsigned int baudrate);
uint8_t usart2_senddata(uint8_t *buf,uint8_t len);
 


#endif


