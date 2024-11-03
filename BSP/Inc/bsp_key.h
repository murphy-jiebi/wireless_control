/*
*********************************************************************************************************
*
*	模块名称 : bsp 底层驱动头文件
*	文件名称 : bsp.h
*	版    本 : V1.0
*	说    明 : 
*
*	修改记录 :
*		版本号  日期             作者              说明
*		V1.0    2022-04-08       
*
*********************************************************************************************************
*/
#ifndef _BSP_KEY_H_
#define _BSP_KEY_H_
#include "main.h"
/*
*********************************************************************************************************
*                                            宏定义
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
