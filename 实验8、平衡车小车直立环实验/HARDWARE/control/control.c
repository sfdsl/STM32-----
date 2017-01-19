#include "control.h"
#include "motor_pwm.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "timer.h"
#include "encord.h"

extern int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数
extern int Moto1,Moto2;                            //电机PWM变量 应是Motor的 向Moto致敬	
extern float Angle_Balance,Gyro_Balance;           //平衡倾角 平衡陀螺仪 转向陀螺仪


int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
u8 Flag_Target,yanshi_count,yanshi_flag;
/**************************************************************************
函数功能：所有的控制代码都在这里面
         1ms定时中断由TIM1定时器触发
         严格保证采样和数据处理的时间同步				 
**************************************************************************/
int TIM1_UP_IRQHandler(void) 
{  
  static u8 ms5 = 0,ms8=0;				//中断次数计数器  
	 if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
	 {
		 TIM_ClearITPendingBit(TIM1 , TIM_IT_Update); //清除中断标志
	   TIM1_IRQCNT ++;
		  ms5++;	 
		  ms8++;
		 if(ms5==3)
		{
			ms5=0;					//每3次中断执行一次,3ms
			Flag_Target=!Flag_Target;
		  if(Flag_Target==1)                                                  //6ms读取一次陀螺仪和加速度计的值，更高的采样频率可以改善卡尔曼滤波和互补滤波的效果
			{
			Get_Angle();                                                        //===更新姿态	
			return 0;	                                               
			}
	  	Get_Angle();                                                        //===更新姿态	                                                            //===扫描按键状态 单击双击可以改变小车运行状态
 			Balance_Pwm =balance(Angle_Balance,Gyro_Balance);                   //===平衡PID控制	
		  Velocity_Pwm=velocity(Encoder_Left,Encoder_Right);                  //===速度环PID控制	 记住，速度反馈是正反馈，就是小车快的时候要慢下来就需要再跑快一点
 	    Moto1=Balance_Pwm-Velocity_Pwm;                                     //===计算左轮电机最终PWM
 	  	Moto2=Balance_Pwm-Velocity_Pwm;                                    //===计算右轮电机最终PWM
   		Xianfu_Pwm();                                                       //===PWM限幅
      if(Turn_Off(Angle_Balance)==0)                                      //===如果不存在异常	
		   Set_Pwm(Moto1,Moto2);                                               //===赋值给PWM寄存器  			
		}	
		if(ms8==12)//12ms
		{
		ms8=0;
		Encoder_Left=-Read_Encoder(2);//===读取编码器的值，因为两个电机的旋转了180度的，所以对其中一个取反，保证输出极性一致
		Encoder_Right=Read_Encoder(3);//===读取编码器的值
		}
	 }
  return 0;
} 
/**************************************************************************
函数功能：直立PD控制
入口参数：角度、角速度
返回  值：直立控制PWM
作    者：平衡小车之家

500,2 小车较稳  不能负重
800,3.2 小车跑偏，能负重0.6KG
800，3.0 小车跑
**************************************************************************/
int balance(float Angle,float Gyro)
{  
   float Bias,kp=800,kd=3.2;
	 int balance;
	 Bias=Angle-ZHONGZHI;       //===求出平衡的角度中值 和机械相关
	 balance=Gyro*kd+kp*Bias;   //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return balance;
}

/**************************************************************************
函数功能：速度PI控制 修改前进后退速度，请修Target_Velocity，比如，改成60就比较慢了
入口参数：左轮编码器、右轮编码器
返回  值：速度控制PWM
作    者：
**************************************************************************/
int velocity(int encoder_left,int encoder_right)
{  
    static float Velocity,Encoder_Least,Encoder,Movement;
	  static float Encoder_Integral;
	  float kp=90,ki=0.50;
	  //=============速度PI控制器=======================//	
		Encoder_Least =(Encoder_Left+Encoder_Right)-0;                    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
		Encoder *= 0.8;		                                                //===一阶低通滤波器       
		Encoder += Encoder_Least*0.2;	                                    //===一阶低通滤波器    
		Encoder_Integral +=Encoder;                                       //===积分出位移 积分时间：10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //===接收遥控器数据，控制前进后退
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===积分限幅
		if(Encoder_Integral<-10000)	Encoder_Integral=-10000;              //===积分限幅	
		Velocity=Encoder*kp+Encoder_Integral*ki;                          //===速度控制	
		return Velocity;
}

/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
	  u16 PWM1,PWM2;
    	if(moto1<0)	{	MOTOR_A_CON2=1;	MOTOR_A_CON1=0;}
			else 	        {MOTOR_A_CON2=0;	MOTOR_A_CON1=1; } 	
			
			PWM1=myabs(moto1)+2500;
			TIM_SetCompare1(TIM8,PWM1);	
			
		  if(moto2<0)  { MOTOR_B_CON1=0;MOTOR_B_CON2=1;	}
			else         { MOTOR_B_CON1=1;MOTOR_B_CON2=0; }
		
			PWM2=myabs(moto2)+2500;
      TIM_SetCompare2(TIM8,PWM2);	
			
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
 void Xianfu_Pwm(void)
{	
	  int Amplitude=6900;    //===PWM满幅是7200 限制在6900
    if(Moto1<-Amplitude) Moto1=-Amplitude;	
		if(Moto1>Amplitude)  Moto1=Amplitude;	
	  if(Moto2<-Amplitude) Moto2=-Amplitude;	
		if(Moto2>Amplitude)  Moto2=Amplitude;		
	
}

/**************************************************************************
函数功能：异常关闭电机
入口参数：倾角和电压
返回  值：1：异常  0：正常
**************************************************************************/
u8 Turn_Off(float angle)
{
	    u8 temp;
			if(angle<-40||angle>40)
			{	                                                 //===倾角大于40度关闭电机
      temp=1;                                            //===Flag_Stop置1关闭电机
			                                          //===可自行增加主板温度过高时关闭电机
			MOTOR_GO_STOP;
      }
			else
      temp=0;
      return temp;			
}
	
/**************************************************************************
函数功能：获取角度 三种算法经过我们的调校，都非常理想 
入口参数：获取角度的算法 1：DMP  2：卡尔曼 3：互补滤波
返回  值：无
**************************************************************************/
void Get_Angle(void)
{ 
					mpu_dmp_get_data();                     //===读取加速度、角速度、倾角
					Angle_Balance=Pitch;             //===更新平衡倾角
					Gyro_Balance=gyro[1];            //===更新平衡角速度
}
/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
