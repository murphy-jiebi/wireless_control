#include "stm32f10x.h"  
#include "key.h"
#include <stdio.h>
#include "bsp_ws281x.h"
#include "adc.h"
#include "LCD_USART3.h"
#include "LCD.h"
#include "my_sd.h"
#include "LCD_USART3.h"
#include "string.h"
#include "delay.h"
#include "tim.h"
//#include "hc14.h"
//#include "bsp_ws281x.h"
//#include "protocol.h"
//#include "crc16.h"
 
void Select_Port(unsigned char Key_Select_Port);
void Key_Clean_Receiver_Select(unsigned char Select_State);
unsigned char Get_Port_Select_Number(unsigned char *Key_Port_Select);
void Select_All_Port(void);
unsigned char Get_isMeetWith_Start(void);


// 这里直接弄一个全局变量， 使用主函数里面发送引爆指令


// 按键选择接收机项 , (同一时间)每一次只能选择并设置一个接收机
unsigned char Key_Select_Receiver = 0; 
// 这里是28个按键的状态
unsigned char Key_Select_State[28] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// （接收机1）Key8选择的端子状态
unsigned char Key8_Port_Select[6] = {0,0,0,0,0,0};
// （接收机2）Key9选择的端子状态
unsigned char Key9_Port_Select[6] = {0,0,0,0,0,0};
// （接收机3）Key10选择的端子状态
unsigned char Key10_Port_Select[6] = {0,0,0,0,0,0};
// （接收机4）Key11选择的端子状态
unsigned char Key11_Port_Select[6] = {0,0,0,0,0,0};
// （接收机5）Key12选择的端子状态
unsigned char Key12_Port_Select[6] = {0,0,0,0,0,0};
// （接收机6）Key13选择的端子状态
unsigned char Key13_Port_Select[6] = {0,0,0,0,0,0};
// （接收机7）Key15选择的端子状态
unsigned char Key15_Port_Select[6] = {0,0,0,0,0,0};
// （接收机8）Key16选择的端子状态
unsigned char Key16_Port_Select[6] = {0,0,0,0,0,0};
// （接收机9）Key17选择的端子状态
unsigned char Key17_Port_Select[6] = {0,0,0,0,0,0};
// （接收机10）Key17选择的端子状态
unsigned char Key18_Port_Select[6] = {0,0,0,0,0,0};
// （接收机11）Key19选择的端子状态
unsigned char Key19_Port_Select[6] = {0,0,0,0,0,0};
// （接收机12）Key20选择的端子状态
unsigned char Key20_Port_Select[6] = {0,0,0,0,0,0};

unsigned char Key1_Port_Select[6] = {0,0,0,0,0,0};
unsigned char Key2_Port_Select[6] = {0,0,0,0,0,0};
unsigned char Key3_Port_Select[6] = {0,0,0,0,0,0};
unsigned char Key4_Port_Select[6] = {0,0,0,0,0,0};
unsigned char Key5_Port_Select[6] = {0,0,0,0,0,0};
unsigned char Key6_Port_Select[6] = {0,0,0,0,0,0};
unsigned char Key7_Port_Select[6] = {0,0,0,0,0,0};

