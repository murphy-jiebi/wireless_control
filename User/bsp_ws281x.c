#include "bsp_ws281x.h"
#include "stm32f10x.h"  
#include "delay.h"

#define TIMING_ONE  61
#define TIMING_ZERO 25

unsigned short RGB_Buffer[696]; // 29 * 24 = 696
unsigned char RGB_Colour[29][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

unsigned char Colour[29][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

    uint8_t keyToColor[29]={0,28,27,26,25,24,23,15,16,17,18,19,20,14,13,12,11,10,9,1,2,3,4,5,6,22,21,8,7};
/********************************************************
Get PWM1 signal used TIM2_CH1 by GPIO PA0,
Used DMA1_CH2 translate data for TIM2_CCR1 with buffer.
********************************************************/
void WS2812X_Init(void) {
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
	TIM_OCInitTypeDef  TIM_OCInitStructure = {0};
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	DMA_InitTypeDef DMA_InitStructure = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* GPIOA Configuration: TIM2 Channel 1 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* Compute the prescaler value */
	//PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 720 - 1; // 800kHz 
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);

	/*  configure DMA    */
	/*  DMA clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/*  DMA1 Channel2 Config for PWM1 by TIM2_CH1 */
	DMA_DeInit(DMA1_Channel2);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)&(TIM2->CCR1); // physical address of Timer 3 CCR1
	DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)RGB_Buffer;   // this is the buffer memory 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;				   // data shifted from memory to peripheral
	DMA_InitStructure.DMA_BufferSize = 42;                             // I don't understand
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			   // automatically increase buffer index
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;					   // stop DMA feed after buffer size is reached
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	
	/* TIM2 DMA Request enable */
	TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);
}


void WS2812X_SendColor(unsigned char Color[][3], unsigned short Length) {
	unsigned short memaddr = 0;		                // reset buffer memory index
	// (6 * 24) + 43 = 187
	unsigned short buffersize = (Length * 24) + 43;	// number of bytes needed is #LEDs * 24 bytes + 42 trailing bytes       
	while(Length) {
        // GREEN data		
		for(unsigned char i = 0; i < 8; i++) {
			RGB_Buffer[memaddr] = ((Color[Length][1]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}
		// RED
		for(unsigned char i = 0; i < 8; i++) {
			RGB_Buffer[memaddr] = ((Color[Length][0]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}
		// BLUE
		for(unsigned char i = 0; i < 8; i++) {
			RGB_Buffer[memaddr] = ((Color[Length][2]<<i) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
			memaddr++;
		}
		Length--;
	}

  	RGB_Buffer[memaddr] = ((Color[0][2]<<8) & 0x0080) ? TIMING_ONE:TIMING_ZERO;
	memaddr++;	
	while(memaddr < buffersize) {
		RGB_Buffer[memaddr] = 0;
		memaddr++;
	}
	DMA_SetCurrDataCounter(DMA1_Channel2, buffersize); 	// load number of bytes to be transferred
	DMA_Cmd(DMA1_Channel2, ENABLE); 			// enable DMA channel 2
	TIM_Cmd(TIM2, ENABLE); 						// enable Timer 2
	while(!DMA_GetFlagStatus(DMA1_FLAG_TC2)) ; 	// wait until transfer complete
	TIM_Cmd(TIM2, DISABLE); 	                // disable Timer 2
	DMA_Cmd(DMA1_Channel2, DISABLE); 			// disable DMA channel 2
	DMA_ClearFlag(DMA1_FLAG_TC2); 				// clear DMA1 Channel 2 transfer complete flag
}


void WS2812X_Reset(void) {
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	Delay_Us(200);
}



void WS2812X_tset(void) {
	WS2812X_Reset();
	unsigned char i, j, k = 1; // 这是一个坑, 这里必须从1开始, 否则RGB显示会出错！！！
	for (i = 0; i < 29; i++) {
		for(j = 0; j < 29; j++) {
			if(j == k) {
				Colour[j][0] = 0;
				Colour[j][1] = 255;
				Colour[j][2] = 0;
			}else{
				Colour[j][0] = 0;
				Colour[j][1] = 0;
				Colour[j][2] = 0;
			}
		}
		k++;
	    WS2812X_SendColor(Colour, 28); 
		Delay_Ms(100);
	}
	Colour[28][0] = 0;
	Colour[28][1] = 0;
	Colour[28][2] = 0;
}


void WS2812X_SedColor(unsigned char R, unsigned char G, unsigned char B, unsigned char RGB) {
	if(RGB >=1 &&RGB <= 28) {
		WS2812X_Reset();
		if(RGB >= 1 && RGB <= 7) {
			Colour[29 - RGB][0] = R;
			Colour[29 - RGB][1] = G;
	        Colour[29 - RGB][2] = B;
		}else if(RGB >= 8 && RGB <= 14) {
			Colour[15 + (RGB -8)][0] = R;
			Colour[15 + (RGB -8)][1] = G;
	        Colour[15 + (RGB -8)][2] = B;
		}else if(RGB >= 15 && RGB <= 21) {
			Colour[14 - (RGB - 15)][0] = R;
			Colour[14 - (RGB - 15)][1] = G;
	        Colour[14 - (RGB - 15)][2] = B;
		}else if(RGB >= 22 && RGB <= 28) {
			Colour[RGB - 21][0] = R;
			Colour[RGB - 21][1] = G;
	        Colour[RGB - 21][2] = B;
		}
		WS2812X_SendColor(Colour, 28);
	}
}

RGB_T ledState[5]={0x00000000,0x0000FF00,0x000000FF,0x00FF0000,0x00FFFF00};

extern uint8_t keyState[29];

void LedRefresh(void)
{
    WS2812X_Reset();
    for(uint8_t i=1;i<29;i++)
    {
        Colour[keyToColor[i]][0]=ledState[keyState[i]].byte.Rbyte;
        Colour[keyToColor[i]][1]=ledState[keyState[i]].byte.Gbyte;
        Colour[keyToColor[i]][2]=ledState[keyState[i]].byte.Bbyte;
    }
    WS2812X_SendColor(Colour, 28);
}
