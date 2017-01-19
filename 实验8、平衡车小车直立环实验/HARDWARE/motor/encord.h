#ifndef __ENCODER_H
#define __ENCODER_H

#include "sys.h"

void Encoder_Init_TIM3(void);
void Encoder_Init_TIM2(void);
int Read_Encoder(u8 TIMX);
#endif
