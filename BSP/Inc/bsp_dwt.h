/*
*********************************************************************************************************
*
*        模块名称 : TIMER滴答定时器初始化头文件
*        文件名称 : bsp_timer.h
*        版    本 : V1.0
*        说    明 : 
*
*        修改记录 :
*                版本号  日期             作者              说明
*                V1.0    2024-01-17     
*
*********************************************************************************************************
*/
#ifndef _BSP_DWT_H_
#define _BSP_DWT_H_

#include "bsp.h"

/*
*********************************************************************************************************
*                                            宏定义
*********************************************************************************************************
*/
#define  DWT_CYCCNT  *(volatile unsigned int *)0xE0001004
#define  DWT_CR      *(volatile unsigned int *)0xE0001000
#define  DEM_CR      *(volatile unsigned int *)0xE000EDFC
#define  DBGMCU_CR   *(volatile unsigned int *)0xE0042004
        


/*
*********************************************************************************************************
*                                            变量声明
*********************************************************************************************************
*/
void bsp_RunPer10ms(void);
void bsp_DelayUS(uint32_t _ulDelayUS);
void bsp_DelayMS(uint32_t _ulDelayMS);
void bsp_DwtInit(void);



#endif /* _BSP_DWT_H_ */

