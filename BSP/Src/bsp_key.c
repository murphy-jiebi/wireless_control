/*
*********************************************************************************************************
*
*	模块名称 : IWDG 初始化
*	文件名称 : bsp_iwdg.c
*	版    本 : V1.0
*	说    明 : IWDG 初始化
*
*	修改记录 :
*		版本号  日期             作者              说明
*		V1.0    2022-04-08      
*
*********************************************************************************************************
*/
#include "bsp_key.h"
#include "adc.h"

/*
*********************************************************************************************************
*                                            变量定义
*********************************************************************************************************
*/
#define FILTER_CNT 3
#define ADC_ERROR  150

KEY_CODE_T keyQueue;
extern uint8_t flagSelfCheck;

const uint16_t KeyAdcValue[6] = {0, 534, 1149, 1755, 2368, 2886};


uint16_t adcUpperLimit[7]={100,900,1650,2350,2950,3650,4060};
uint16_t adcLowerLimit[7]={0,600,1400,2150,2750,3450,3900};

uint8_t  adcChannel[4]={ADC_Channel_12,ADC_Channel_13,ADC_Channel_11,ADC_Channel_10};


uint8_t keyState[29]={0};  //0 unuse
uint8_t keyDevState[12][6]={0};  //0 unuse

/*
*********************************************************************************************************
*        函 数 名: bsp_PutKey
*        功能说明: 
*        形    参: 
*        返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _ulResult)
{
    keyQueue.Buff[keyQueue.WritePtr++] = _ulResult;
    if(keyQueue.WritePtr >= 10)
    {
        keyQueue.WritePtr = 0;
    }
}
/*
*********************************************************************************************************
*        函 数 名: bsp_GetKey
*        功能说明: 
*        形    参: 
*        返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ucKeyCode = 0;
	
	if(keyQueue.ReadPtr != keyQueue.WritePtr)
	{
		ucKeyCode = keyQueue.Buff[keyQueue.ReadPtr++];
		
		if(keyQueue.ReadPtr >= 10)
		{
			keyQueue.ReadPtr = 0;
		}
	}
	
	return ucKeyCode;
}

/*
*********************************************************************************************************
*        函 数 名: bsp_ClearKey
*        功能说明: 
*        形    参: 
*        返 回 值: 无
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	keyQueue.ReadPtr = keyQueue.WritePtr;
}


/*
*********************************************************************************************************
*        函 数 名: KeyVarInit
*        功能说明: 
*        形    参: 
*        返 回 值: 无
*********************************************************************************************************
*/
void KeyVarInit(void)
{
	memset(keyQueue.Buff, 0x00, KEY_BUF_SIZE);
	keyQueue.ReadPtr = 0;
	keyQueue.WritePtr = 0;
}


void keyScan(void)
{
    static uint32_t filtCnt[4]={0};
    static uint8_t preKeyval[4]={0};
    uint8_t currKeyval[4]={0};
    uint16_t ADC_Value=0;
    uint8_t i=0;
    for(uint8_t m=0;m<4;m++)
    {
        ADC_Value = ADC_GetValue(adcChannel[m]);
        for (i = 0; i < 7; i++)
        {
            if ((ADC_Value < adcUpperLimit[i]) && (ADC_Value >= adcLowerLimit[i]))
            {
                currKeyval[m] = i + 1;
                break;
            }
        }

        if (currKeyval[m] != preKeyval[m])
        {
            preKeyval[m] = currKeyval[m];
            filtCnt[m] = 0;
        }
        else
        {
            if(currKeyval[m] != 0)
            {
                if (++filtCnt[m] > 20)
                {
                    if(currKeyval[m]==7)
                    {
                        bsp_PutKey(25+m);
                    }
                    else
                    {
                        bsp_PutKey(currKeyval[m]+7*m-m);// return putkey(currKeyval[m]+7*m);
                        
                    }
                    filtCnt[m]=0;
                }
            }else{
                filtCnt[m]=0;
            }
        }
    }
}

uint8_t preRegion=0;

