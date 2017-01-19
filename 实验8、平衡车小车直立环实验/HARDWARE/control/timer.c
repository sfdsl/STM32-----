#include "timer.h"

u32 TIM1_IRQCNT = 0;
void TIM1_INIT(u16 period_num)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//                                                                     
	TIM_DeInit(TIM1);
	TIM_TimeBaseStructure.TIM_Period=period_num;//装载值
	
	//prescaler is 1200,that is 72000000/72/500=2000Hz;
	TIM_TimeBaseStructure.TIM_Prescaler=72-1;//分频系数
	//set clock division 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //or TIM_CKD_DIV2 or TIM_CKD_DIV4
	//count up
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	//clear the TIM2 overflow interrupt flag
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	//TIM2 overflow interrupt enable
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	//enable TIM2
	TIM_Cmd(TIM1,ENABLE);
}
