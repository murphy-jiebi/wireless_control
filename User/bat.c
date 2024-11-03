#include "stm32f10x.h"  
#include "bat.h"
#include "delay.h"
#include <stdio.h>
#include "adc.h"  

// 实际测试电池电压4.15V，检测到电压4.145V、4.152V、4.147V，误差非常小了(百分之一)
// 在此项目中并不需要百分之百的电量准确，只需要大概的值即可，控制LED显示大致剩余电流。
// 如果存在误差，误差来源于万用表测量误差、电阻精度误差、ADC漂浮等原因，影响微乎其微！！！
// 主要原因可以考虑电阻的原因，目前使用的是精度: 1% ，可以尝试替换精度: 0.1%
float Get_BAT_voltage(void) {
	float vlotage = 0;
	unsigned int value = 0;
	for(unsigned char i = 0; i < 5; i++) {
		// value += Get_BAT_ADCValue();
		value += ADC_GetValue(ADC_Channel_1);
		Delay_Ms(3);
	}
	value /= 5;
	vlotage = ((float)value * 3.30) / 4096;  // 分压值
	float i = (vlotage / 10000);  // 这个电流值非常小，几乎是不耗电的！！！！
	return vlotage + (i * 47000);
}


// 计算电池电量剩余百分比
float Get_BAT_mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



