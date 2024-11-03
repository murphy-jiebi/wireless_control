/*
*********************************************************************************************************
*
*	ģ������ : bsp �ײ�����ͷ�ļ�
*	�ļ����� : bsp.h
*	��    �� : V1.0
*	˵    �� : 
*
*	�޸ļ�¼ :
*		�汾��  ����             ����              ˵��
*		V1.0    2022-04-08       
*
*********************************************************************************************************
*/
#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_
#include "main.h"
/*
*********************************************************************************************************
*                                            �궨��
*********************************************************************************************************
*/
#define KEY_BUF_SIZE 10





typedef struct
{
	uint8_t Buff[KEY_BUF_SIZE];
	uint8_t WritePtr;
	uint8_t ReadPtr;
}KEY_CODE_T;

uint8_t bsp_GetKey(void);
void bsp_ClearKey(void);
void bsp_KeyScan(void);
void bsp_KeyInit(void);
void keyScan(void);
void KeyVarInit(void);
void keyHandle(void);

#endif /* _BSP_KEY_H_ */
