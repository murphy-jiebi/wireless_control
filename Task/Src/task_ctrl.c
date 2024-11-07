#include "task_ctrl.h"
#include "task_hc14.h"
#include "LCD.h"
#include "my_sd.h"
#include "LCD_USART3.h"

uint8_t flag_checkORfire[29]={0};
void SelfCheck(void)
{
    static uint8_t step=1;
    static uint16_t CntPreVal=0;
    static uint8_t firstIn=0;
    static uint8_t flag1=0;
	
	
    if(!flagSelfCheck)
    {
        return;
    }
    switch(step)
    {
        case 0:
            step=1;
            break;
        case 1:
//            if(GET_WIRE_L1)
//            {
//                keyState[WIRE_L1]=NO_SELECT;
//            }else{
//                keyState[WIRE_L1]=NO_CON;
//            }
//            if(GET_WIRE_L2)
//            {
//                keyState[WIRE_L2]=NO_SELECT;
//            }else{
//                keyState[WIRE_L2]=NO_CON;
//            }
//            if(GET_WIRE_M1)
//            {
//                keyState[WIRE_M1]=NO_SELECT;
//            }else{
//                keyState[WIRE_M1]=NO_CON;
//            }
//            if(GET_WIRE_M2)
//            {
//                keyState[WIRE_M2]=NO_SELECT;
//            }else{
//                keyState[WIRE_M2]=NO_CON;
//            }
//            if(GET_WIRE_R1)
//            {
//                keyState[WIRE_R1]=NO_SELECT;
//            }else{
//                keyState[WIRE_R1]=NO_CON;
//            }
//            if(GET_WIRE_R2)
//            {
//                keyState[WIRE_R2]=NO_SELECT;
//            }else{
//                keyState[WIRE_R2]=NO_CON;
//            }
           memset(&keyState[WIRE_L1],NO_SELECT,6);
            step=7;
        break;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:  
            if(!firstIn)
            {
                flag_checkORfire[step]=1;
                firstIn=1;
                CntPreVal=Cnt100ms;
                if(!(Hc14SendCmd(0x01,step-REGION1+1,keyDevState[step-REGION1])))//后续修改，暂时占个坑
                {
                    Delay_Ms(200);
                    Hc14SendCmd(0x01,step-REGION1+1,keyDevState[step-REGION1]);
                }  
            }else{
                if(Cnt100ms-CntPreVal>50)
                {
                    firstIn=0;
                    step++;
                }
            }
  
////            if(keyState[step]!=NO_SELECT)
////            {
//                if(!firstIn)
//                {
//                    CntPreVal=Cnt100ms;
//                    firstIn=1;
//                    if(!(Hc14SendCmd(0x01,step-REGION1+1,keyDevState[step-REGION1])))//后续修改，暂时占个坑
//                    {
//                        Delay_Ms(200);
//                        Hc14SendCmd(0x01,step-REGION1+1,keyDevState[step-REGION1]);
//                    }   
//                }else{
//                    if((Cnt100ms-CntPreVal>50)&&(flag1==0))
//                    {
//                        if(!(Hc14SendCmd(0x01,step-REGION1+1,keyDevState[step-REGION1])))//后续修改，暂时占个坑
//                        {
//                            Delay_Ms(200);
//                            Hc14SendCmd(0x01,step-REGION1+1,keyDevState[step-REGION1]);
//                        }
//                        flag1=1;
//                    }
//                    if((Cnt100ms-CntPreVal>100)&&(flag1==1))
//                    {
//                        firstIn=0;
//                        step++;
//                        flag1=0;
//                    }
//                }
//                
////            }else{
////                step++;
////                firstIn=0;

////            }
            break;
        case 19:
            flagSelfCheck=0;
            step=1;
            firstIn=0;
			
			
            for(uint8_t i=7;i<19;i++)
            {
                if(flag_checkORfire[i]==1)
                {
                    keyState[i]=NO_CON;
                }
            }
            memset(flag_checkORfire,0,29);
            keyState[FUNC_CHECK]=NO_CON;
            LCD_USART3_SendString((unsigned char *)"txt_check.pic=6");
            LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
            LCD_USART3_SendString((unsigned char *)"txt_check.pic=5");
            LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);

            unsigned char* Log_Time = LCD_Get_RTC_Time();
            if(strstr((const char *)Log_Time, "年") != NULL && strstr((const char *)Log_Time, "月") != NULL) 
            {
                My_SD_OpenLogFile();
                unsigned char Log[80];
                sprintf((char *)Log, "%s检测正常\r\n", Log_Time);
                LCD_USART3_ClearStruct();
                My_SD_WriteLog((unsigned char*)Log);
                LCD_USART3_ClearStruct();
            }
            LCD_USART3_SendString((unsigned char *)"slt_show_cont.txt=\"\"");
            LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
            LCD_USART3_SendString((unsigned char *)"click show_log,1");
            LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
            // SD卡读取记录的日志
            My_SD_OpenLogFile();
            My_SD_ReadLog();
            break;
        default:
            break;
        
    }
    
    
