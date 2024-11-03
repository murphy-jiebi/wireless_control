#ifndef _SPI_H_
#define _SPI_H_
// #include "stm32f4xx.h"
#include "stm32f10x.h"
#include "main.h"

// void SPI3_Init(void);
void SPI1_Init(void);
// unsigned char Spi3_send_show_byte(unsigned char data);
unsigned char Spi1_WriteReadByte(unsigned char data);

#endif