void Key_ADC1_Scanf(void) {
	static unsigned char Key_Check_State = 0;
	static unsigned char Key_Flag = 0;
	static unsigned int i = 0;
	unsigned short ADC_Value = ADC_GetValue(ADC_Channel_12);
	// ADC Value: 0
	if(ADC_Value <= 20 && Key_Flag == 0) 
    {
		i++;
		if(i >= 1000) 
        {
			Key_Select_Receiver = 1;
			if(LCD_Wired_Receiver_Select_Status[0] == 0 && Key_Select_State[0] == 0)
            {
				LCD_USART3_SendString("img_wired_1.pic=1");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[0] = 1;
				WS2812X_SedColor(0, 0, 255, 1);
				Key_Select_State[0] = 1;
			}else{
				LCD_USART3_SendString("img_wired_1.pic=4");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[0] = 0;
				WS2812X_SedColor(0, 0, 0, 1);
				Key_Select_State[0] = 0;
			}
			Key_Flag = 1;
		}
	}
    else if(ADC_Value >= 4070 && Key_Flag == 1) 
    {
		Key_Flag = 0;
		i = 0;
	}


	// ADC Value: 794
	if(ADC_Value >= 600 && ADC_Value <= 900 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			Key_Select_Receiver = 2;
			if(LCD_Wired_Receiver_Select_Status[1] == 0 && Key_Select_State[1] == 0) {
				LCD_USART3_SendString("img_wired_2.pic=1");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[1] = 1;
				WS2812X_SedColor(0, 0, 255, 2);
				Key_Select_State[1] = 1;
			}else{
				LCD_USART3_SendString("img_wired_2.pic=4");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[1] = 0;
				WS2812X_SedColor(0, 0, 0, 2);
				Key_Select_State[1] = 0;
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 1540
	if(ADC_Value >= 1400 && ADC_Value <= 1650 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			Key_Select_Receiver = 3;
			if(LCD_Wired_Receiver_Select_Status[2] == 0 && Key_Select_State[2] == 0) {
				LCD_USART3_SendString("img_wired_3.pic=1");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[2] = 1;
				WS2812X_SedColor(0, 0, 255, 3);
				Key_Select_State[2] = 1;
			}else{
				LCD_USART3_SendString("img_wired_3.pic=4");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[2] = 0;
				WS2812X_SedColor(0, 0, 0, 3);
				Key_Select_State[2] = 0;
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 2258
	if(ADC_Value >= 2150 && ADC_Value <= 2350 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			Key_Select_Receiver = 4;
			if(LCD_Wired_Receiver_Select_Status[3] == 0 && Key_Select_State[3] == 0) {
				LCD_USART3_SendString("img_wired_4.pic=1");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[3] = 1;
				WS2812X_SedColor(0, 0, 255, 4);
				Key_Select_State[3] = 1;
			}else{
				LCD_USART3_SendString("img_wired_4.pic=4");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[3] = 0;
				WS2812X_SedColor(0, 0, 0, 4);
				Key_Select_State[3] = 0;
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}


	// ADC_Value: 2833
	if(ADC_Value >= 2750 && ADC_Value <= 2950 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			Key_Select_Receiver = 5;
			if(LCD_Wired_Receiver_Select_Status[4] == 0 && Key_Select_State[4] == 0) {
				LCD_USART3_SendString("img_wired_5.pic=1");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[4] = 1;
				WS2812X_SedColor(0, 0, 255, 5);
				Key_Select_State[4] = 1;
			}else{
				LCD_USART3_SendString("img_wired_5.pic=4");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[4] = 0;
				WS2812X_SedColor(0, 0, 0, 5);
				Key_Select_State[4] = 0;
			}
			Key_Flag = 1;
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}

	// ADC_Value: 3506
	if(ADC_Value >= 3450 && ADC_Value <= 3650 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			Key_Select_Receiver = 6;
			if(LCD_Wired_Receiver_Select_Status[5] == 0 && Key_Select_State[5] == 0) {
				LCD_USART3_SendString("img_wired_6.pic=1");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[5] = 1;
				WS2812X_SedColor(0, 0, 255, 6);
				Key_Select_State[5] = 1;
			}else{
				LCD_USART3_SendString("img_wired_6.pic=4");
				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
				LCD_Wired_Receiver_Select_Status[5] = 0;
				WS2812X_SedColor(0, 0, 0, 6);
				Key_Select_State[5] = 0;
			}
			Key_Flag = 1;
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}

	// ADC_Value: 3998
	if(ADC_Value >= 3900 && ADC_Value <= 4010 && Key_Flag == 0) {
		i++; // ADC Value: 532
		if(i >= 1000) {
			// Key_Select_Receiver = 7;
			Key_Select_State[6] = 1;
			WS2812X_SedColor(0, 0, 255, 7);
			LCD_USART3_SendString((unsigned char *)"txt_check.pic=6");
			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
			Key_Check_State = 1;
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	if(Key_Check_State == 1 && Key_Flag == 0) {
		WS2812X_SedColor(0, 0, 0, 7);
		LCD_USART3_SendString((unsigned char *)"txt_check.pic=5");
		LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
		Key_Check_State = 0;
		Key_Select_State[6] = 0;
		LCD_USART3_ClearStruct();
		LCD_USART3_SendString((unsigned char*)"click get_time,1");
		LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
		unsigned char j = 0;
		while(strstr((const char *)LCD_USART3.Buff, "年")== NULL && j < 20) {
				Delay_Ms(50);
				j++;
		}
		if(strstr((const char *)LCD_USART3.Buff, "年") != NULL && strstr((const char *)LCD_USART3.Buff, "月") != NULL) {
				printf("正在写入日志\r\n");
				My_SD_OpenLogFile();
				unsigned char log[80];
				sprintf((char *)log, "%s有线点火端子检测正常\r\n", LCD_USART3.Buff);
				LCD_USART3_ClearStruct();
				My_SD_WriteLog((unsigned char*)log);
				LCD_USART3_ClearStruct();
		}
		LCD_USART3_SendString((unsigned char *)"slt_show_cont.txt=\"\"");
		LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
		LCD_USART3_SendString((unsigned char *)"click show_log,1");
		LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
		My_SD_OpenLogFile();
		My_SD_ReadLog();
	}
}



void Key_ADC2_Scanf(void) {
	static unsigned char Key_Flag = 0;
	static unsigned int i = 0;
	unsigned short ADC_Value = ADC_GetValue(ADC_Channel_13);
	// ADC Value: 0
	if(ADC_Value <= 20 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 8 && Key_Select_State[7] == 1) {
				Key_Select_Receiver = 8;
				Key_Clean_Receiver_Select(Key_Select_State[7]);
			}else{
				Key_Select_State[7] = !Key_Select_State[7];
			    printf("Key8:%d\r\n",Key_Select_State[7]);
				Key_Select_Receiver = 8;
				Key_Clean_Receiver_Select(Key_Select_State[7]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC Value: 794
	if(ADC_Value >= 600 && ADC_Value <= 900 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 9 && Key_Select_State[8] == 1) {
				Key_Select_Receiver = 9;
				Key_Clean_Receiver_Select(Key_Select_State[8]);
			}else{
				Key_Select_State[8] = !Key_Select_State[8];
			    printf("Key9:%d\r\n",Key_Select_State[8]);
				Key_Select_Receiver = 9;
				Key_Clean_Receiver_Select(Key_Select_State[8]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 1540
	if(ADC_Value >= 1400 && ADC_Value <= 1650 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 10 && Key_Select_State[9] == 1) {
				Key_Select_Receiver = 10;
				Key_Clean_Receiver_Select(Key_Select_State[9]);
			}else{
				Key_Select_State[9] = !Key_Select_State[9];
			    printf("Key10:%d\r\n",Key_Select_State[9]);
				Key_Select_Receiver = 10;
				Key_Clean_Receiver_Select(Key_Select_State[9]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 2258
	if(ADC_Value >= 2150 && ADC_Value <= 2350 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 11 && Key_Select_State[10] == 1) {
				Key_Select_Receiver = 11;
				Key_Clean_Receiver_Select(Key_Select_State[10]);
			}else{
				Key_Select_State[10] = !Key_Select_State[10];
			    printf("Key11:%d\r\n",Key_Select_State[10]);
				Key_Select_Receiver = 11;
				Key_Clean_Receiver_Select(Key_Select_State[10]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 2833
	if(ADC_Value >= 2750 && ADC_Value <= 2950 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 12 && Key_Select_State[11] == 1) {
				Key_Select_Receiver = 12;
				Key_Clean_Receiver_Select(Key_Select_State[11]);
			}else{
				Key_Select_State[11] = !Key_Select_State[11];
			    printf("Key12:%d\r\n",Key_Select_State[11]);
				Key_Select_Receiver = 12;
				Key_Clean_Receiver_Select(Key_Select_State[11]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 3506
	if(ADC_Value >= 3450 && ADC_Value <= 3650 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 13 && Key_Select_State[12] == 1) {
				Key_Select_Receiver = 13;
				 Key_Clean_Receiver_Select(Key_Select_State[12]);
			}else{
				Key_Select_State[12] = !Key_Select_State[12];
			    printf("Key13:%d\r\n",Key_Select_State[12]);
				Key_Select_Receiver = 13;
				Key_Clean_Receiver_Select(Key_Select_State[12]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 3998
	if(ADC_Value >= 3900 && ADC_Value <= 4010 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 14 && Key_Select_State[13] == 1) {
				Key_Select_Receiver = 14;
				// Key_Clean_Receiver_Select(Key_Select_State[13]);
			}else{
				Key_Select_State[13] = !Key_Select_State[13];
			    printf("Key14:%d\r\n",Key_Select_State[13]);
				Key_Select_Receiver = 14;
				// Key_Clean_Receiver_Select(Key_Select_State[13]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
}
 




void Key_ADC3_Scanf(void) {
	static unsigned char Key_Flag = 0;
	static unsigned int i = 0;
	unsigned short ADC_Value = ADC_GetValue(ADC_Channel_11);
	// ADC Value: 0
	if(ADC_Value <= 20 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 15 && Key_Select_State[14] == 1) {
				Key_Select_Receiver = 15;
				// Key_Clean_Receiver_Select(Key_Select_State[14]);
			}else{
				Key_Select_State[14] = !Key_Select_State[14];
			    printf("Key15:%d\r\n",Key_Select_State[14]);
				Key_Select_Receiver = 15;
				// Key_Clean_Receiver_Select(Key_Select_State[14]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC Value: 794
	if(ADC_Value >= 600 && ADC_Value <= 900 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 16 && Key_Select_State[15] == 1) {
				Key_Select_Receiver = 16;
				// Key_Clean_Receiver_Select(Key_Select_State[15]);
			}else{
				Key_Select_State[15] = !Key_Select_State[15];
			    printf("Key16:%d\r\n",Key_Select_State[15]);
				Key_Select_Receiver = 16;
				// Key_Clean_Receiver_Select(Key_Select_State[15]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 1540
	if(ADC_Value >= 1400 && ADC_Value <= 1650 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 17 && Key_Select_State[16] == 1) {
				Key_Select_Receiver = 17;
				// Key_Clean_Receiver_Select(Key_Select_State[16]);
			}else{
				Key_Select_State[16] = !Key_Select_State[16];
			    printf("Key17:%d\r\n",Key_Select_State[16]);
				Key_Select_Receiver = 17;
				// Key_Clean_Receiver_Select(Key_Select_State[16]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 2258
	if(ADC_Value >= 2150 && ADC_Value <= 2350 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 18 && Key_Select_State[17] == 1) {
				Key_Select_Receiver = 18;
				// Key_Clean_Receiver_Select(Key_Select_State[17]);
			}else{
				Key_Select_State[17] = !Key_Select_State[17];
			    printf("Key18:%d\r\n",Key_Select_State[17]);
				Key_Select_Receiver = 18;
				// Key_Clean_Receiver_Select(Key_Select_State[17]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 2833
	if(ADC_Value >= 2750 && ADC_Value <= 2950 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 19 && Key_Select_State[18] == 1) {
				Key_Select_Receiver = 19;
				//Key_Clean_Receiver_Select(Key_Select_State[18]);
			}else{
				Key_Select_State[18] = !Key_Select_State[18];
			    printf("Key19:%d\r\n",Key_Select_State[18]);
				Key_Select_Receiver = 19;
				//Key_Clean_Receiver_Select(Key_Select_State[18]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 3506
	if(ADC_Value >= 3450 && ADC_Value <= 3650 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 20 && Key_Select_State[19] == 1) {
				Key_Select_Receiver = 20;
				//Key_Clean_Receiver_Select(Key_Select_State[19]);
			}else{
				Key_Select_State[19] = !Key_Select_State[19];
			    printf("Key20:%d\r\n",Key_Select_State[19]);
				Key_Select_Receiver = 20;
				//Key_Clean_Receiver_Select(Key_Select_State[19]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 3998
	if(ADC_Value >= 3900 && ADC_Value <= 4010 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_State[20] == 0) {
				WS2812X_SedColor(0, 0, 255, 21);
				Key_Select_State[20] = 1;
				LCD_Select_Control_Status[2] = 1;
				LCD_USART3_SendString((unsigned char *)"txt_start.pic=6");
				LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
				printf("Key21:%d\r\n",Key_Select_State[20]);
			}else if(Key_Select_State[20] == 1) {
				WS2812X_SedColor(0, 0, 0, 21);
				Key_Select_State[20] = 0;
				LCD_Select_Control_Status[2] = 0;
				LCD_USART3_SendString((unsigned char *)"txt_start.pic=5");
				LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);

				LCD_USART3_SendString((unsigned char *)"vis btn_confirm,0");
				LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
				LCD_USART3_SendString((unsigned char *)"vis btn_cancel,0");
				LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
				printf("Key21:%d\r\n",Key_Select_State[20]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
}


void Key_ADC4_Scanf(void) {
	static unsigned char Check_RGB_State = 0;
	static unsigned char Key_Flag = 0;
	static unsigned int i = 0;
	unsigned short ADC_Value = ADC_GetValue(ADC_Channel_10);
	/*
	// ADC Value: 0
	if(ADC_Value <= 20 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 22 && Key_Select_State[21] == 1) {
				Key_Select_Receiver = 22;
				// Key_Clean_Receiver_Select(Key_Select_State[21]);
			}else{
				Key_Select_State[21] = !Key_Select_State[21];
			    printf("Key22:%d\r\n",Key_Select_State[21]);
				Key_Select_Receiver = 22;
				// Key_Clean_Receiver_Select(Key_Select_State[21]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC Value: 794
	if(ADC_Value >= 600 && ADC_Value <= 900 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 23 && Key_Select_State[22] == 1) {
				Key_Select_Receiver = 23;
				// Key_Clean_Receiver_Select(Key_Select_State[22]);
			}else{
				Key_Select_State[22] = !Key_Select_State[22];
			    printf("Key23:%d\r\n",Key_Select_State[22]);
				Key_Select_Receiver = 23;
				// Key_Clean_Receiver_Select(Key_Select_State[22]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 1540
	if(ADC_Value >= 1400 && ADC_Value <= 1650 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 24 && Key_Select_State[23] == 1) {
				Key_Select_Receiver = 24;
				// Key_Clean_Receiver_Select(Key_Select_State[23]);
			}else{
				Key_Select_State[23] = !Key_Select_State[23];
			    printf("Key24:%d\r\n",Key_Select_State[23]);
				Key_Select_Receiver = 24;
				// Key_Clean_Receiver_Select(Key_Select_State[23]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 2258
	if(ADC_Value >= 2150 && ADC_Value <= 2350 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 25 && Key_Select_State[24] == 1) {
				Key_Select_Receiver = 25;
				// Key_Clean_Receiver_Select(Key_Select_State[24]);
			}else{
				Key_Select_State[24] = !Key_Select_State[24];
			    printf("Key25:%d\r\n",Key_Select_State[24]);
				Key_Select_Receiver = 25;
				// Key_Clean_Receiver_Select(Key_Select_State[24]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 2833
	if(ADC_Value >= 2750 && ADC_Value <= 2950 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 26 && Key_Select_State[25] == 1) {
				Key_Select_Receiver = 26;
				// Key_Clean_Receiver_Select(Key_Select_State[25]);
			}else{
				Key_Select_State[25] = !Key_Select_State[25];
			    printf("Key26:%d\r\n",Key_Select_State[25]);
				Key_Select_Receiver = 26;
				// Key_Clean_Receiver_Select(Key_Select_State[25]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}
	// ADC_Value: 3506
	if(ADC_Value >= 3450 && ADC_Value <= 3650 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_Receiver != 27 && Key_Select_State[26] == 1) {
				Key_Select_Receiver = 27;
				// Key_Clean_Receiver_Select(Key_Select_State[26]);
			}else{
				Key_Select_State[26] = !Key_Select_State[26];
			    printf("Key27:%d\r\n",Key_Select_State[26]);
				Key_Select_Receiver = 27;
				// Key_Clean_Receiver_Select(Key_Select_State[26]);
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	} */


	// ADC_Value: 3998
	if(ADC_Value >= 3900 && ADC_Value <= 4010 && Key_Flag == 0) {
		i++;
		if(i >= 1000) {
			if(Key_Select_State[20] == 1 && LCD_Select_Control_Status[2] == 1) {
				Key_Select_State[27] = 1;
				WS2812X_SedColor(0, 0, 255, 28);
				Check_RGB_State = 1;
				
			}
			Key_Flag = 1;
		}
	}else if(ADC_Value >= 4070 && Key_Flag == 1) {
		Key_Flag = 0;
		i = 0;
	}

	if(Check_RGB_State == 1 && Key_Flag == 0 && Key_Select_State[27] == 1) {
		if(Public_Key_State == 0) {
			LCD_USART3_SendString((unsigned char *)"vis btn_confirm,1");
			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
			LCD_USART3_SendString((unsigned char *)"vis btn_cancel,1");
			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
			TIM5_Count = 0;
			Public_Key_State = 1;
			TIM_Cmd(TIM5, ENABLE);
		}else if(Public_Key_State == 1 && TIM5_Count < 5) {
			Public_Key_State = 0;
			TIM5_Count = 0;
			TIM_Cmd(TIM5, DISABLE);
			LCD_Ignition_Window_Status = 0;
			LCD_USART3_SendString((unsigned char *)"vis btn_confirm,0");
			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
			LCD_USART3_SendString((unsigned char *)"vis btn_cancel,0");
			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
			Wired_test_count = 0;
			TIM_Cmd(TIM3, ENABLE);
		}
		WS2812X_SedColor(0, 0, 0, 28);
		Check_RGB_State = 0;
		Key_Select_State[27] = 0;
	}
}




// 判断一下是否满足启动条件 , 如果什么都不选, 选择接收机不选择引爆端子, 那不满足启动条件
unsigned char Get_isMeetWith_Start(void) {
	 unsigned char a = Get_Port_Select_Number(Key8_Port_Select);
	 unsigned char b = Get_Port_Select_Number(Key9_Port_Select);
	 unsigned char c = Get_Port_Select_Number(Key10_Port_Select);
	 unsigned char d = Get_Port_Select_Number(Key11_Port_Select);
	 unsigned char e = Get_Port_Select_Number(Key12_Port_Select);
	 unsigned char f = Get_Port_Select_Number(Key13_Port_Select);
	 unsigned char g = Get_Port_Select_Number(Key15_Port_Select);
	 unsigned char h = Get_Port_Select_Number(Key16_Port_Select);
	 unsigned char i = Get_Port_Select_Number(Key17_Port_Select);
	 unsigned char j = Get_Port_Select_Number(Key18_Port_Select);
	 unsigned char k = Get_Port_Select_Number(Key19_Port_Select);
	 unsigned char l = Get_Port_Select_Number(Key20_Port_Select);
	 unsigned char m = (a + b + c + d + e + f + g + h + i + j + k + l);
     if(m > 0) {
		return 1;
	 }else{
		return 0;
	 }
}



// 实现全选或非全选
void Select_All_Port(void) {

	/*
	if(Key_Select_Receiver != 0) {
		unsigned char number = 0;
		if(Key_Select_Receiver == 1 && Key_Select_State[0] == 1) {
			number = Get_Port_Select_Number(Key1_Port_Select);
			unsigned char j = 0;
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					Key1_Port_Select[j] = 1;
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					Key1_Port_Select[j] = 0;
					WS2812X_SedColor(0, 0, 0, i);
				}
				j++;
			}
			number = Get_Port_Select_Number(Key1_Port_Select);
			if(number >= 6) {
				//HC14_Send_Radio_data(0x01, 0x03, 0xff);
			}else{
				//HC14_Send_Radio_data(0x01, 0x04, 0xff);
			}
		}else if(Key_Select_Receiver == 2 && Key_Select_State[1] == 1) {
			number = Get_Port_Select_Number(Key2_Port_Select);
			for (unsigned i = 0; i < 6; i++) {
				if(number < 6) {
					Key2_Port_Select[i] = 1;
				}else{
					Key2_Port_Select[i] = 0;
				}
			}	
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					WS2812X_SedColor(0, 0, 0, i);
				}
			}
		}else if(Key_Select_Receiver == 3 && Key_Select_State[2] == 1) {
			number = Get_Port_Select_Number(Key3_Port_Select);
			for (unsigned i = 0; i < 6; i++) {
				if(number < 6) {
					Key3_Port_Select[i] = 1;
				}else{
					Key3_Port_Select[i] = 0;
				}
			}	
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					WS2812X_SedColor(0, 0, 0, i);
				}
			}
		}else if(Key_Select_Receiver == 4 && Key_Select_State[3] == 1) {
			number = Get_Port_Select_Number(Key4_Port_Select);
			for (unsigned i = 0; i < 6; i++) {
				if(number < 6) {
					Key4_Port_Select[i] = 1;
				}else{
					Key4_Port_Select[i] = 0;
				}
			}	
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					WS2812X_SedColor(0, 0, 0, i);
				}
			}
		}else if(Key_Select_Receiver == 5 && Key_Select_State[4] == 1) {
			number = Get_Port_Select_Number(Key5_Port_Select);
			for (unsigned i = 0; i < 6; i++) {
				if(number < 6) {
					Key5_Port_Select[i] = 1;
				}else{
					Key5_Port_Select[i] = 0;
				}
			}	
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					WS2812X_SedColor(0, 0, 0, i);
				}
			}
		}else if(Key_Select_Receiver == 6 && Key_Select_State[5] == 1) {
			number = Get_Port_Select_Number(Key6_Port_Select);
			for (unsigned i = 0; i < 6; i++) {
				if(number < 6) {
					Key6_Port_Select[i] = 1;
				}else{
					Key6_Port_Select[i] = 0;
				}
			}	
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					WS2812X_SedColor(0, 0, 0, i);
				}
			}
		}else if(Key_Select_Receiver == 7 && Key_Select_State[6] == 1) {
			number = Get_Port_Select_Number(Key7_Port_Select);
			for (unsigned i = 0; i < 6; i++) {
				if(number < 6) {
					Key7_Port_Select[i] = 1;
				}else{
					Key7_Port_Select[i] = 0;
				}
			}	
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					WS2812X_SedColor(0, 0, 0, i);
				}
			}
		}else if(Key_Select_Receiver == 8 && Key_Select_State[7] == 1) {
			number = Get_Port_Select_Number(Key8_Port_Select);
			for (unsigned i = 0; i < 6; i++) {
				if(number < 6) {
					Key8_Port_Select[i] = 1;
				}else{
					Key8_Port_Select[i] = 0;
				}
			}	
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					WS2812X_SedColor(0, 0, 0, i);
				}
			}
		}else if(Key_Select_Receiver == 9 && Key_Select_State[8] == 1) {
			number = Get_Port_Select_Number(Key9_Port_Select);
			for (unsigned i = 0; i < 6; i++) {
				if(number < 6) {
					Key9_Port_Select[i] = 1;
				}else{
					Key9_Port_Select[i] = 0;
				}
			}	
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					WS2812X_SedColor(0, 0, 0, i);
				}
			}
		}else if(Key_Select_Receiver == 10 && Key_Select_State[9] == 1) {
			number = Get_Port_Select_Number(Key10_Port_Select);
			for (unsigned i = 0; i < 6; i++) {
				if(number < 6) {
					Key10_Port_Select[i] = 1;
				}else{
					Key10_Port_Select[i] = 0;
				}
			}	
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					WS2812X_SedColor(0, 0, 0, i);
				}
			}
		}else if(Key_Select_Receiver == 11 && Key_Select_State[10] == 1) {
			number = Get_Port_Select_Number(Key11_Port_Select);
			for (unsigned i = 0; i < 6; i++) {
				if(number < 6) {
					Key11_Port_Select[i] = 1;
				}else{
					Key11_Port_Select[i] = 0;
				}
			}	
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					WS2812X_SedColor(0, 0, 0, i);
				}
			}
		}else if(Key_Select_Receiver == 12 && Key_Select_State[11] == 1) {
			number = Get_Port_Select_Number(Key12_Port_Select);
			for (unsigned i = 0; i < 6; i++) {
				if(number < 6) {
					Key12_Port_Select[i] = 1;
				}else{
					Key12_Port_Select[i] = 0;
				}
			}	
			for(unsigned i = 13; i < 19; i++) {
				if(number < 6) {
					WS2812X_SedColor(0, 0, 255, i);
				}else{
					WS2812X_SedColor(0, 0, 0, i);
				}
			}
		}
	}*/
}



void Select_Port(unsigned char Key_Select_Port) {
	if(Key_Select_Receiver == 8 && Key_Select_State[7] == 1) {
		if (Key_Select_Port == 22) {
			Key8_Port_Select[0] = !Key8_Port_Select[0];
			if (Key8_Port_Select[0] == 1) {
				// HC14_Send_Radio_data(RemoteID[0], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key8_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					// HC14_Send_Radio_data(RemoteID[0], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key8_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					// HC14_Send_Radio_data(RemoteID[0], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key1_Port_Select[1] = !Key1_Port_Select[1];
			if (Key1_Port_Select[1] == 1) {
				// //HC14_Send_Radio_data(RemoteID[0], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key1_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					// //HC14_Send_Radio_data(RemoteID[0], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key1_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					// //HC14_Send_Radio_data(RemoteID[0], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key1_Port_Select[2] = !Key1_Port_Select[2];
			if (Key1_Port_Select[2] == 1) {
				// //HC14_Send_Radio_data(RemoteID[0], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key1_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[0], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key1_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[0], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key1_Port_Select[3] = !Key1_Port_Select[3];
			if (Key1_Port_Select[3] == 1) {
				////HC14_Send_Radio_data(RemoteID[0], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key1_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[0], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key1_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[0], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key1_Port_Select[4] = !Key1_Port_Select[4];
			if (Key1_Port_Select[4] == 1){
				////HC14_Send_Radio_data(RemoteID[0], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key1_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[0], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key1_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[0], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key1_Port_Select[5] = !Key1_Port_Select[5];
			if (Key1_Port_Select[5] == 1) {
				////HC14_Send_Radio_data(RemoteID[0], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key1_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[0], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key1_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[0], 0x04, 0x18);
				}
			}
		}
	}else if(Key_Select_Receiver == 2 && Key_Select_State[1] == 1) {
		if (Key_Select_Port == 13) {
			Key2_Port_Select[0] = !Key2_Port_Select[0];
			if (Key2_Port_Select[0] == 1) {
				////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key2_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key2_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[1], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key2_Port_Select[1] = !Key2_Port_Select[1];
			if (Key2_Port_Select[1] == 1) {
				////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key2_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key2_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[1], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key2_Port_Select[2] = !Key2_Port_Select[2];
			if (Key2_Port_Select[2] == 1) {
				////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key2_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key2_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[1], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key2_Port_Select[3] = !Key2_Port_Select[3];
			if (Key2_Port_Select[3] == 1) {
				////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key2_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key2_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[1], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key2_Port_Select[4] = !Key2_Port_Select[4];
			if (Key2_Port_Select[4] == 1){
				////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key2_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key2_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[1], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key2_Port_Select[5] = !Key2_Port_Select[5];
			if (Key2_Port_Select[5] == 1) {
				////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key2_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[1], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key2_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[1], 0x04, 0x18);
				}
			}
		}
	}else if(Key_Select_Receiver == 3 && Key_Select_State[2] == 1) {
		if (Key_Select_Port == 13) {
			Key3_Port_Select[0] = !Key3_Port_Select[0];
			if (Key3_Port_Select[0] == 1) {
				////HC14_Send_Radio_data(RemoteID[2], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key3_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[2], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key3_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[2], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key3_Port_Select[1] = !Key3_Port_Select[1];
			if (Key3_Port_Select[1] == 1) {
				////HC14_Send_Radio_data(RemoteID[2], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key3_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[2], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key3_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					////HC14_Send_Radio_data(RemoteID[2], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key3_Port_Select[2] = !Key3_Port_Select[2];
			if (Key3_Port_Select[2] == 1) {
				////HC14_Send_Radio_data(RemoteID[2], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key3_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					////HC14_Send_Radio_data(RemoteID[2], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key3_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					// //HC14_Send_Radio_data(RemoteID[2], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key3_Port_Select[3] = !Key3_Port_Select[3];
			if (Key3_Port_Select[3] == 1) {
				//HC14_Send_Radio_data(RemoteID[2], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key3_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[2], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key3_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[2], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key3_Port_Select[4] = !Key3_Port_Select[4];
			if (Key3_Port_Select[4] == 1){
				//HC14_Send_Radio_data(RemoteID[2], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key3_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[2], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key3_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[2], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key3_Port_Select[5] = !Key3_Port_Select[5];
			if (Key3_Port_Select[5] == 1) {
				//HC14_Send_Radio_data(RemoteID[2], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key3_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[2], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key3_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[2], 0x04, 0x18);
				}
			}
		}
	}else if(Key_Select_Receiver == 4 && Key_Select_State[3] == 1) {
		if (Key_Select_Port == 13) {
			Key4_Port_Select[0] = !Key4_Port_Select[0];
			if (Key4_Port_Select[0] == 1) {
				//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key4_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key4_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[3], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key4_Port_Select[1] = !Key4_Port_Select[1];
			if (Key4_Port_Select[1] == 1) {
				//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key4_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key4_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[3], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key4_Port_Select[2] = !Key4_Port_Select[2];
			if (Key4_Port_Select[2] == 1) {
				//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key4_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key4_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[3], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key4_Port_Select[3] = !Key4_Port_Select[3];
			if (Key4_Port_Select[3] == 1) {
				//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key4_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key4_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[3], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key4_Port_Select[4] = !Key4_Port_Select[4];
			if (Key4_Port_Select[4] == 1){
				//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key4_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key4_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[3], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key4_Port_Select[5] = !Key4_Port_Select[5];
			if (Key4_Port_Select[5] == 1) {
				//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key4_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[3], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key4_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[3], 0x04, 0x18);
				}
			}
		}
	}else if(Key_Select_Receiver == 5 && Key_Select_State[4] == 1) {
		if (Key_Select_Port == 13) {
			Key5_Port_Select[0] = !Key5_Port_Select[0];
			if (Key5_Port_Select[0] == 1) {
				//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key5_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key5_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[4], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key5_Port_Select[1] = !Key5_Port_Select[1];
			if (Key5_Port_Select[1] == 1) {
				//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key5_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key5_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[4], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key5_Port_Select[2] = !Key5_Port_Select[2];
			if (Key5_Port_Select[2] == 1) {
				//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key5_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key5_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[4], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key5_Port_Select[3] = !Key5_Port_Select[3];
			if (Key5_Port_Select[3] == 1) {
				//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key5_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key5_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[4], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key5_Port_Select[4] = !Key5_Port_Select[4];
			if (Key5_Port_Select[4] == 1){
				//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key5_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key5_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[4], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key5_Port_Select[5] = !Key5_Port_Select[5];
			if (Key5_Port_Select[5] == 1) {
				//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key5_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[4], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key5_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[4], 0x04, 0x18);
				}
			}
		}
	}else if(Key_Select_Receiver == 6 && Key_Select_State[5] == 1) {
		if (Key_Select_Port == 13) {
			Key6_Port_Select[0] = !Key6_Port_Select[0];
			if (Key6_Port_Select[0] == 1) {
				//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key6_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key6_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[5], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key6_Port_Select[1] = !Key6_Port_Select[1];
			if (Key6_Port_Select[1] == 1) {
				//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key6_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key6_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[5], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key6_Port_Select[2] = !Key6_Port_Select[2];
			if (Key6_Port_Select[2] == 1) {
				//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key6_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key6_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[5], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key6_Port_Select[3] = !Key6_Port_Select[3];
			if (Key6_Port_Select[3] == 1) {
				//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key6_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key6_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[5], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key6_Port_Select[4] = !Key6_Port_Select[4];
			if (Key6_Port_Select[4] == 1){
				//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key6_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key6_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[5], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key6_Port_Select[5] = !Key6_Port_Select[5];
			if (Key6_Port_Select[5] == 1) {
				//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key6_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[5], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key6_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[5], 0x04, 0x18);
				}
			}
		}
	}else if(Key_Select_Receiver == 7 && Key_Select_State[6] == 1) {
		if (Key_Select_Port == 13) {
			Key7_Port_Select[0] = !Key7_Port_Select[0];
			if (Key7_Port_Select[0] == 1) {
				//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key7_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key7_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[6], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key7_Port_Select[1] = !Key7_Port_Select[1];
			if (Key7_Port_Select[1] == 1) {
				//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key7_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key7_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[6], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key7_Port_Select[2] = !Key7_Port_Select[2];
			if (Key7_Port_Select[2] == 1) {
				//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key7_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key7_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[6], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key7_Port_Select[3] = !Key7_Port_Select[3];
			if (Key7_Port_Select[3] == 1) {
				//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key7_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key7_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[6], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key7_Port_Select[4] = !Key7_Port_Select[4];
			if (Key7_Port_Select[4] == 1){
				//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key7_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key7_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[6], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key7_Port_Select[5] = !Key7_Port_Select[5];
			if (Key7_Port_Select[5] == 1) {
				//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key7_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[6], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key7_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[6], 0x04, 0x18);
				}
			}
		}
	}else if(Key_Select_Receiver == 8 && Key_Select_State[7] == 1) {
		if (Key_Select_Port == 13) {
			Key8_Port_Select[0] = !Key8_Port_Select[0];
			if (Key8_Port_Select[0] == 1) {
				//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key8_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key8_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[7], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key8_Port_Select[1] = !Key8_Port_Select[1];
			if (Key8_Port_Select[1] == 1) {
				//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key8_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key8_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[7], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key8_Port_Select[2] = !Key8_Port_Select[2];
			if (Key8_Port_Select[2] == 1) {
				//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key8_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key8_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[7], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key8_Port_Select[3] = !Key8_Port_Select[3];
			if (Key8_Port_Select[3] == 1) {
				//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key8_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key8_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[7], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key8_Port_Select[4] = !Key8_Port_Select[4];
			if (Key8_Port_Select[4] == 1){
				//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key8_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key8_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[7], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key8_Port_Select[5] = !Key8_Port_Select[5];
			if (Key8_Port_Select[5] == 1) {
				//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key8_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[7], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key8_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[7], 0x04, 0x18);
				}
			}
		}
	}else if(Key_Select_Receiver == 9 && Key_Select_State[8] == 1) {
		if (Key_Select_Port == 13) {
			Key9_Port_Select[0] = !Key9_Port_Select[0];
			if (Key9_Port_Select[0] == 1) {
				//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key9_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key9_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[8], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key9_Port_Select[1] = !Key9_Port_Select[1];
			if (Key9_Port_Select[1] == 1) {
				//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key9_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key9_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[8], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key9_Port_Select[2] = !Key9_Port_Select[2];
			if (Key9_Port_Select[2] == 1) {
				//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key9_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key9_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[8], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key9_Port_Select[3] = !Key9_Port_Select[3];
			if (Key9_Port_Select[3] == 1) {
				//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key9_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key9_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[8], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key9_Port_Select[4] = !Key9_Port_Select[4];
			if (Key9_Port_Select[4] == 1){
				//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key9_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key9_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[8], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key9_Port_Select[5] = !Key9_Port_Select[5];
			if (Key9_Port_Select[5] == 1) {
				//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key9_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[8], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key9_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[8], 0x04, 0x18);
				}
			}
		}
	}else if(Key_Select_Receiver == 10 && Key_Select_State[9] == 1) {
		if (Key_Select_Port == 13) {
			Key10_Port_Select[0] = !Key10_Port_Select[0];
			if (Key10_Port_Select[0] == 1) {
				//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key10_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key10_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[9], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key10_Port_Select[1] = !Key10_Port_Select[1];
			if (Key10_Port_Select[1] == 1) {
				//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key10_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key10_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[9], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key10_Port_Select[2] = !Key10_Port_Select[2];
			if (Key10_Port_Select[2] == 1) {
				//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key10_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key10_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[9], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key10_Port_Select[3] = !Key10_Port_Select[3];
			if (Key10_Port_Select[3] == 1) {
				//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key10_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key10_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[9], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key10_Port_Select[4] = !Key10_Port_Select[4];
			if (Key10_Port_Select[4] == 1){
				//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key10_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key10_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[9], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key10_Port_Select[5] = !Key10_Port_Select[5];
			if (Key10_Port_Select[5] == 1) {
				//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key10_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[9], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key10_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[9], 0x04, 0x18);
				}
			}
		}
	}else if(Key_Select_Receiver == 11 && Key_Select_State[10] == 1) {
		if (Key_Select_Port == 13) {
			Key11_Port_Select[0] = !Key11_Port_Select[0];
			if (Key11_Port_Select[0] == 1) {
				//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key11_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key11_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[10], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key11_Port_Select[1] = !Key11_Port_Select[1];
			if (Key11_Port_Select[1] == 1) {
				//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key11_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key11_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[10], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key11_Port_Select[2] = !Key11_Port_Select[2];
			if (Key11_Port_Select[2] == 1) {
				//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key11_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key11_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[10], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key11_Port_Select[3] = !Key11_Port_Select[3];
			if (Key11_Port_Select[3] == 1) {
				//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key11_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key11_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[10], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key11_Port_Select[4] = !Key11_Port_Select[4];
			if (Key11_Port_Select[4] == 1){
				//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key11_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key11_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[10], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key11_Port_Select[5] = !Key11_Port_Select[5];
			if (Key11_Port_Select[5] == 1) {
				//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key11_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[10], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key11_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[10], 0x04, 0x18);
				}
			}
		}
	}else if(Key_Select_Receiver == 12 && Key_Select_State[11] == 1) {
		if (Key_Select_Port == 13) {
			Key12_Port_Select[0] = !Key12_Port_Select[0];
			if (Key12_Port_Select[0] == 1) {
				//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x13);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key12_Port_Select[0] == 0) {
				if (RGB_Colour[9][0] == 0 && RGB_Colour[9][1] > 0 && RGB_Colour[9][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x13);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key12_Port_Select[0] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[11], 0x04, 0x13);
				}
			}
		}else if (Key_Select_Port == 14) {
			Key12_Port_Select[1] = !Key12_Port_Select[1];
			if (Key12_Port_Select[1] == 1) {
				//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x14);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key12_Port_Select[1] == 0) {
				if (RGB_Colour[8][0] == 0 && RGB_Colour[8][1] > 0 && RGB_Colour[8][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x14);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key12_Port_Select[1] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[11], 0x04, 0x14);
				}
			}
		}else if (Key_Select_Port == 15) {
			Key12_Port_Select[2] = !Key12_Port_Select[2];
			if (Key12_Port_Select[2] == 1) {
				//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x15);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key12_Port_Select[2] == 0) {
				if (RGB_Colour[7][0] == 0 && RGB_Colour[7][1] > 0 && RGB_Colour[7][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x15);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key12_Port_Select[2] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[11], 0x04, 0x15);
				}
			}
		}else if (Key_Select_Port == 16) {
			Key12_Port_Select[3] = !Key12_Port_Select[3];
			if (Key12_Port_Select[3] == 1) {
				//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x16);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key12_Port_Select[3] == 0) {
				if (RGB_Colour[4][0] == 0 && RGB_Colour[4][1] > 0 && RGB_Colour[4][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x16);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key12_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[11], 0x04, 0x16);
				}
			}
		}else if (Key_Select_Port == 17){
			Key12_Port_Select[4] = !Key12_Port_Select[4];
			if (Key12_Port_Select[4] == 1){
				//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x17);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key12_Port_Select[4] == 0) {
				if (RGB_Colour[5][0] == 0 && RGB_Colour[5][1] > 0 && RGB_Colour[5][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x17);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key12_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[11], 0x04, 0x17);
				}
			}
		}else if (Key_Select_Port == 18) {
			Key12_Port_Select[5] = !Key12_Port_Select[5];
			if (Key12_Port_Select[5] == 1) {
				//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x18);
				WS2812X_SedColor(0, 0, 255, Key_Select_Port);
			}else if (Key12_Port_Select[5] == 0) {
				if (RGB_Colour[6][0] == 0 && RGB_Colour[6][1] > 0 && RGB_Colour[6][2] == 0) {
					//HC14_Send_Radio_data(RemoteID[11], 0x03, 0x18);
					WS2812X_SedColor(0, 0, 255, Key_Select_Port);
					Key12_Port_Select[3] = 1;
				}else{
					WS2812X_SedColor(0, 0, 0, Key_Select_Port);
					//HC14_Send_Radio_data(RemoteID[11], 0x04, 0x18);
				}
			}
		}
	}
}
  
  


// 选中的接收机, 如果没有选则任何接线端子, 那么将会把这个选中状态释放掉
unsigned char Get_Port_Select_Number(unsigned char *Key_Port_Select) {
	unsigned char number = 0;
	for (unsigned char i = 0; i < 6; ++i) {
        number += Key_Port_Select[i];
    }
    return number;
}



// 清除选择接收机 , 如果当前接收机选择，没有选择任何一个端子，将会把这个选择清空
void Key_Clean_Receiver_Select(unsigned char Select_State) {
	unsigned char *Key_Port_Select[] = {
			Key8_Port_Select, 
			Key9_Port_Select, 
			Key10_Port_Select, 
			Key11_Port_Select, 
			Key12_Port_Select,
			Key13_Port_Select, 
			Key15_Port_Select, 
			Key16_Port_Select, 
			Key17_Port_Select, 
			Key18_Port_Select,
			Key19_Port_Select, 
			Key20_Port_Select
	};
 
	if(Select_State == 1)  
    {
		WS2812X_SedColor(0,0,255,Key_Select_Receiver);
		if(Key_Select_Receiver != 8) 
        {
			if(Get_Port_Select_Number(Key_Port_Select[0]) == 0 && Key_Select_State[7] == 1) {
				WS2812X_SedColor(0,0,0,8); 
				Key_Select_State[7] = 0;
			}
		}

		if(Key_Select_Receiver != 9) {
			if(Get_Port_Select_Number(Key_Port_Select[1]) == 0 && Key_Select_State[8] == 1) {
				WS2812X_SedColor(0,0,0,9); 
				Key_Select_State[8] = 0;
			}
		}

		if(Key_Select_Receiver != 10) {
			if(Get_Port_Select_Number(Key_Port_Select[2]) == 0 && Key_Select_State[9] == 1) {
				WS2812X_SedColor(0,0,0,10); 
				Key_Select_State[9] = 0;
			}
		}

		if(Key_Select_Receiver != 11) {
			if(Get_Port_Select_Number(Key_Port_Select[3]) == 0 && Key_Select_State[10] == 1) {
				WS2812X_SedColor(0,0,0,11); 
				Key_Select_State[10] = 0;
			}
		}

		if(Key_Select_Receiver != 12) {
			if(Get_Port_Select_Number(Key_Port_Select[4]) == 0 && Key_Select_State[11] == 1) {
				WS2812X_SedColor(0,0,0,12); 
				Key_Select_State[11] = 0;
			}
		}

		if(Key_Select_Receiver != 13) {
			if(Get_Port_Select_Number(Key_Port_Select[5]) == 0 && Key_Select_State[12] == 1) {
				WS2812X_SedColor(0,0,0,13); 
				Key_Select_State[12] = 0;
			}
		}

		if(Key_Select_Receiver != 15) {
			if(Get_Port_Select_Number(Key_Port_Select[7]) == 0 && Key_Select_State[14] == 1) {
				WS2812X_SedColor(0,0,0,15); 
				Key_Select_State[14] = 0;
			}
		}

		if(Key_Select_Receiver != 16) {
			if(Get_Port_Select_Number(Key_Port_Select[8]) == 0 && Key_Select_State[15] == 1) {
				WS2812X_SedColor(0,0,0,16); 
				Key_Select_State[15] = 0;
			}
		}

		if(Key_Select_Receiver != 17) {
			if(Get_Port_Select_Number(Key_Port_Select[9]) == 0 && Key_Select_State[16] == 1) {
				WS2812X_SedColor(0,0,0,17); 
				Key_Select_State[16] = 0;
			}
		}

		if(Key_Select_Receiver != 18) {
			if(Get_Port_Select_Number(Key_Port_Select[10]) == 0 && Key_Select_State[17] == 1) {
				WS2812X_SedColor(0,0,0,18); 
				Key_Select_State[17] = 0;
			}
		}

		if(Key_Select_Receiver != 19) {
			if(Get_Port_Select_Number(Key_Port_Select[11]) == 0 && Key_Select_State[18] == 1) {
				WS2812X_SedColor(0,0,0,19); 
				Key_Select_State[18] = 0;
			}
		}

		if(Key_Select_Receiver != 20) {
			if(Get_Port_Select_Number(Key_Port_Select[12]) == 0 && Key_Select_State[19] == 1) {
				WS2812X_SedColor(0,0,0,20); 
				Key_Select_State[19] = 0;
			}
		}



		for(unsigned char i = 0; i < 6; i++) {
			if(Key_Select_Receiver == 8) {
				if(i == 0 && Key8_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key8_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key8_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key8_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key8_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key8_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key8_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key8_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key8_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key8_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key8_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key8_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}else if(Key_Select_Receiver == 9) {
				if(i == 0 && Key9_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key9_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key9_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key9_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key9_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key9_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key9_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key9_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key9_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key9_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key9_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key9_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}else if(Key_Select_Receiver == 10) {
				if(i == 0 && Key10_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key10_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key10_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key10_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key10_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key10_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key10_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key10_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key10_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key10_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key10_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key10_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}else if(Key_Select_Receiver == 11) {
				if(i == 0 && Key11_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key11_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key11_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key11_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key11_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key11_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key11_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key11_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key11_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key11_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key11_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key11_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}else if(Key_Select_Receiver == 12) {
				if(i == 0 && Key12_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key12_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key12_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key12_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key12_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key12_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key12_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key12_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key12_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key12_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key12_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key12_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}else if(Key_Select_Receiver == 13) {
				if(i == 0 && Key13_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key13_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key13_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key13_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key13_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key13_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key13_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key13_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key13_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key13_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key13_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key13_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}else if(Key_Select_Receiver == 15) {
				if(i == 0 && Key15_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key15_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key15_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key15_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key15_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key15_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key15_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key15_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key15_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key15_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key15_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key15_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}else if(Key_Select_Receiver == 16) {
				if(i == 0 && Key16_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key16_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key16_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key16_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key16_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key16_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key16_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key16_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key16_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key16_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key16_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key16_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}else if(Key_Select_Receiver == 17) {
				if(i == 0 && Key17_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key17_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key17_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key17_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key17_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key17_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key17_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key17_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key17_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key17_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key17_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key17_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}else if(Key_Select_Receiver == 18) {
				if(i == 0 && Key18_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key18_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key18_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key18_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key18_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key18_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key18_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key18_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key18_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key18_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key18_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key18_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}else if(Key_Select_Receiver == 19) {
				if(i == 0 && Key19_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key19_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key19_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key19_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key19_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key19_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key19_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key19_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key19_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key19_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key19_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key19_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}else if(Key_Select_Receiver == 20) {
				if(i == 0 && Key20_Port_Select[0] == 1) {
					WS2812X_SedColor(0,0,255,22);
				}else if(i == 0 && Key20_Port_Select[0] == 0) {
					WS2812X_SedColor(0,0,0,22);
				}else if(i == 1 && Key20_Port_Select[1] == 1) {
					WS2812X_SedColor(0,0,255,23);
				}else if(i == 1 && Key20_Port_Select[1] == 0) {
					WS2812X_SedColor(0,0,0,23);
				}else if(i == 2 && Key20_Port_Select[2] == 1) {
					WS2812X_SedColor(0,0,255,24);
				}else if(i == 2 && Key20_Port_Select[2] == 0) {
					WS2812X_SedColor(0,0,0,24);
				}else if(i == 3 && Key20_Port_Select[3] == 1) {
					WS2812X_SedColor(0,0,255,25);
				}else if(i == 3 && Key20_Port_Select[3] == 0) {
					WS2812X_SedColor(0,0,0,25);
				}else if(i == 4 && Key20_Port_Select[4] == 1) {
					WS2812X_SedColor(0,0,255,26);
				}else if(i == 4 && Key20_Port_Select[4] == 0) {
					WS2812X_SedColor(0,0,0,26);
				}else if(i == 5 && Key20_Port_Select[5] == 1) {
					WS2812X_SedColor(0,0,255,27);
				}else if(i == 5 && Key20_Port_Select[5] == 0) {
					WS2812X_SedColor(0,0,0,27);
				}
			}
		}
	}else{
		// 不选中, 直接把当前RGB置无色
		WS2812X_SedColor(0,0,0,Key_Select_Receiver); 
		for (unsigned char i = 22; i < 28; i++) {
			WS2812X_SedColor(0,0,0,i);
		}

		if(Key_Select_Receiver == 8) {
			for (unsigned char i = 0; i < 6; i++) {
				Key8_Port_Select[i] = 0;
			}
		}

		if(Key_Select_Receiver == 9) {
			for (unsigned char i = 0; i < 6; i++) {
				Key9_Port_Select[i] = 0;
			}
		}

		if(Key_Select_Receiver == 10) {
			for (unsigned char i = 0; i < 6; i++) {
				Key10_Port_Select[i] = 0;
			}
		}

		if(Key_Select_Receiver == 11) {
			for (unsigned char i = 0; i < 6; i++) {
				Key11_Port_Select[i] = 0;
			}
		}

		if(Key_Select_Receiver == 12) {
			for (unsigned char i = 0; i < 6; i++) {
				Key12_Port_Select[i] = 0;
			}
		}

		if(Key_Select_Receiver == 13) {
			for (unsigned char i = 0; i < 6; i++) {
				Key13_Port_Select[i] = 0;
			}
		}

		if(Key_Select_Receiver == 15) {
			for (unsigned char i = 0; i < 6; i++) {
				Key15_Port_Select[i] = 0;
			}
		}

		if(Key_Select_Receiver == 16) {
			for (unsigned char i = 0; i < 6; i++) {
				Key16_Port_Select[i] = 0;
			}
		}

		if(Key_Select_Receiver == 17) {
			for (unsigned char i = 0; i < 6; i++) {
				Key17_Port_Select[i] = 0;
			}
		}

		if(Key_Select_Receiver == 18) {
			for (unsigned char i = 0; i < 6; i++) {
				Key18_Port_Select[i] = 0;
			}
		}

		if(Key_Select_Receiver == 19) {
			for (unsigned char i = 0; i < 6; i++) {
				Key19_Port_Select[i] = 0;
			}
		}

		if(Key_Select_Receiver == 20) {
			for (unsigned char i = 0; i < 6; i++) {
				Key20_Port_Select[i] = 0;
			}
		}
	}
}
