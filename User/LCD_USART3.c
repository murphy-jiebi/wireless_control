#include "LCD_USART3.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"

void LCD_USART3_Init(unsigned int baudrate) {
    
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0} ;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_Cmd(USART3, ENABLE);
}

LCD_USART3_TypeDef LCD_USART3 = {0};

void LCD_USART3_ClearStruct(void) {
    LCD_USART3.Count = 0;
    LCD_USART3.Flag = 0;
    memset(&LCD_USART3, 0, sizeof(LCD_USART3));
}

void LCD_USART3_SendString(unsigned char *String) {
    while (*String != 0) {
        while (!(USART3->SR & USART_SR_TXE));
        USART3->DR = *String++;
    }
}

void LCD_USART3_SendByte(unsigned char Byte) {
    while (!(USART3->SR & USART_SR_TXE));
    USART3->DR = Byte;
}

void LCD_USART3_SendThreeBytes(unsigned char Byte1, unsigned char Byte2, unsigned char Byte3) {
    LCD_USART3_SendByte(Byte1);
    LCD_USART3_SendByte(Byte2);
    LCD_USART3_SendByte(Byte3);
}

void USART3_IRQHandler(void) {
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        LCD_USART3.Buff[LCD_USART3.Count++] = USART_ReceiveData(USART3);
        if (LCD_USART3.Count >= 100) {
            LCD_USART3.Count = 0;
        }
        LCD_USART3.Flag = 0;
    }
    if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET) {
        USART_ReceiveData(USART3);
        LCD_USART3.Buff[LCD_USART3.Count] = 0;
        LCD_USART3.Flag = 1;
    }
}

