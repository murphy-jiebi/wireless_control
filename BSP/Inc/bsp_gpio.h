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
#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_
#include "stm32f10x.h"



#define LEFT_IGN1   PAout(15)
#define LEFT_IGN2 PCout(11)
#define MIDDLE_IGN1 PDout(2)
#define MIDDLE_IGN2 PBout(4)
#define RIGHT_IGN1 PBout(6)
#define RIGHT_IGN2 PBout(8)

#define  SET_WIRE_L1        GPIO_SetBits(GPIOA,GPIO_Pin_15)
#define  RESET_WIRE_L1      GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define  GET_WIRE_L1        GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)

#define  SET_WIRE_L2        GPIO_SetBits(GPIOC,GPIO_Pin_11)
#define  RESET_WIRE_L2      GPIO_ResetBits(GPIOC,GPIO_Pin_11)
#define  GET_WIRE_L2        GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)

#define  SET_WIRE_M1        GPIO_SetBits(GPIOD,GPIO_Pin_2)
#define  RESET_WIRE_M1      GPIO_ResetBits(GPIOD,GPIO_Pin_2)
#define  GET_WIRE_M1        GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)

#define  SET_WIRE_M2        GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define  RESET_WIRE_M2      GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define  GET_WIRE_M2        GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)

#define  SET_WIRE_R1        GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define  RESET_WIRE_R1      GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define  GET_WIRE_R1        GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)

#define  SET_WIRE_R2        GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define  RESET_WIRE_R2      GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define  GET_WIRE_R2        GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

#define  SET_HC_MODE         GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define  RESET_HC_MODE       GPIO_ResetBits(GPIOC, GPIO_Pin_4)

#define  GET_HC_STATUS       GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
/*
#define LEFT_INSP1 PCin(10)
#define LEFT_INSP2 PCin(12)
#define MIDDLE_INSP3 PBin(3)
#define MIDDLE_INSP4 PBin(5)
#define RIGHT_INSP5 PBin(7)
#define RIGHT_INSP6 PBin(9)
*/

void bsp_GpioInit(void);

#endif /* _BSP_KEY_H_ */
