#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "sys.h"


#define PI 3.14159265
#define ZHONGZHI 0


extern	int Balance_Pwm,Velocity_Pwm;
int balance(float angle,float gyro);
int velocity(int encoder_left,int encoder_right);
void Set_Pwm(int moto1,int moto2);
void Xianfu_Pwm(void);
u8 Turn_Off(float angle);
void Get_Angle(void);
int myabs(int a);
	
#endif
