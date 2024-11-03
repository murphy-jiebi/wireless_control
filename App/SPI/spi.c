#include "spi.h" 

// ������SD����Ӳ��spi

void SPI1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    SPI_InitTypeDef SPI_InitStruct = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;  // ���츴�����
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
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;		//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//��ʼ��

	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	//����Ƶ
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;						//�ڶ���ʱ�ӱ��ز���
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;						//ʱ�ӿ���Ϊ��
	SPI_InitStruct.SPI_CRCPolynomial = 0x7;							//CRC����ʽ
	SPI_InitStruct.SPI_DataSize  = SPI_DataSize_8b;					//��λ����֡��ʽ
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//˫��˫��ȫ˫��
	SPI_InitStruct.SPI_FirstBit  = SPI_FirstBit_MSB;				//��λ�ȷ�
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;						//��ģʽ
	SPI_InitStruct.SPI_NSS  = SPI_NSS_Soft;							//�������
	SPI_Init(SPI1, &SPI_InitStruct);
	
	SPI_Cmd(SPI1, ENABLE);	
}*/


//void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data);
//uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx);
//�������ݣ����߽������ݵ�ʱ�򣬶���Ҫ�������ݣ����԰ѽ��շ���д��ͬһ������
unsigned char Spi1_WriteReadByte(unsigned char data)
{
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
    SPI_I2S_SendData(SPI1,data);
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
    return SPI_I2S_ReceiveData(SPI1);
}

