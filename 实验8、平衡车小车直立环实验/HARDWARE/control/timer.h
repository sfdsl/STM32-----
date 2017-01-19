#ifndef _TIMEER_H_
#define _TIMER_H_

#include "sys.h"

extern u32 TIM1_IRQCNT;

void TIM1_INIT(u16 period_num);//用于精确延时
#endif