void keyHandle(void)
{
    //get key
    uint8_t keyVal=0;
    uint8_t i=0,j=0;
    static uint8_t preKeyVal=0;
    

    keyVal=bsp_GetKey();
    if(keyVal==0)
    {
        return;
    }
    if(keyVal!=preKeyVal)
    {
        keyState[FUNC_SELECT_ALL]=NO_CON;
    }
    //启动后按键失效
     if((keyState[FUNC_START]==SELECTED))
    {
        if((keyVal!=FUNC_START)&&(keyVal!=FUNC_FIRE))
        {
            return;
        }
    }
    if((keyState[FUNC_CHECK]==SELECTED))
    {
        return;
    }
    //点火后按键失效
//    if((keyState[FUNC_FIRE]==SELECTED))
//    {
//        return;
//    }
    switch(keyVal)
    {
        case WIRE_L1:
        case WIRE_L2:
        case WIRE_M1:
        case WIRE_M2:
        case WIRE_R1:
        case WIRE_R2:
            if(keyState[keyVal]==SELECTED)
            {
                keyState[keyVal]=NO_SELECT;
            }else if(keyState[keyVal]==NO_SELECT)
            {
                keyState[keyVal]=SELECTED;
            }
            else{
//                keyState[keyVal]=NO_SELECT; 
            }
            break;
        case REGION1:
        case REGION2:
        case REGION3:
        case REGION4:
        case REGION5:
        case REGION6:
        case REGION7:
        case REGION8:
        case REGION9:
        case REGION10:
        case REGION11:
        case REGION12:
            if(keyState[keyVal]==NO_CON)
            {
                return;
            }
            if(preRegion!=0)
            {
                 for(i=0;i<6;i++)
                {
                    if(keyDevState[preRegion-REGION1][i]==SELECTED)
                    {
                        break;
                    }
                }
                if(i>=6)
                {
                    if(keyVal!=preKeyVal)
                    {
                        keyState[preRegion]=NO_SELECT;
                    }
                }
            }
            
            if(keyState[keyVal]==SELECTED)
            {
                if(keyVal!=preKeyVal)
                {
//                    for(i=1;i<7;i++)
//                    {
//                        keyState[DEV1+i-1]=keyDevState[keyVal-REGION1],6);[i-1]+1;
//                    }
                    memcpy(&keyState[DEV1],keyDevState[keyVal-REGION1],6);
                    preRegion=keyVal;
                }else{
                    keyState[keyVal]=NO_SELECT;
                    memset(&keyState[DEV1],NO_CON,6);
                    for(i=0;i<6;i++)
                    {
                        if(keyDevState[keyVal-REGION1][i]==SELECTED)
                        {
                            keyDevState[keyVal-REGION1][i]=NO_SELECT;
                        }
                    }
//                    memcpy(&keyState[DEV1],keyDevState[keyVal-REGION1],6);
                    preRegion=0;
                }
            }
            else if(keyState[keyVal]==NO_SELECT)
            {
                keyState[keyVal]=SELECTED;
                memcpy(&keyState[DEV1],keyDevState[keyVal-REGION1],6);
                preRegion=keyVal;
            }
            else
            {
                keyState[keyVal]=NO_SELECT;
            }
            break;
        case DEV1:
        case DEV2:
        case DEV3:
        case DEV4:
        case DEV5:
        case DEV6:
            if(preRegion!=0)
            {
                if(keyState[keyVal]==SELECTED)
                {
                    keyState[keyVal]=NO_SELECT;
                    keyDevState[preRegion-REGION1][keyVal-DEV1]=NO_SELECT;
                }else if(keyState[keyVal]==NO_SELECT){
                     keyState[keyVal]=SELECTED;
                    keyDevState[preRegion-REGION1][keyVal-DEV1]=SELECTED;
                }else{
                    
                }
            }
            break;
        case FUNC_CHECK:
            if(flagSelfCheck==0)
            {
                keyState[FUNC_CHECK]=SELECTED;
                flagSelfCheck=1;
            }
//            if(keyState[FUNC_CHECK]==SELECTED)
//            {
//                keyState[FUNC_CHECK]=NO_CON;
//                flagSelfCheck=0;
//            }else{
//                keyState[FUNC_CHECK]=SELECTED;
//                

//            }
            break;
        case FUNC_SELECT_ALL:
            if(preRegion==0)
            {            
                return;
            }
            if(keyState[FUNC_SELECT_ALL]==SELECTED)
            {
                keyState[FUNC_SELECT_ALL]=NO_CON;
                for(i=0;i<6;i++)
                {
                    if(keyDevState[preRegion-REGION1][i]==SELECTED)
                    {
                        keyDevState[preRegion-REGION1][i]=NO_SELECT;
                    }
                }  
            }else{
                keyState[FUNC_SELECT_ALL]=SELECTED;
                for(i=0;i<6;i++)
                {
                    if(keyDevState[preRegion-REGION1][i]==NO_SELECT)
                    {
                        keyDevState[preRegion-REGION1][i]=SELECTED;
                    }
                }
            }
            memcpy(&keyState[DEV1],keyDevState[preRegion-REGION1],6);
            break;
        case FUNC_START:
            if(keyState[FUNC_START]==SELECTED)
            {
                keyState[FUNC_START]=NO_CON;
            }else{
                keyState[FUNC_START]=SELECTED;
            }    
            break;
        case FUNC_FIRE:
            if(keyState[FUNC_START]==SELECTED)
            {
                if(keyState[FUNC_FIRE]==SELECTED)
                {
                    keyState[FUNC_FIRE]=NO_CON;
                    keyState[FUNC_START]=NO_CON;
                    TIM5_Count = 0;
                    TIM_Cmd(TIM5, DISABLE);
                    flagFire=1;
                }else{
                    keyState[FUNC_FIRE]=SELECTED;
                    TIM5_Count = 0;
                    TIM_Cmd(TIM5, ENABLE);
                }
            }
            break;
        default:
            break;
    }
    preKeyVal=keyVal;
}