
#include "ws2812b.h"

#include "tim.h"
#include "bat.h"
#include "LCD_USART3.h"
#include "usart1.h"
#include "string.h"
#include "LCD.h"
#include "bsp_ws281x.h"
//#include "wired_port.h"
#include "main.h"
#include "my_sd.h"
#include "adc.h"
#include "key.h"
#include "tim.h"
#include "usart2.h"
#include "bsp_key.h"

#include "task_hc14.h"
#include "task_ctrl.h"

uint16_t Cnt100ms=0;
uint8_t flagSelfCheck=0;
uint8_t flagFire=0;

uint16_t a=0;


int main()
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	Delay_Init();
	USART1_Init(115200);

	USART2_Init(9600);
	LCD_USART3_Init(115200);

    bsp_GpioInit();
	LCD_Rest();
    
//    RESET_HC_MODE;
//    usart2_senddata("AT",2);
//    Delay_Ms(100);
//    usart2_senddata("AT+DEFAULT",10);
//    Delay_Ms(100);
	WS2812X_Init();
//	WS2812X_tset();

	unsigned char i = 0;
	while (strstr((const char *)LCD_USART3.Buff, "hello") == NULL && i < 40)
	{
		Delay_Ms(100);
		i++;
	}
	My_SD_Init();

	unsigned char *Log_Time = LCD_Get_RTC_Time();
	if(strstr((const char *)Log_Time, "年") != NULL && strstr((const char *)Log_Time, "月") != NULL) 
   {
		My_SD_OpenLogFile();
		unsigned char Log[80];
		sprintf((char *)Log, "%s启动并自检完成\r\n", Log_Time);
		LCD_USART3_ClearStruct();
		My_SD_WriteLog((unsigned char*)Log);
		LCD_USART3_ClearStruct();
    }

	ADC_Configura_Init();
    KeyVarInit();
	TIM4_Init();
	TIM3_Init();
	TIM5_Init();
    TIM_Cmd(TIM3, ENABLE);
    memset(&keyState[WIRE_L1],NO_CON,6+12+6);

	while (1)
	{
        keyScan();
        LcdRecHandle();
        keyHandle();
        LedRefresh();
		BatLevelRefresh();
        LcdRefresh();
        FireProcess();
        SelfCheck();
        if(usart2_rx.rxflag)
        {
            usart2_rx.rxflag=0;
            Hc14RecProcess(usart2_rx.rxbuf,usart2_rx.rxlen);
            usart2_rx.rxlen=0;
        }
	}
}
