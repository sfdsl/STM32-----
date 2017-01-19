#include "motor_pwm.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  


//��ʼ�������������IO
void Motor_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    // �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3 ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3);
}
//TIM8��ͨ��1��2���ó�PWM����˿�
static void TIM8_Mode_Config(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* Time base configuration */		 
	TIM_TimeBaseStructure.TIM_Period = arr;       
	TIM_TimeBaseStructure.TIM_Prescaler = psc;	    
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//����ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = 0;	   //��������ֵ�������������������ֵʱ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ


	TIM_OC1Init(TIM8, &TIM_OCInitStructure);	//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx 
	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OC2Init(TIM8, &TIM_OCInitStructure); 	

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);//TIM8��CCR1�ϵ�Ԥװ�ؼĴ���ʹ��
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);//TIM8��CCR2�ϵ�Ԥװ�ؼĴ���ʹ��

	
	TIM_ARRPreloadConfig(TIM8, ENABLE);	  // ʹ��TIM8���ؼĴ���ARR
	TIM8->BDTR|=0X8000;
	TIM_Cmd(TIM8, ENABLE);
}


                                                                                                                                                             void MotorInit(u16 arr,u16 psc)
{
	Motor_GPIO_Config();
	TIM8_Mode_Config(arr,psc);	
}
void SetMotorPWM(u16 PWM1,u16 PWM2)
{
 	TIM_SetCompare1(TIM8,PWM1);	 
	TIM_SetCompare2(TIM8,PWM2);
}
