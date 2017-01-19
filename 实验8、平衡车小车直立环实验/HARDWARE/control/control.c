#include "control.h"
#include "motor_pwm.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "timer.h"
#include "encord.h"

extern int Encoder_Left,Encoder_Right;             //���ұ��������������
extern int Moto1,Moto2;                            //���PWM���� Ӧ��Motor�� ��Moto�¾�	
extern float Angle_Balance,Gyro_Balance;           //ƽ����� ƽ�������� ת��������


int Balance_Pwm,Velocity_Pwm,Turn_Pwm;
u8 Flag_Target,yanshi_count,yanshi_flag;
/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         1ms��ʱ�ж���TIM1��ʱ������
         �ϸ�֤���������ݴ����ʱ��ͬ��				 
**************************************************************************/
int TIM1_UP_IRQHandler(void) 
{  
  static u8 ms5 = 0,ms8=0;				//�жϴ���������  
	 if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
	 {
		 TIM_ClearITPendingBit(TIM1 , TIM_IT_Update); //����жϱ�־
	   TIM1_IRQCNT ++;
		  ms5++;	 
		  ms8++;
		 if(ms5==3)
		{
			ms5=0;					//ÿ3���ж�ִ��һ��,3ms
			Flag_Target=!Flag_Target;
		  if(Flag_Target==1)                                                  //6ms��ȡһ�������Ǻͼ��ٶȼƵ�ֵ�����ߵĲ���Ƶ�ʿ��Ը��ƿ������˲��ͻ����˲���Ч��
			{
			Get_Angle();                                                        //===������̬	
			return 0;	                                               
			}
	  	Get_Angle();                                                        //===������̬	                                                            //===ɨ�谴��״̬ ����˫�����Ըı�С������״̬
 			Balance_Pwm =balance(Angle_Balance,Gyro_Balance);                   //===ƽ��PID����	
		  Velocity_Pwm=velocity(Encoder_Left,Encoder_Right);                  //===�ٶȻ�PID����	 ��ס���ٶȷ�����������������С�����ʱ��Ҫ����������Ҫ���ܿ�һ��
 	    Moto1=Balance_Pwm-Velocity_Pwm;                                     //===�������ֵ������PWM
 	  	Moto2=Balance_Pwm-Velocity_Pwm;                                    //===�������ֵ������PWM
   		Xianfu_Pwm();                                                       //===PWM�޷�
      if(Turn_Off(Angle_Balance)==0)                                      //===����������쳣	
		   Set_Pwm(Moto1,Moto2);                                               //===��ֵ��PWM�Ĵ���  			
		}	
		if(ms8==12)//12ms
		{
		ms8=0;
		Encoder_Left=-Read_Encoder(2);//===��ȡ��������ֵ����Ϊ�����������ת��180�ȵģ����Զ�����һ��ȡ������֤�������һ��
		Encoder_Right=Read_Encoder(3);//===��ȡ��������ֵ
		}
	 }
  return 0;
} 
/**************************************************************************
�������ܣ�ֱ��PD����
��ڲ������Ƕȡ����ٶ�
����  ֵ��ֱ������PWM
��    �ߣ�ƽ��С��֮��

500,2 С������  ���ܸ���
800,3.2 С����ƫ���ܸ���0.6KG
800��3.0 С����
**************************************************************************/
int balance(float Angle,float Gyro)
{  
   float Bias,kp=800,kd=3.2;
	 int balance;
	 Bias=Angle-ZHONGZHI;       //===���ƽ��ĽǶ���ֵ �ͻ�е���
	 balance=Gyro*kd+kp*Bias;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}

/**************************************************************************
�������ܣ��ٶ�PI���� �޸�ǰ�������ٶȣ�����Target_Velocity�����磬�ĳ�60�ͱȽ�����
��ڲ��������ֱ����������ֱ�����
����  ֵ���ٶȿ���PWM
��    �ߣ�
**************************************************************************/
int velocity(int encoder_left,int encoder_right)
{  
    static float Velocity,Encoder_Least,Encoder,Movement;
	  static float Encoder_Integral;
	  float kp=90,ki=0.50;
	  //=============�ٶ�PI������=======================//	
		Encoder_Least =(Encoder_Left+Encoder_Right)-0;                    //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder *= 0.8;		                                                //===һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.2;	                                    //===һ�׵�ͨ�˲���    
		Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //===����ң�������ݣ�����ǰ������
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===�����޷�
		if(Encoder_Integral<-10000)	Encoder_Integral=-10000;              //===�����޷�	
		Velocity=Encoder*kp+Encoder_Integral*ki;                          //===�ٶȿ���	
		return Velocity;
}

/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
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
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
 void Xianfu_Pwm(void)
{	
	  int Amplitude=6900;    //===PWM������7200 ������6900
    if(Moto1<-Amplitude) Moto1=-Amplitude;	
		if(Moto1>Amplitude)  Moto1=Amplitude;	
	  if(Moto2<-Amplitude) Moto2=-Amplitude;	
		if(Moto2>Amplitude)  Moto2=Amplitude;		
	
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������Ǻ͵�ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
u8 Turn_Off(float angle)
{
	    u8 temp;
			if(angle<-40||angle>40)
			{	                                                 //===��Ǵ���40�ȹرյ��
      temp=1;                                            //===Flag_Stop��1�رյ��
			                                          //===���������������¶ȹ���ʱ�رյ��
			MOTOR_GO_STOP;
      }
			else
      temp=0;
      return temp;			
}
	
/**************************************************************************
�������ܣ���ȡ�Ƕ� �����㷨�������ǵĵ�У�����ǳ����� 
��ڲ�������ȡ�Ƕȵ��㷨 1��DMP  2�������� 3�������˲�
����  ֵ����
**************************************************************************/
void Get_Angle(void)
{ 
					mpu_dmp_get_data();                     //===��ȡ���ٶȡ����ٶȡ����
					Angle_Balance=Pitch;             //===����ƽ�����
					Gyro_Balance=gyro[1];            //===����ƽ����ٶ�
}
/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
