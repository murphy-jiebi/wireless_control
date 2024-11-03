#ifndef __TIM_H
#define __TIM_H

extern unsigned char TIM4_Flag;
extern unsigned char Wired_test_count;

extern unsigned char Public_Key_State;
extern unsigned int TIM5_Count;

void TIM5_Init(void);

void TIM4_Init(void);
void TIM4_Stop(void);
void TIM4_Start(void);

void TIM3_Init(void);
void TIM3_task(void);

extern unsigned char sd_log;

#endif
