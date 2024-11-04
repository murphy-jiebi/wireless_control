#include "bsp_gpio.h"

// ���IO����
// ���װ�� 1 ->  GPIOA, PA15
// ���װ�� 2 ->  GPIOC PC11
// �м�װ�� 1 ->  GPIOD PD2
// �м�װ�� 2 ->  GPIOB PB4
// �ұ�װ�� 1 -> GPIOB PB6
// �ұ�װ�� 2 -> GPIOB PB8

// ���ߵ����Ӽ��
// ���װ�� 1 ->  GPIOC PC10
// ���װ�� 2 ->  GPIOC PC12
// �м�װ�� 1 ->  GPIOB PB3
// �м�װ�� 2 ->  GPIOB PB5
// �ұ�װ�� 1 -> GPIOB PB7
// �ұ�װ�� 2 -> GPIOB PB9

//����ģ�����
//hc key    PC4
//HC STA     PC5

void (*SetWireFunc[6])(void)={0};
void bsp_GpioInit(void)
{
    //���߿���
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
    // GPIOB  PB8��PB6��PB4

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

    //���߼��
    GPIOB_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_7 | GPIO_Pin_5 | GPIO_Pin_3;
    GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������Ϊ��������Ӧ���������Կ����������룿
    //GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIOB_InitStructure);


    GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_10;
    GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������Ϊ��������Ӧ���������Կ����������룿
    //GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIOC_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIOC_InitStructure);

    //����ģ�鷢�ͼ��
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