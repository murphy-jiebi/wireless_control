#include "usart2.h"
#include "stdio.h"
#include "string.h"
#include "bsp_gpio.h"

USART2_TypeDef usart2_rx={0};


void USART2_Init(unsigned int baudrate) 
{
    
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0} ;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART2, ENABLE);
}

uint8_t usart2_senddata(uint8_t *buf,uint8_t len)
{
    uint8_t i=0;
    if(GET_HC_STATUS!=0)
    {
        return 0;
    }
    for(i=0;i<len;i++)
    {
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR = buf[i];
    }
    return 1;
}



void USART2_IRQHandler(void) 
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
    {
        usart2_rx.rxbuf[usart2_rx.rxlen++] = USART_ReceiveData(USART2);
        if (usart2_rx.rxlen >= 32) 
        {
            usart2_rx.rxlen = 0;
        }
    }
    if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) {
        USART_ReceiveData(USART2);
        usart2_rx.rxbuf[usart2_rx.rxlen] = 0;
        usart2_rx.rxflag = 1;
    }
}
