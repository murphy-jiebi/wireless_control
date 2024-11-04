#include "LCD.h"
#include "LCD_USART3.h"
#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"
#include "bsp_ws281x.h"
#include "delay.h"
#include "my_sd.h"
#include "key.h"
#include "tim.h"
#include "bsp_gpio.h"
#include "adc.h"
#include "bat.h"

unsigned char LCD_Log_Window_Status;      // 操作日志弹窗状态(1:弹出状态 0:关闭状态)
unsigned char LCD_Ignition_Window_Status; // 引爆确认弹窗状态(1:弹出状态 0:关闭状态)
unsigned char LCD_Sleep_State = 0;        // 显示屏睡眠状态(1:睡眠状态 0:非睡眠状态)


unsigned char LCD_Wired_Receiver_Select_Status[6] = {0,0,0,0,0,0};  // 有线接收机控制区（选择状态）
unsigned char LCD_Select_Control_Status[4] = {0,0,0,0};             // 决策控制区（选择状态）




void LCD_Rest(void) {
    LCD_USART3_SendString((unsigned char*)"rest");
	LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
}



unsigned char* LCD_Get_RTC_Time(void) {
	static unsigned char buffer[50];
	memset(buffer, 0, sizeof(buffer));
	LCD_USART3_ClearStruct();
	LCD_USART3_SendString((unsigned char*)"click get_time,1");
	LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
	unsigned char i = 0;
	while(i < 30) {
		if(strstr((const char *)LCD_USART3.Buff, "年") != NULL && strstr((const char *)LCD_USART3.Buff, "月") != NULL) {
			break;
		}
		Delay_Ms(50);
		i++;
	}
	if(i < 30) {
		strncpy((char *)buffer, (const char *)LCD_USART3.Buff, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
		printf("RTC Time:%s\r\n",LCD_USART3.Buff);
	}else{
		 snprintf((char *)buffer, 50, "1970年01月01日00时00分00秒");
		 printf("RTC Time: Error: Timeout\r\n");
	}
	LCD_USART3_ClearStruct();
	return (unsigned char*)buffer;
}




// 解析LCD返回串口的数据
//void Parsing_LCD_Response_Data(unsigned char *LCD_USART3_DATA) 
//{
//	if (strncmp("#2#", (const char *)LCD_USART3.Buff, 3) == 0 && LCD_Ignition_Window_Status != 1 && LCD_Select_Control_Status[2] != 1) {
//		// printf("显示屏:有线接收机控制区按下\r\n");
//		for (unsigned char i = 1; i < 7; i++) {
//			unsigned char cmd[7];
//			sprintf((char *)cmd, "#2#%d#", i);
//			if (strstr((const char *)LCD_USART3.Buff, (const char *)cmd) != NULL) {
//				unsigned char img[20];
//				if (LCD_Wired_Receiver_Select_Status[i - 1] == 0) {
//					sprintf((char *)img, "img_wired_%d.pic=1", i);
//					LCD_Wired_Receiver_Select_Status[i - 1] = 1;
//					Key_Select_State[i-1] = 1;
//					WS2812X_SedColor(0, 0, 255, i);
//				}else if (LCD_Wired_Receiver_Select_Status[i - 1] == 1) {
//					sprintf((char *)img, "img_wired_%d.pic=4", i);
//					LCD_Wired_Receiver_Select_Status[i - 1] = 0;
//					Key_Select_State[i-1] = 0;
//					WS2812X_SedColor(0, 0, 0, i);
//				}
//				LCD_USART3_SendString(img);
//				LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
//				break;
//			}
//		}
//		
//	}
//    else if (strncmp("#3#", (const char *)LCD_USART3.Buff, 3) == 0)
//    {
//        
//    }
//    else if (strncmp("#1#", (const char *)LCD_USART3.Buff, 3) == 0) {
//		if (strstr((const char *)LCD_USART3.Buff, "#1#1#") != NULL && LCD_Ignition_Window_Status != 1) {
//			    // printf("显示屏:检测键按下\r\n");
//				WS2812X_SedColor(0, 0, 255, 7);
//				LCD_USART3_SendString((unsigned char *)"txt_check.pic=6");
//				LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//				// Delay_Ms(100);
//				WS2812X_SedColor(0, 0, 0, 7);
//				LCD_USART3_SendString((unsigned char *)"txt_check.pic=5");
//				LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			    // 获取显示屏RTC时钟时间
//				unsigned char* Log_Time = LCD_Get_RTC_Time();
//				if(strstr((const char *)Log_Time, "年") != NULL && strstr((const char *)Log_Time, "月") != NULL) {
//					My_SD_OpenLogFile();
//					unsigned char Log[80];
//					sprintf((char *)Log, "%s有线接收机端子检测正常\r\n", Log_Time);
//					LCD_USART3_ClearStruct();
//					My_SD_WriteLog((unsigned char*)Log);
//					LCD_USART3_ClearStruct();
//				}
//				LCD_USART3_SendString((unsigned char *)"slt_show_cont.txt=\"\"");
//				LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//				LCD_USART3_SendString((unsigned char *)"click show_log,1");
//				LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//				// SD卡读取记录的日志
//				My_SD_OpenLogFile();
//				My_SD_ReadLog();

//		}else if(strstr((const char *)LCD_USART3.Buff, "#1#3#") != NULL && LCD_Ignition_Window_Status != 1) {
//				// printf("启动键按下\r\n");
//			    if(LCD_Select_Control_Status[2] == 0) {
//					LCD_Select_Control_Status[2] = 1;
//					LCD_USART3_SendString((unsigned char *)"txt_start.pic=6");
//					LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//					WS2812X_SedColor(0, 0, 255, 21);
//					Key_Select_State[20] = 1;
//				}else if(LCD_Select_Control_Status[2] == 1){
//					LCD_Select_Control_Status[2] = 0;
//					LCD_USART3_SendString((unsigned char *)"txt_start.pic=5");
//					LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//					WS2812X_SedColor(0, 0, 0, 21);
//					Key_Select_State[20] = 0;
//				}
//		}else if(strstr((const char *)LCD_USART3.Buff, "#1#4#") != NULL) {
//				
//				if(LCD_Select_Control_Status[2] == 1) {
//					
//					if(LCD_Select_Control_Status[3] == 0) {
//						LCD_Select_Control_Status[3] = 1;
//						LCD_USART3_SendString((unsigned char *)"txt_ignition.pic=6");
//						LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//						LCD_USART3_SendString((unsigned char *)"vis btn_confirm,1");
//						LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//						LCD_USART3_SendString((unsigned char *)"vis btn_cancel,1");
//						LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//						LCD_USART3_SendString((unsigned char *)"txt_ignition.pic=5");
//						LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//						LCD_Ignition_Window_Status = 1;
//						
//						if(Public_Key_State == 0) {
//							TIM5_Count = 0;
//							Public_Key_State = 1;
//							TIM_Cmd(TIM5, ENABLE);
//						}
//					}
//				}
//		}
//	}else if (strncmp("#7#", (const char *)LCD_USART3.Buff, 3) == 0) {
//		// 确认点火
//		if (strstr((const char *)LCD_USART3.Buff, "#7#1#") != NULL) {
//			LCD_USART3_SendString((unsigned char *)"vis btn_confirm,0");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_USART3_SendString((unsigned char *)"vis btn_cancel,0");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_USART3_SendString((unsigned char *)"txt_start.pic=5");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_Ignition_Window_Status = 0;
//			LCD_Select_Control_Status[2] = 0;
//			LCD_Select_Control_Status[3] = 0;

//			WS2812X_SedColor(0, 0, 0, 21);
//			Key_Select_State[20] = 0;
//			
//			Public_Key_State = 0;
//			TIM5_Count = 0;
//			TIM_Cmd(TIM5, DISABLE);
//			
//			unsigned char* Log_Time = LCD_Get_RTC_Time();
//			if(strstr((const char *)Log_Time, "年") != NULL && strstr((const char *)Log_Time, "月") != NULL) {
//				My_SD_OpenLogFile();
//				unsigned char Log[80];
//				sprintf((char *)Log, "%s确认执行点火指令\r\n", Log_Time);
//				LCD_USART3_ClearStruct();
//				My_SD_WriteLog((unsigned char*)Log);
//				LCD_USART3_ClearStruct();
//			}
//			 Wired_test_count = 0;
//			 TIM_Cmd(TIM3, ENABLE);
//			
//		
//		}else if(strstr((const char *)LCD_USART3.Buff, "#7#2#") != NULL) {
//			
//			LCD_USART3_SendString((unsigned char *)"vis btn_confirm,0");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_USART3_SendString((unsigned char *)"vis btn_cancel,0");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_USART3_SendString((unsigned char *)"txt_start.pic=5");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_Ignition_Window_Status = 0;
//			LCD_Select_Control_Status[2] = 0;
//			LCD_Select_Control_Status[3] = 0;

//			WS2812X_SedColor(0, 0, 0, 21);
//			Key_Select_State[20] = 0;
//			
//			RESET_WIRE_L1;
//            RESET_WIRE_L2;
//            RESET_WIRE_M1;
//            RESET_WIRE_M2;
//            RESET_WIRE_R1;
//            RESET_WIRE_R2;
//			
//			Public_Key_State = 0;
//			TIM5_Count = 0;
//			TIM_Cmd(TIM5, DISABLE);
//			
//			unsigned char* Log_Time = LCD_Get_RTC_Time();
//			if(strstr((const char *)Log_Time, "年") != NULL && strstr((const char *)Log_Time, "月") != NULL) {
//				My_SD_OpenLogFile();
//				unsigned char Log[80];
//				sprintf((char *)Log, "%s取消执行点火指令\r\n", Log_Time);
//				LCD_USART3_ClearStruct();
//				My_SD_WriteLog((unsigned char*)Log);
//				LCD_USART3_ClearStruct();
//			}
//		}
//	}else if (strncmp("#6#", (const char *)LCD_USART3.Buff, 3) == 0) {
//		// 弹出日志弹窗, 显示屏串口发送: #6#6#
//		LCD_Log_Window_Status = 1;
//	}else if (strncmp("#9#", (const char *)LCD_USART3.Buff, 3) == 0) {
//		// 关闭日志弹窗, 显示屏串口发送: #9#9#
//		LCD_Log_Window_Status = 0;
//	}else if (strncmp("#8#", (const char *)LCD_USART3.Buff, 3) == 0) {
//		// 按下弹出日志
//		if (strstr((const char *)LCD_USART3.Buff, "#8#1#") != NULL && LCD_Ignition_Window_Status != 1) {
//			LCD_USART3_SendString((unsigned char *)"slt_show_cont.txt=\"\"");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_USART3_SendString((unsigned char *)"click show_log,1");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			// SD卡读取记录的日志
//			My_SD_OpenLogFile();
//			My_SD_ReadLog();
//		}else if(strstr((const char *)LCD_USART3.Buff, "#8#2#") != NULL && LCD_Ignition_Window_Status != 1) {
//			LCD_USART3_SendString((unsigned char *)"set_time_page.set_nian.val=rtc0");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_USART3_SendString((unsigned char *)"set_time_page.set_yue.val=rtc1");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_USART3_SendString((unsigned char *)"set_time_page.set_ri.val=rtc2");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_USART3_SendString((unsigned char *)"set_time_page.set_ri.val=rtc3");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_USART3_SendString((unsigned char *)"set_time_page.set_ri.val=rtc4");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_USART3_SendString((unsigned char *)"set_time_page.set_ri.val=rtc5");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//			LCD_USART3_SendString((unsigned char *)"page set_time_page");
//			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//		}
//	}else if(LCD_USART3.Buff[0] == 0x86 && LCD_USART3.Buff[1] == 0xFF && LCD_USART3.Buff[2] == 0xFF) {
//		// printf("进入休眠模式\r\n");
//		LCD_Sleep_State = 1;
//	}else if(LCD_Sleep_State == 1 && LCD_USART3.Flag ==1 && LCD_USART3.Count > 0) {
//		// printf("退出休眠模式\r\n");
//		LCD_Sleep_State = 0;
//	}
//}

void BatLevelRefresh(void)
{
    if (TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == SET)
    {
        if (LCD_Sleep_State != 1)
        {
            static unsigned char Last_bat = 0;
            float value = (float)Get_BAT_voltage();
            if (value >= 12.0)
            {
                LCD_USART3_SendString((unsigned char *)"txt_ele_level.txt=\"100%\"");
                LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
            }
            else
            {
                unsigned char bat = (unsigned char)round(Get_BAT_mapFloat(value, 9, 12, 1, 100));
                if (Last_bat == 0 || (bat + 1) < Last_bat || (Last_bat + 1) < bat)
                {
                    unsigned char buffer[5];
                    sprintf((char *)buffer, "%d", bat);
                    unsigned char send_buffer[30];
                    sprintf((char *)send_buffer, "txt_ele_level.txt=\"%s%%\"", buffer);
                    LCD_USART3_SendString(send_buffer);
                    LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
                    Last_bat = bat;
                }
            }
        }
        TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    }
}

uint8_t lcdColor[5]={4,2,1,3,3};
void LcdRefresh(void)
{
    static uint8_t state[29]={0};
    uint8_t txString[30]={0};
    uint8_t i=0;
    for(i=1;i<29;i++)
    {
        if(keyState[i]!=state[i])
        {
            state[i]=keyState[i];
            if(i<=6)
            {
                sprintf((char *)txString,"img_wired_%d.pic=%d",i,lcdColor[keyState[i]]);
                LCD_USART3_SendString(txString);
                LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
            }
            else if(i<=18)
            {
                sprintf((char *)txString,"img_nowire_%d.pic=%d",i-REGION1+1,lcdColor[keyState[i]]);
                LCD_USART3_SendString(txString);
                LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
            }
            else if(i<=24)
            {
                sprintf((char *)txString,"img_blast_%d.pic=%d",i-DEV1+1,lcdColor[keyState[i]]);
                LCD_USART3_SendString(txString);
                LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
            }else{
                if(i==25)
                {
                    if(keyState[FUNC_CHECK]==SELECTED)
                    {
                        sprintf((char *)txString,"txt_check.pic=6");
                    }else{
                        sprintf((char *)txString,"txt_check.pic=5");
                    }
                    LCD_USART3_SendString(txString);
                    LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
                }
                if(i==26)
                {
                    if(keyState[FUNC_SELECT_ALL]==SELECTED)
                    {
                        sprintf((char *)txString,"txt_select_all.pic=6");
                    }else{
                        sprintf((char *)txString,"txt_select_all.pic=5");
                    }
                    LCD_USART3_SendString(txString);
                    LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
                }
                if(i==27)
                {
                    if(keyState[FUNC_START]==SELECTED)
                    {
                        sprintf((char *)txString,"txt_start.pic=6");
                    }else{
                        sprintf((char *)txString,"txt_start.pic=5");
                    } 
                    LCD_USART3_SendString(txString);
                    LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
                }
                if(i==28)
                {
                    if(keyState[FUNC_FIRE]==SELECTED)
                    {
                        LCD_USART3_SendString((unsigned char *)"txt_ignition.pic=6");
                        LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                        LCD_USART3_SendString((unsigned char *)"vis btn_confirm,1");
                        LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                        LCD_USART3_SendString((unsigned char *)"vis btn_cancel,1");
                        LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
//                        LCD_USART3_SendString((unsigned char *)"txt_ignition.pic=5");
//                        LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    }else{
                        LCD_USART3_SendString((unsigned char *)"vis btn_confirm,0");
                        LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                        LCD_USART3_SendString((unsigned char *)"vis btn_cancel,0");
                        LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                        LCD_USART3_SendString((unsigned char *)"txt_start.pic=5");
                        LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
						LCD_USART3_SendString((unsigned char *)"txt_ignition.pic=5");
						LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    }
                }
            }
        }
    }
}

void LCD_cancelFire(void)
{
	LCD_USART3_SendString((unsigned char *)"vis btn_confirm,0");
	LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
	LCD_USART3_SendString((unsigned char *)"vis btn_cancel,0");
	LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
	LCD_USART3_SendString((unsigned char *)"txt_start.pic=5");
	LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
}
extern void bsp_PutKey(uint8_t _ulResult);

void LcdRecHandle(void)
{
    uint8_t temp1,temp2;
    if (LCD_USART3.Flag != 1)
    {
        return;
    }
    TIM4_Stop();
    
    if(LCD_USART3.Buff[0] == 0x86 && LCD_USART3.Buff[1] == 0xFF && LCD_USART3.Buff[2] == 0xFF)
    {
        LCD_Sleep_State = 1;
    }else if(LCD_Sleep_State == 1 && LCD_USART3.Flag ==1 && LCD_USART3.Count > 0) {
		// printf("退出休眠模式\r\n");
		LCD_Sleep_State = 0;
	}else{
        sscanf(LCD_USART3.Buff,"#%d#%d#",&temp1,&temp2);
        switch(temp1)
        {
            case 1:
                bsp_PutKey(FUNC_CHECK+temp2-1);
                break;
            case 2:
                bsp_PutKey(WIRE_L1+temp2-1);
                break;
            case 3:
                bsp_PutKey(REGION1+temp2-1);
                break;
            case 4:
                bsp_PutKey(DEV1+temp2-1);
                break;
            case 6:
                LCD_Log_Window_Status = 1;
                break;
            case 7:
//                keyState[FUNC_FIRE]=NO_CON;
                keyState[FUNC_START]=NO_CON;
                TIM5_Count = 0;
                TIM_Cmd(TIM5, DISABLE);
                if(temp2==1)
                {
                    flagFire=1;
                    LCD_USART3_SendString((unsigned char *)"vis btn_confirm,0");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    LCD_USART3_SendString((unsigned char *)"vis btn_cancel,0");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    LCD_USART3_SendString((unsigned char *)"txt_start.pic=5");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                }else if(temp2==2)
                {
                    
                    LCD_USART3_SendString((unsigned char *)"vis btn_confirm,0");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    LCD_USART3_SendString((unsigned char *)"vis btn_cancel,0");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    LCD_USART3_SendString((unsigned char *)"txt_start.pic=5");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                }else{
                    
                }
                break;
            case 8:
                if(temp2==1)
                {
                    LCD_USART3_SendString((unsigned char *)"slt_show_cont.txt=\"\"");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    LCD_USART3_SendString((unsigned char *)"click show_log,1");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    // SD卡读取记录的日志
                    My_SD_OpenLogFile();
                    My_SD_ReadLog();
                }else if(temp2==2)
                {
                    LCD_USART3_SendString((unsigned char *)"set_time_page.set_nian.val=rtc0");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    LCD_USART3_SendString((unsigned char *)"set_time_page.set_yue.val=rtc1");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    LCD_USART3_SendString((unsigned char *)"set_time_page.set_ri.val=rtc2");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    LCD_USART3_SendString((unsigned char *)"set_time_page.set_ri.val=rtc3");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    LCD_USART3_SendString((unsigned char *)"set_time_page.set_ri.val=rtc4");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    LCD_USART3_SendString((unsigned char *)"set_time_page.set_ri.val=rtc5");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                    LCD_USART3_SendString((unsigned char *)"page set_time_page");
                    LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
                }else{
                    
                }
                break;
            case 9:
                LCD_Log_Window_Status = 0;
                break;
            default:
                break;                   
        }
    }
    
    LCD_USART3_ClearStruct();
    TIM4_Start();
}