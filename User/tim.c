#include "tim.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "LCD.h"
#include "key.h"
#include "bsp_ws281x.h"
#include "LCD_USART3.h"
#include "bsp_gpio.h"

//100ms
void TIM5_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    TIM_TimeBaseInitTypeDef TIM5_TimeBaseInitStruct = {0};
    TIM5_TimeBaseInitStruct.TIM_Period = 999;
    TIM5_TimeBaseInitStruct.TIM_Prescaler = 7199;
    TIM5_TimeBaseInitStruct.TIM_ClockDivision = 0;
    TIM5_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM5_TimeBaseInitStruct);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM5, DISABLE);
}
 
unsigned char Public_Key_State = 0;
unsigned int TIM5_Count = 0;

void TIM5_IRQHandler(void) {
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 
    {
        
		TIM5_Count++;
		printf("TIM5_Count:%d\r\n",TIM5_Count);
		if(TIM5_Count >= 50) {
//			Public_Key_State = 0;
			TIM5_Count = 0;
			TIM_Cmd(TIM5, DISABLE);
//			Key_Select_State[20] = 0;
//			WS2812X_SedColor(0, 0, 0, 21);
//			LCD_Ignition_Window_Status = 0;
//			LCD_Select_Control_Status[2] = 0;
//			LCD_Select_Control_Status[3] = 0;
//			printf("Stop TIM5\r\n");
			LCD_USART3_SendString((unsigned char *)"vis btn_confirm,0");
			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
			LCD_USART3_SendString((unsigned char *)"vis btn_cancel,0");
			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
			LCD_USART3_SendString((unsigned char *)"txt_start.pic=5");
			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
            keyState[FUNC_START]=NO_CON;
            keyState[FUNC_FIRE]=NO_CON;
		}
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    }
}



void TIM4_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_InternalClockConfig(TIM4);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    // 1000ms
    TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1; 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
    TIM_Cmd(TIM4, ENABLE);
}

void TIM4_Stop(void) {
    TIM_Cmd(TIM4, DISABLE);
    TIM_SetCounter(TIM4, 0);
}

void TIM4_Start(void) {
    TIM_SetCounter(TIM4, 0);
    TIM_Cmd(TIM4, ENABLE);
}


void TIM3_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
    TIM_TimeBaseInitStruct.TIM_Period = 999;  // 50ms
    TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, DISABLE);
}


unsigned char Wired_test_count = 0;
unsigned char sd_log = 0;

void TIM3_task(void) 
{
    Cnt100ms++;
//    if(Wired_test_count == 0) {
//        TIM4_Stop();
//        // printf("开启点火任务\r\n");
//        if(LCD_Wired_Receiver_Select_Status[0] == 1 && Key_Select_State[0] == 1) {
//            // printf("引爆第1个点火头\r\n");
//            WS2812X_SedColor(255, 0, 0, 1);
//            LCD_USART3_SendString((unsigned char*)"img_wired_1.pic=3");
//			LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
//            SET_WIRE_L1;
//            Wired_test_count = 1;
//        }else{
//            Wired_test_count = 1;
//        }
//    }else if(Wired_test_count == 1) {
//        RESET_WIRE_L1;
//        if(LCD_Wired_Receiver_Select_Status[1] == 1 && Key_Select_State[1] == 1) {
//             // printf("引爆第2个点火头\r\n");
//            WS2812X_SedColor(255, 0, 0, 2);
//            LCD_USART3_SendString((unsigned char*)"img_wired_2.pic=3");
//			LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
//            SET_WIRE_L2;
//            Wired_test_count = 2;
//        }else{
//            Wired_test_count = 2;
//        }
//    }else if(Wired_test_count == 2) {
//        RESET_WIRE_L1;
//        RESET_WIRE_L2;
//        if(LCD_Wired_Receiver_Select_Status[2] == 1 && Key_Select_State[2] == 1) {
//            // printf("引爆第3个点火头\r\n");
//            WS2812X_SedColor(255, 0, 0, 3);
//            LCD_USART3_SendString((unsigned char*)"img_wired_3.pic=3");
//			LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
//            SET_WIRE_M1;
//            Wired_test_count = 3;
//        }else{
//            Wired_test_count = 3;
//        }
//    }else if(Wired_test_count == 3) {
//        RESET_WIRE_L1;
//        RESET_WIRE_L2;
//        RESET_WIRE_M1;
//        if(LCD_Wired_Receiver_Select_Status[3] == 1 && Key_Select_State[3] == 1) {
//            // printf("引爆第4个点火头\r\n");
//            WS2812X_SedColor(255, 0, 0, 4);
//            LCD_USART3_SendString((unsigned char*)"img_wired_4.pic=3");
//			LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
//            SET_WIRE_M2;
//            Wired_test_count = 4;
//        }else{
//            Wired_test_count = 4;
//        }
//    }else if(Wired_test_count == 4) {
//        RESET_WIRE_L1;
//        RESET_WIRE_L2;
//        RESET_WIRE_M1;
//        RESET_WIRE_M2;
//        if(LCD_Wired_Receiver_Select_Status[4] == 1 && Key_Select_State[4] == 1) {
//            // printf("引爆第5个点火头\r\n");
//            LCD_USART3_SendString((unsigned char*)"img_wired_5.pic=3");
//			LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
//            WS2812X_SedColor(255, 0, 0, 5);
//            SET_WIRE_R1;
//            Wired_test_count = 5;
//        }else{
//            Wired_test_count = 5;
//        }
//    }else if(Wired_test_count == 5) {
//        RESET_WIRE_L1;
//        RESET_WIRE_L2;
//        RESET_WIRE_M1;
//        RESET_WIRE_M2;
//        RESET_WIRE_R1;
//        if(LCD_Wired_Receiver_Select_Status[5] == 1 && Key_Select_State[5] == 1) {
//            // printf("引爆第6个点火头\r\n");
//            WS2812X_SedColor(255, 0, 0, 6);
//            LCD_USART3_SendString((unsigned char*)"img_wired_6.pic=3");
//			LCD_USART3_SendThreeBytes(0xff, 0xff, 0xff);
//            SET_WIRE_R2;
//            Wired_test_count = 6;
//        }else{
//            Wired_test_count = 6;
//        }
//    }else if(Wired_test_count == 6) {
//        RESET_WIRE_L1;
//        RESET_WIRE_L2;
//        RESET_WIRE_M1;
//        RESET_WIRE_M2;
//        RESET_WIRE_R1;
//        RESET_WIRE_R2;
//        Wired_test_count = 0;
//        TIM_Cmd(TIM3, DISABLE); // 关闭定时器3
//        TIM4_Start();
//        // printf("点火任务执行完毕\r\n");
//        sd_log = 1;
//    }
}
   


void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        TIM3_task();
    }
}
