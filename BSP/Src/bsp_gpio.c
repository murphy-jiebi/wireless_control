#include "bsp_gpio.h"

// 点火IO配置
// 左边装置 1 ->  GPIOA, PA15
// 左边装置 2 ->  GPIOC PC11
// 中间装置 1 ->  GPIOD PD2
// 中间装置 2 ->  GPIOB PB4
// 右边装置 1 -> GPIOB PB6
// 右边装置 2 -> GPIOB PB8

// 有线点火端子检测
// 左边装置 1 ->  GPIOC PC10
// 左边装置 2 ->  GPIOC PC12
// 中间装置 1 ->  GPIOB PB3
// 中间装置 2 ->  GPIOB PB5
// 右边装置 1 -> GPIOB PB7
// 右边装置 2 -> GPIOB PB9

//无线模块控制
//hc key    PC4
//HC STA     PC5

void (*SetWireFunc[6])(void)={0};
void bsp_GpioInit(void)
{
    //有线控制
    // GPIOA PA15
    GPIO_InitTypeDef GPIOA_InitStructure = {0};
    GPIO_InitTypeDef GPIOB_InitStructure = {0};
    GPIO_InitTypeDef GPIOC_InitStructure = {0};
    GPIO_InitTypeDef GPIOD_InitStructure = {0};
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    
    GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIOA_InitStructure);
    GPIO_ResetBits(GPIOA, GPIO_Pin_15);
    // GPIOB  PB8、PB6、PB4

    GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_6 | GPIO_Pin_4;
    GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIOB_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_6 | GPIO_Pin_4);
    // GPIOC PC11

    GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIOC_InitStructure);
    GPIO_ResetBits(GPIOC, GPIO_Pin_11);
    // GPIOD PD2
    
    GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIOD_InitStructure);
    GPIO_ResetBits(GPIOD, GPIO_Pin_2);

    RESET_WIRE_L1;
    RESET_WIRE_L2;
    RESET_WIRE_M1;
    RESET_WIRE_M2;
    RESET_WIRE_R1;
    RESET_WIRE_R2;

    //有线检测
    GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_3;
    GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 这里配置为浮空输入应更合理，可以考虑下拉输入？
    //GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIOB_InitStructure);


    GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_10;
    GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 这里配置为浮空输入应更合理，可以考虑下拉输入？
    //GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIOC_InitStructure);

    //无线模块发送检测
    GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIOC_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_4);
    
    GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIOC_InitStructure);
    

}