#ifndef __BSP_WS281X_H
#define __BSP_WS281X_H

#include "stm32f10x.h"



typedef union{
    uint32_t RGB;
    struct
    {
        uint32_t Bbyte:8;
        uint32_t Gbyte:8;
        uint32_t Rbyte:8;
        uint32_t Abyte:8;
    }byte;  
}RGB_T;

extern unsigned char RGB_Colour[29][3];

extern unsigned char Colour[29][3];


void WS2812X_Init(void);
void WS2812X_SendColor(unsigned char Color[][3], unsigned short Length);
void WS2812X_SedColor(unsigned char R, unsigned char G, unsigned char B, unsigned char RGB);
void WS2812X_Reset(void);
void WS2812X_tset(void);
void LedRefresh(void);

#endif
