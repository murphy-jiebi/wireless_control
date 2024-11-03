#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "adc.h"

void ADC_Configura_Init(void) {
    ADC_InitTypeDef ADC_InitStructure = {0};
    GPIO_InitTypeDef GPIOC_InitStructure = {0};
	 GPIO_InitTypeDef GPIOA_InitStructure = {0};

    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    
    GPIOC_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIOC_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIOC_InitStructure);
	
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_1;  // PA1
    GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIOA_InitStructure);

    // ADC1 ����
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; // ��ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // ����ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // ��ʹ���ⲿ����
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // �Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1; // 1��ת��ͨ��
    ADC_Init(ADC1, &ADC_InitStructure);

    // ʹ��ADC1��У׼
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

unsigned short ADC_GetValue(unsigned char Channel) 
{
    ADC_RegularChannelConfig(ADC1, Channel, 1, ADC_SampleTime_55Cycles5); // ����ADCͨ���Ͳ���ʱ��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // ��ʼת��
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); // �ȴ�ת�����
    unsigned short ADCValue = ADC_GetConversionValue(ADC1); // ��ȡADCֵ
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC); // �����־λ
    return ADCValue;
}
