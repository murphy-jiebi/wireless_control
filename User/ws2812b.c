#include "stm32f10x.h"  
#include "ws2812b.h"
#include "delay.h"

// 28 * 24 = 672
unsigned short RGB_Buffer[672]; // 28���� * 24λ = 672λ

void WS2812B_Init(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // ��GPIOA��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // ��TIM2��ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    // ��DMA1��ʱ��
	
	// PA0 -> DI
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
	TIM_TimeBaseStructure.TIM_Period = 90 - 1; // 800kHz  ARR
	TIM_TimeBaseStructure.TIM_Prescaler = 0;   // PSC
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef  TIM_OCInitStructure = {0};
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM1ģʽ��С�ڱȽ�ֵ����ߵ�ƽ����������͵�ƽ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�������ͨ������
	TIM_OCInitStructure.TIM_Pulse = 0;//�Ƚ�ֵ��ʼΪ0��һֱ����͵�ƽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	DMA_InitTypeDef DMA_InitStructure = {0};
	DMA_DeInit(DMA1_Channel2);	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)&(TIM2->CCR1);	// ����վ����ʼ��ַ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ��
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�Ƿ�����
	DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)RGB_Buffer;		// �洢��վ�����ʼ��ַ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ��
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					// �Ƿ�����
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						// ���䷽�򣬴Ӵ洢��������
	DMA_InitStructure.DMA_BufferSize = 0;	//��������С�����������
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							// ����ģʽ������ѭ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//�洢�����洢��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//���ȼ�
	DMA_Init(DMA1_Channel2, &DMA_InitStructure); // DMA1-ͨ��2
	TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);    // ʹ�ܶ�ʱ��2��TIM_DMA_Update�����ж�
}

// �޸�RGB��ɫ��������ֵ
void WS2812B_SetColor(unsigned char RGB, unsigned int Color) {
	 if(RGB <= 27) {
		 unsigned short k = RGB * 24;
		 for (unsigned char i = 0; i < 24; i++) {
			if (0x800000 & ((Color << i))) {
				RGB_Buffer[k] = 55;
			}else{
				RGB_Buffer[k] = 25;
			}
			k++;
		 }
	 }
	 // WS2812B_SendColor()
}


// ʹ��DMA����RGB��ɫ����
// ����RGB����: �޸�RGB������ɫֵ -> ����RGB��ɫ���� -> ���RGB��ɫ
// ���RGB��ɫ���ݲ���ȷ�����������̲���ȷ�����ܻᵼ��RGB����ICֱ�ӿ���������
// �������ԣ�����֮�������ϵ磬LED����ʾ��ɫ������ģ���һ���ǵ���֮ǰ���õ�ֵ�������ϵ����Ҫ��������LED����ʾ��ɫ
void WS2812B_SendColor(void) {
	DMA_SetCurrDataCounter(DMA1_Channel2, 672); // �����������д���ݣ���Ҫ���Ͷ��ٸ�����
	DMA_Cmd(DMA1_Channel2, ENABLE);				// ʹ��DMA����ʼ����
	TIM_Cmd(TIM2, ENABLE);						// ʹ�ܶ�ʱ��
	while (!DMA_GetFlagStatus(DMA1_FLAG_TC2));  // �ȴ��������
	TIM_Cmd(TIM2, DISABLE);
	DMA_Cmd(DMA1_Channel2, DISABLE);            // �ر�DMA
	DMA_ClearFlag(DMA1_FLAG_TC2);	            // ���DMA��־λ
}


// RGBȫ��Ϩ��
// �����ҪϨ��ĳһ��RGB���飬��ʹ�� WS2812B_SetColor() + WS2812B_SendColor()
void WS2812B_WholeOFF(void) {
	for (unsigned char i = 0; i < 28; i++) {
		WS2812B_SetColor(i, RGB_BLACK);
	}
	WS2812B_SendColor();
	WS2812B_Reset();
}


// RGB��λ
// ��������280us���ϵĵ͵�ƽ�źţ�RGB�Զ���λ
// RGB��λ֮��������������ɫ���ݿ��ܻ���ִ��ң�������ȫ�������ɫ������
void WS2812B_Reset(void) {
	// code...
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	Delay_Ms(1);
}


void WS2812_test(void) {
	static unsigned char j = 0;
	if(j <= 27) {
		for(unsigned char i = 0; i < 28; i++) {
			if(i != j) {
				WS2812B_SetColor(i,RGB_BLACK);
			}
		}
		WS2812B_SetColor(j,RGB_RED);
		WS2812B_SendColor();
		j++;
	}else{
		WS2812B_Reset();
		WS2812B_WholeOFF();
	}
}



