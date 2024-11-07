#include "task_hc14.h"
#include "usart2.h"
#include "bsp_key.h"

const uint8_t groupSn=1;

extern uint8_t flag_checkORfire[29];

uint8_t Get_CrcXOR(uint8_t *_ucpBuf, uint16_t _usLen)
{
	uint16_t i;
	uint8_t  Temp_Crc = 0x00;	
	for(i = 0; i < _usLen; i++)
	{
		Temp_Crc ^= *_ucpBuf++;
	}
	return Temp_Crc;
}

uint8_t Hc14SendCmd(uint8_t cmd,uint8_t dev,uint8_t *status)
{
    uint8_t sendbuf[10]={0};
    uint8_t i=0;
    uint8_t ch=0;
    for(i=0;i<6;i++)
    {
        if(status[i]==SELECTED)
        {
            ch|=(1<<i);
        }
    }
    sendbuf[0]=0xFE;
    if(cmd==0x01)
    {
        sendbuf[1]=0x04;
    }else{
        sendbuf[1]=0x05;
        sendbuf[4]=ch&0x3F;
    }
    sendbuf[2]=cmd;
    sendbuf[3]=(groupSn<<4)|(dev&0x0f);
    sendbuf[sendbuf[1]]=Get_CrcXOR(sendbuf,sendbuf[1]);
  
    return usart2_senddata(sendbuf,sendbuf[1]+1);
}

extern uint8_t preRegion;
void Hc14RecProcess(uint8_t *buf,uint8_t len)
{
    uint8_t *ptr=NULL;
    uint8_t i=0;
    uint8_t j=0;
    uint8_t all=0;
    uint8_t temp_group=0;
    uint8_t temp_dev=0;
    uint8_t temp_ch=0;
	uint8_t temp_ch1=0;
    for(i=0;i<len;i++)
    {
        if(buf[i]==0xFE)
        {
            break;
        }
    }
    if((len-i)<4)
    {
        return;
    }
    ptr=&buf[i];
    
//    if(ptr[1]!=(len-i-1))
//    {
//        return;
//    }
    if(Get_CrcXOR(ptr,*(ptr+1))!= *(ptr + *(ptr + 1)))
    {
        return;
    }
    temp_group=(buf[3]>>4)&0x0f;
    if(temp_group!=groupSn)
    {
        return;
    }
    temp_dev=(buf[3])&0x0f;
    temp_ch=buf[4];
	temp_ch1=buf[5];
    switch(buf[2])
    {
        case 0x01:
            
            break;
        case 0x02:
            
            break;
        case 0xF1:
            flag_checkORfire[temp_dev+REGION1-1]=2;
            keyState[temp_dev+REGION1-1]=NO_SELECT;
            for(j=0;j<6;j++)
            {
                if(temp_ch&0x01)
                {
					if(temp_ch1&0x01)
					{
						keyDevState[temp_dev-1][j]=FIRED;
					}else{
						keyDevState[temp_dev-1][j]=NO_SELECT;
					}
                }else{
                    keyDevState[temp_dev-1][j]=NO_CON;
                }
                temp_ch>>=1;
				temp_ch1>>=1;
            }
            break;
        case 0xF2:
            flag_checkORfire[temp_dev+REGION1-1]=4;
            
            for(i=0;i<6;i++)
            {    
				if(temp_ch&0x01)
                {
					keyDevState[temp_dev-1][i]=FIRED;
				}
				
				temp_ch>>=1;
//                if(keyDevState[temp_dev-1][i]==SELECTED)
//                {
//                    keyDevState[temp_dev-1][i]=FIRED;
//                }
                if(keyDevState[temp_dev-1][i]==NO_SELECT)
                {
                    all=1;
                }
            }
            if(all==0)
            {
                keyState[temp_dev+REGION1-1]=FIRED;
            }
            if(preRegion==(temp_dev+REGION1-1))
            {
                memcpy(&keyState[DEV1],keyDevState[temp_dev-1],6);
            }
            break;
        default:
            break;
    }
}


