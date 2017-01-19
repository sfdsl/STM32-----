#include "encord.h"

/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无

TIM3_CH1  PB4
TIM3_CH2  PB5  部门重映像

**************************************************************************/
void Encoder_Init_TIM3(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_ICInitTypeDef  TIM3_ICInitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		    // 复用浮空输入
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);	
	 GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);
   GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);//改变指定管脚的映射
	
	/* Time base configuration */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	
	
	TIM_TimeBaseStructure.TIM_Period = 65534;       //当定时器从0计数到999，即为1000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为36MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	/*本段代码由艾思科技-STM32库函数代码生成器自动生成
	正交编码器
	*/
  
	//TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge );//设置TIMx编码界面
	//TIM_ICStructInit(&TIM3_ICInitStructure);
  
	//TIM3_ICInitStructure.TIM_ICFilter =6;//选择输入比较滤波器
	//TIM_ICInit(TIM3, &TIM3_ICInitStructure);	//根据TIM_ICInitStruct中指定的参数初始化外设TIMx 
	
	//TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM1的更新标志位
 // TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	//TIM3->CNT = 0;
	TIM3->CCMR1 |= 1<<0;          //输入模式，IC1FP1映射到TI1上
  TIM3->CCMR1 |= 1<<8;          //输入模式，IC2FP2映射到TI2上
  TIM3->CCER |= 0<<1;           //IC1不反向
  TIM3->CCER |= 0<<5;           //IC2不反向
	TIM3->SMCR |= 3<<0;	          //SMS='011' 所有的输入均在上升沿和下降沿有效
	TIM3->CR1 |= 0x01;            //CEN=1，使能定时器
//  TIM_Cmd(TIM3, ENABLE);
}


/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
TIM2_CH1 
TIM2_CH2  完全重映像  
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		    // 复用浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);//改变指定管脚的映射
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	/* Time base configuration */		 
	TIM_TimeBaseStructure.TIM_Period = 65534;       //当定时器从0计数到999，即为1000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为36MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//设置时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	

//	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge );//设置TIMx编码界面
	//TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM1的更新标志位
 
// TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
//	TIM2->CNT = 0;
	TIM2->CCMR1 |= 1<<0;          //输入模式，IC1FP1映射到TI1上
  TIM2->CCMR1 |= 1<<8;          //输入模式，IC2FP2映射到TI2上
  TIM2->CCER |= 0<<1;           //IC1不反向
  TIM2->CCER |= 0<<5;           //IC2不反向
	TIM2->SMCR |= 3<<0;	          //SMS='011' 所有的输入均在上升沿和下降沿有效
	TIM2->CR1 |= 0x01;            //CEN=1，使能定时器
  //TIM_Cmd(TIM2, ENABLE);
	
}


/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Read_Encoder(u8 TIMX)
{
    int Encoder_TIM;    
   switch(TIMX)
	 {
	   case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;
		 case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	

		 default:  Encoder_TIM=0;
	 }
		return Encoder_TIM;
}
