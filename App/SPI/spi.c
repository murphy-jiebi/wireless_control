#include "spi.h" 

// 这里是SD卡的硬件spi

void SPI1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef SPI_InitStruct = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;  // 推挽复用输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_InitStruct.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStruct.SPI_FirstBit  = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_NSS  = SPI_NSS_Soft;
    SPI_Init(SPI1, &SPI_InitStruct);

    SPI_Cmd(SPI1, ENABLE);  
}


/* void SPI1_Init(void) {

    GPIO_InitTypeDef GPIO_InitStructure = {0};
	SPI_InitTypeDef SPI_InitStruct		= {0};
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//开外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;		//复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	//浮空
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//初始化

	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	//二分频
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;						//第二个时钟边沿采样
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;						//时钟空闲为高
	SPI_InitStruct.SPI_CRCPolynomial = 0x7;							//CRC多项式
	SPI_InitStruct.SPI_DataSize  = SPI_DataSize_8b;					//八位数据帧格式
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//双线双向全双工
	SPI_InitStruct.SPI_FirstBit  = SPI_FirstBit_MSB;				//高位先发
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;						//主模式
	SPI_InitStruct.SPI_NSS  = SPI_NSS_Soft;							//软件管理
	SPI_Init(SPI1, &SPI_InitStruct);
	
	SPI_Cmd(SPI1, ENABLE);	
}*/


//void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data);
//uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx);
//发送数据，或者接收数据的时候，都需要发送数据，可以把接收发送写成同一个函数
unsigned char Spi1_WriteReadByte(unsigned char data)
{
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
    SPI_I2S_SendData(SPI1,data);
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
    return SPI_I2S_ReceiveData(SPI1);
}

