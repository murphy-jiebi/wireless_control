#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"
#include "math.h"
#include "stdio.h"
#include "bsp_gpio.h"
#include "delay.h"
#include "tim.h"

typedef enum 
{
    WIRE_L1=1,
    WIRE_L2=2,
    WIRE_M1=3,
    WIRE_M2=4,
    WIRE_R1=5,
    WIRE_R2=6,
    REGION1,
    REGION2,
    REGION3,
    REGION4,
    REGION5,
    REGION6,
    REGION7,
    REGION8,
    REGION9,
    REGION10,
    REGION11,
    REGION12,
    DEV1,
    DEV2,
    DEV3,
    DEV4,
    DEV5,
    DEV6,
    FUNC_CHECK,
    FUNC_SELECT_ALL,
    FUNC_START,
    FUNC_FIRE,
}keyvalue_t;

typedef enum
{
    NO_CON,
    NO_SELECT,
    SELECTED,
    FIRED,
    FAULT,
}keyState_t;

extern uint8_t flagSelfCheck;
extern uint8_t flagFire;
extern uint8_t keyState[29];
extern uint8_t keyDevState[12][6];
extern uint16_t Cnt100ms;

#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

#define GPIOA_ODR_Addr (GPIOA_BASE + 12)
#define GPIOB_ODR_Addr (GPIOB_BASE + 12)
#define GPIOC_ODR_Addr (GPIOC_BASE + 12) 
#define GPIOD_ODR_Addr (GPIOD_BASE + 12)

#define GPIOA_IDR_Addr (GPIOA_BASE + 8)
#define GPIOB_IDR_Addr (GPIOB_BASE + 8)
#define GPIOC_IDR_Addr (GPIOC_BASE + 8)
#define GPIOD_IDR_Addr (GPIOD_BASE + 8)

#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr, n)
#define PAin(n)  BIT_ADDR(GPIOA_IDR_Addr, n)

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr, n)
#define PBin(n)  BIT_ADDR(GPIOB_IDR_Addr, n)

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr, n)
#define PCin(n)  BIT_ADDR(GPIOC_IDR_Addr, n)

#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr,n)
#define PDin(n)  BIT_ADDR(GPIOD_IDR_Addr,n)

#endif
