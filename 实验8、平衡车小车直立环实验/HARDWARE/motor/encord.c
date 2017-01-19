#include "encord.h"

/**************************************************************************
�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����

TIM3_CH1  PB4
TIM3_CH2  PB5  ������ӳ��

**************************************************************************/
void Encoder_Init_TIM3(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//TIM_ICInitTypeDef  TIM3_ICInitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		    // ���ø�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);	
	 GPIO_ResetBits(GPIOB,GPIO_Pin_4|GPIO_Pin_5);
   GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);//�ı�ָ���ܽŵ�ӳ��
	
	/* Time base configuration */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	
	
	TIM_TimeBaseStructure.TIM_Period = 65534;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ36MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//����ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	/*���δ����ɰ�˼�Ƽ�-STM32�⺯�������������Զ�����
	����������
	*/
  
	//TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge );//����TIMx�������
	//TIM_ICStructInit(&TIM3_ICInitStructure);
  
	//TIM3_ICInitStructure.TIM_ICFilter =6;//ѡ������Ƚ��˲���
	//TIM_ICInit(TIM3, &TIM3_ICInitStructure);	//����TIM_ICInitStruct��ָ���Ĳ�����ʼ������TIMx 
	
	//TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIM1�ĸ��±�־λ
 // TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	//TIM3->CNT = 0;
	TIM3->CCMR1 |= 1<<0;          //����ģʽ��IC1FP1ӳ�䵽TI1��
  TIM3->CCMR1 |= 1<<8;          //����ģʽ��IC2FP2ӳ�䵽TI2��
  TIM3->CCER |= 0<<1;           //IC1������
  TIM3->CCER |= 0<<5;           //IC2������
	TIM3->SMCR |= 3<<0;	          //SMS='011' ���е�������������غ��½�����Ч
	TIM3->CR1 |= 0x01;            //CEN=1��ʹ�ܶ�ʱ��
//  TIM_Cmd(TIM3, ENABLE);
}


/**************************************************************************
�������ܣ���TIM2��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
TIM2_CH1 
TIM2_CH2  ��ȫ��ӳ��  
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		    // ���ø�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);//�ı�ָ���ܽŵ�ӳ��
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	/* Time base configuration */		 
	TIM_TimeBaseStructure.TIM_Period = 65534;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ36MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//����ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	

//	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge );//����TIMx�������
	//TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM1�ĸ��±�־λ
 
// TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
//	TIM2->CNT = 0;
	TIM2->CCMR1 |= 1<<0;          //����ģʽ��IC1FP1ӳ�䵽TI1��
  TIM2->CCMR1 |= 1<<8;          //����ģʽ��IC2FP2ӳ�䵽TI2��
  TIM2->CCER |= 0<<1;           //IC1������
  TIM2->CCER |= 0<<5;           //IC2������
	TIM2->SMCR |= 3<<0;	          //SMS='011' ���е�������������غ��½�����Ч
	TIM2->CR1 |= 0x01;            //CEN=1��ʹ�ܶ�ʱ��
  //TIM_Cmd(TIM2, ENABLE);
	
}


/**************************************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
����  ֵ���ٶ�ֵ
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
