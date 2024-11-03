#include "stm32f10x.h"  
#include "ws2812b.h"
#include "delay.h"

// 28 * 24 = 672
unsigned short RGB_Buffer[672]; // 28个灯 * 24位 = 672位

void WS2812B_Init(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开GPIOA启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // 开TIM2启时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    // 开DMA1启时钟
	
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
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef  TIM_OCInitStructure = {0};
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//PWM1模式，小于比较值输出高电平，大于输出低电平，
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//开启输出通道引脚
	TIM_OCInitStructure.TIM_Pulse = 0;//比较值初始为0，一直输出低电平
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	DMA_InitTypeDef DMA_InitStructure = {0};
	DMA_DeInit(DMA1_Channel2);	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int)&(TIM2->CCR1);	// 外设站点起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//是否自增
	DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)RGB_Buffer;		// 存储器站点的起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					// 是否自增
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						// 传输方向，从存储器到外设
	DMA_InitStructure.DMA_BufferSize = 0;	//缓存区大小，传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							// 传送模式，单次循环
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//存储器到存储器
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//优先级
	DMA_Init(DMA1_Channel2, &DMA_InitStructure); // DMA1-通道2
	TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);    // 使能定时器2的TIM_DMA_Update更新中断
}

// 修改RGB颜色数据数组值
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


// 使用DMA发送RGB颜色数据
// 控制RGB流程: 修改RGB数组颜色值 -> 发送RGB颜色数据 -> 清空RGB颜色
// 如果RGB颜色数据不正确或者上述流程不正确，可能会导致RGB内置IC直接卡死！！！
// 经过测试，掉电之后，重新上电，LED的显示颜色是随机的，不一定是掉电之前设置的值，所以上电后需要重新设置LED的显示颜色
void WS2812B_SendColor(void) {
	DMA_SetCurrDataCounter(DMA1_Channel2, 672); // 给传输计数器写数据，需要传送多少个数据
	DMA_Cmd(DMA1_Channel2, ENABLE);				// 使能DMA，开始传输
	TIM_Cmd(TIM2, ENABLE);						// 使能定时器
	while (!DMA_GetFlagStatus(DMA1_FLAG_TC2));  // 等待传输完成
	TIM_Cmd(TIM2, DISABLE);
	DMA_Cmd(DMA1_Channel2, DISABLE);            // 关闭DMA
	DMA_ClearFlag(DMA1_FLAG_TC2);	            // 清除DMA标志位
}


// RGB全部熄灭
// 如果需要熄灭某一个RGB灯珠，请使用 WS2812B_SetColor() + WS2812B_SendColor()
void WS2812B_WholeOFF(void) {
	for (unsigned char i = 0; i < 28; i++) {
		WS2812B_SetColor(i, RGB_BLACK);
	}
	WS2812B_SendColor();
	WS2812B_Reset();
}


// RGB复位
// 持续发送280us以上的低电平信号，RGB自动复位
// RGB复位之后，重新启动，颜色数据可能会出现错乱，建议先全部清空颜色？？？
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



