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

int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数
int Moto1,Moto2;                            //电机PWM变量 应是Motor的 向Moto致敬	
float Angle_Balance,Gyro_Balance;           //平衡倾角 平衡陀螺仪 转向陀螺仪

extern int Balance_Pwm; 
 
 int main(void)
 {	
   NVIC_Configuration();
	 delay_init();	    	 //延时函数初始化	  
	 uart_init(115200);	 	//串口初始化为115200
	 LED_Init();				//初始化与LED连接的硬件接口
	 MotorInit(7199,0); //=====初始化PWM 10KHZ，用于驱动电机
	 Encoder_Init_TIM3();            //=====编码器接口
	 Encoder_Init_TIM2();            //=====初始化编码器2 
	 MPU_Init();                //=====MPU6050初始化	

	 LED1=0;
	while(mpu_dmp_init())//初始化MPU6050
	{	
		printf("\r\nMPU6050 Error\r\n");
 		delay_ms(200);
	}
	
	TIM1_INIT(1000);               //定时器1初始化，1ms 中断一次
	while(1)
	{
		LED0=0;	
 	  printf("平衡倾角%f  左轮编码器值%d  右轮编码器值%d \r\n",Angle_Balance,Encoder_Left,Encoder_Right);
		delay_ms(2500);	                //延时减缓数据传输频率，确保通信的稳定
	  LED0=1;                                                    
	} 
 }

