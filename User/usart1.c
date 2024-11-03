#include "stm32f10x.h"
#include "stdio.h"
#include "usart1.h"
#include "string.h"

// 只需要串口打印功能, 调试日志使用, 烧录成品关闭此功能
void USART1_Init(unsigned int baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx; 
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}


/*----加入以下代码,支持printf函数,而不需要选择use MicroLIB ----*/
#if 1
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE {
    int handle;

};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x) {
    x = x;
}

// 重定义fputc函数
int fputc(int ch, FILE *f) {
    while((USART1->SR & 0X40) == 0);
    USART1->DR = (unsigned char) ch;
    return ch;
}
#endif

