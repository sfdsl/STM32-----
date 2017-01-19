#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "encord.h"
//#include "usmart.h"
#include "key.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "motor_pwm.h"
#include "timer.h"

int Encoder_Left,Encoder_Right;             //���ұ��������������
int Moto1,Moto2;                            //���PWM���� Ӧ��Motor�� ��Moto�¾�	
float Angle_Balance,Gyro_Balance;           //ƽ����� ƽ�������� ת��������

extern int Balance_Pwm; 
 
 int main(void)
 {	
   NVIC_Configuration();
	 delay_init();	    	 //��ʱ������ʼ��	  
	 uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	 LED_Init();				//��ʼ����LED���ӵ�Ӳ���ӿ�
	 MotorInit(7199,0); //=====��ʼ��PWM 10KHZ�������������
	 Encoder_Init_TIM3();            //=====�������ӿ�
	 Encoder_Init_TIM2();            //=====��ʼ��������2 
	 MPU_Init();                //=====MPU6050��ʼ��	

	 LED1=0;
	while(mpu_dmp_init())//��ʼ��MPU6050
	{	
		printf("\r\nMPU6050 Error\r\n");
 		delay_ms(200);
	}
	
	TIM1_INIT(1000);               //��ʱ��1��ʼ����1ms �ж�һ��
	while(1)
	{
		LED0=0;	
 	  printf("ƽ�����%f  ���ֱ�����ֵ%d  ���ֱ�����ֵ%d \r\n",Angle_Balance,Encoder_Left,Encoder_Right);
		delay_ms(2500);	                //��ʱ�������ݴ���Ƶ�ʣ�ȷ��ͨ�ŵ��ȶ�
	  LED0=1;                                                    
	} 
 }

