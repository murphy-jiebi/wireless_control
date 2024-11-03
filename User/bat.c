#include "stm32f10x.h"  
#include "bat.h"
#include "delay.h"
#include <stdio.h>
#include "adc.h"  

// ʵ�ʲ��Ե�ص�ѹ4.15V����⵽��ѹ4.145V��4.152V��4.147V�����ǳ�С��(�ٷ�֮һ)
// �ڴ���Ŀ�в�����Ҫ�ٷ�֮�ٵĵ���׼ȷ��ֻ��Ҫ��ŵ�ֵ���ɣ�����LED��ʾ����ʣ�������
// ��������������Դ�����ñ���������辫����ADCƯ����ԭ��Ӱ��΢����΢������
// ��Ҫԭ����Կ��ǵ����ԭ��Ŀǰʹ�õ��Ǿ���: 1% �����Գ����滻����: 0.1%
float Get_BAT_voltage(void) {
	float vlotage = 0;
	unsigned int value = 0;
	for(unsigned char i = 0; i < 5; i++) {
		// value += Get_BAT_ADCValue();
		value += ADC_GetValue(ADC_Channel_1);
		Delay_Ms(3);
	}
	value /= 5;
	vlotage = ((float)value * 3.30) / 4096;  // ��ѹֵ
	float i = (vlotage / 10000);  // �������ֵ�ǳ�С�������ǲ��ĵ�ģ�������
	return vlotage + (i * 47000);
}


// �����ص���ʣ��ٷֱ�
float Get_BAT_mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