//    memset(&keyState[REGION1],NO_SELECT,12+6);
    
}

void FireProcess(void)
{
    static uint8_t step=1;
    static uint16_t CntPreVal=0;
    static uint8_t firstIn=0;
    static uint8_t flag1=0;
    if(!flagFire)
    {
        return;
    }
    switch(step)
    {
        case 0:
            step=1;
            break;
        case 1:
            if(keyState[1]==SELECTED)
            {
                SET_WIRE_L1;
                keyState[1]=FIRED;
            }
            if(keyState[2]==SELECTED)
            {
                SET_WIRE_L2;
                keyState[2]=FIRED;
            }
            if(keyState[3]==SELECTED)
            {
                SET_WIRE_M1;
                keyState[3]=FIRED;
            }
            if(keyState[4]==SELECTED)
            {
                SET_WIRE_M2;
                keyState[4]=FIRED;
            }
            if(keyState[5]==SELECTED)
            {
                SET_WIRE_R1;
                keyState[5]=FIRED;
            }
            if(keyState[6]==SELECTED)
            {
                SET_WIRE_R2;
                keyState[6]=FIRED;
            }
            step=7;
            break;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
            if(keyState[step]==SELECTED)
            {
                if(!firstIn)
                {
                    flag_checkORfire[step]=3;
                    CntPreVal=Cnt100ms;
                    firstIn=1;
                    if(!(Hc14SendCmd(0x02,step-REGION1+1,keyDevState[step-REGION1])))//后续修改，暂时占个坑
                    {
                        Delay_Ms(200);
                        Hc14SendCmd(0x02,step-REGION1+1,keyDevState[step-REGION1]);
                    }   
                }else{
                    if((Cnt100ms-CntPreVal>50)&&(flag1==0))
                    {
                        if(!(Hc14SendCmd(0x02,step-REGION1+1,keyDevState[step-REGION1])))//后续修改，暂时占个坑
                        {
                            Delay_Ms(200);
                            Hc14SendCmd(0x02,step-REGION1+1,keyDevState[step-REGION1]);
                        }
                        flag1=1;
                    }
                    if((Cnt100ms-CntPreVal>100)&&(flag1==1))
                    {
                        firstIn=0;
                        step++;
                        flag1=0;
                    }
                }
                
            }else{
                step++;
                firstIn=0;

            }
            break;
        case 19:
            flagFire=0;
            step=1;
            firstIn=0;
            sd_log = 1;
            RESET_WIRE_L1;
            RESET_WIRE_L2;
            RESET_WIRE_M1;
            RESET_WIRE_M2;
            RESET_WIRE_R1;
            RESET_WIRE_R2;
//            memset(&keyState[DEV1],NO_SELECT,6);
        
			keyState[FUNC_FIRE]=NO_CON;
            for(uint8_t i=7;i<19;i++)
            {
                if(flag_checkORfire[i]==3)
                {
                    keyState[i]=NO_CON;
                }
            }
            memset(flag_checkORfire,0,29);
            
            sd_log = 0;
			LCD_USART3_ClearStruct();
			LCD_USART3_SendString((unsigned char*)"click get_time,1");
			LCD_USART3_SendThreeBytes(0xff, 0xff ,0xff);
	
			unsigned char j = 0;
			while(strstr((const char *)LCD_USART3.Buff, "年")== NULL && j < 25) {
				Delay_Ms(50);
				j++;
			}

			if(strstr((const char *)LCD_USART3.Buff, "年") != NULL && strstr((const char *)LCD_USART3.Buff, "月") != NULL) {
				My_SD_OpenLogFile();
				unsigned char log[50];
				sprintf((char *)log, (const char*)"%s执行点火命令完毕\r\n", LCD_USART3.Buff);
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
            
            break;
        default:
            break;
    }
}

