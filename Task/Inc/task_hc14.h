#ifndef _TASK_HC14_H
#define _TASK_HC14_H

#include "main.h"


uint8_t Hc14SendCmd(uint8_t cmd,uint8_t dev,uint8_t *status);

void Hc14RecProcess(uint8_t *buf,uint8_t len);


#endif


