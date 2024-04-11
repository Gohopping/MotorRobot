#include "stm32f4xx.h"                  // Device header
#include "PID.h"
#include "Encoder.h"
#include "Timer.h"
#include "Motor.h"
#include "protocol.h"

#define Dead_Voltage 5      /* ������ѹ */

//float Incremental_KP=1.1,Incremental_KI=0.09,Incremental_KD=0.25; /* ����ʽPIDϵ�� *///-1.80 -3.50

float Incremental_KP=0.5,Incremental_KI=0.15,Incremental_KD=0; /* ����ʽPIDϵ�� *///-1.80 -3.50

float Position_KP=0.10,Position_KI=0.001,Position_KD=0; /* λ��ʽPIDϵ�� KD 0.32*/

int Target_Velocity=0,Reality_Velocity=0;   /* Ŀ���ٶȣ�ʵ���ٶ� */
int Target_Position_WW=0,Target_Position_CCW = 0,Reality_PositionA=0,Reality_PositionB=0,Reality_PositionC=0,Reality_PositionD=0;   /* Ŀ��λ�ã�ʵ��λ�� */
int Pwm_Out = 0;//pwm���ֵ
int num = 0;//����Ȧ�������һλС��
int rpm_real = 0;//ʵ��ת��

void MOTOR_Init()
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��ʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��ʱ��
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5);//GPIOA4,A5
	GPIO_ResetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7);//
	GPIO_ResetBits(GPIOE,GPIO_Pin_7 | GPIO_Pin_8);//GPIOB10,B11���øߣ�����
}

int My_abs(int encoder_cnt)
{
    if(encoder_cnt>=0)
    {
        return encoder_cnt;
    }
    else
    {
        return -encoder_cnt;
    }
}

int Motor_Xianfu(int pwm)
{
    if(pwm>100) pwm=100;
    if(pwm<-100) pwm=-100;
    return pwm;
}

void Set_Pwm(MOTOR_CHANNEL_EN motor_channel,int moto)
{
	if(moto > 0)  /*��ת*/
	{
		switch(motor_channel)
		{
			case MOTOR0_CHANNEL_0:
					GPIO_SetBits(GPIOB, GPIO_Pin_10);
					GPIO_ResetBits(GPIOB, GPIO_Pin_11);
			    Pwm_Out=Motor_Xianfu(My_abs(moto)+Dead_Voltage);  /*ȡ����ֵ + ������ѹ*/
	        PWM_SetCompare1(Pwm_Out);
				break;
			
			case MOTOR1_CHANNEL_1:
				  GPIO_SetBits(GPIOA, GPIO_Pin_4);
		      GPIO_ResetBits(GPIOA, GPIO_Pin_5);
			    Pwm_Out=Motor_Xianfu(My_abs(moto)+Dead_Voltage);  /*ȡ����ֵ + ������ѹ*/
	        PWM_SetCompare2(Pwm_Out);
				break;
			case MOTOR2_CHANNEL_2:
				  GPIO_SetBits(GPIOE, GPIO_Pin_7);
		      GPIO_ResetBits(GPIOE, GPIO_Pin_8);
			    Pwm_Out=Motor_Xianfu(My_abs(moto)+Dead_Voltage);  /*ȡ����ֵ + ������ѹ*/
	        PWM_SetCompare3(Pwm_Out);
				break;
			case MOTOR3_CHANNEL_3:
				  GPIO_SetBits(GPIOC, GPIO_Pin_6);
		      GPIO_ResetBits(GPIOC, GPIO_Pin_7);
			    Pwm_Out=Motor_Xianfu(My_abs(moto)+Dead_Voltage);  /*ȡ����ֵ + ������ѹ*/
	        PWM_SetCompare4(Pwm_Out);
				break;
			default:
				break;
		}
	}
	else{         /*��ת*/
		switch(motor_channel)
		{
			case MOTOR0_CHANNEL_0:
					GPIO_ResetBits(GPIOB, GPIO_Pin_10);
					GPIO_SetBits(GPIOB, GPIO_Pin_11);
			    Pwm_Out=Motor_Xianfu(My_abs(moto)+Dead_Voltage);  /*ȡ����ֵ + ������ѹ*/
	        PWM_SetCompare1(Pwm_Out); 
				break;
			
			case MOTOR1_CHANNEL_1:
				  GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		      GPIO_SetBits(GPIOA, GPIO_Pin_5);
			    Pwm_Out=Motor_Xianfu(My_abs(moto)+Dead_Voltage);  /*ȡ����ֵ + ������ѹ*/
	        PWM_SetCompare2(Pwm_Out); 
				break;
			case MOTOR2_CHANNEL_2:
				  GPIO_ResetBits(GPIOE, GPIO_Pin_7);
		      GPIO_SetBits(GPIOE, GPIO_Pin_8);
					Pwm_Out=Motor_Xianfu(My_abs(moto)+Dead_Voltage);  /*ȡ����ֵ + ������ѹ*/
	        PWM_SetCompare3(Pwm_Out); 
				break;
			case MOTOR3_CHANNEL_3:
				  GPIO_ResetBits(GPIOC, GPIO_Pin_6);
		      GPIO_SetBits(GPIOC, GPIO_Pin_7);
					Pwm_Out=Motor_Xianfu(My_abs(moto)+Dead_Voltage);  /*ȡ����ֵ + ������ѹ*/
	        PWM_SetCompare4(Pwm_Out); 
				break;
			default:
				break;
		}
	}
}
//void  MotorPositionSet()
//{
//	static float Eorr,Pwm,Last_Eorr;
//    
//  Eorr=target-reality;                            /* ����ƫ�� */
//  Integral_Eorr+=Eorr;	                        /* ƫ���ۻ� */
//    
////    if(Integral_Eorr> 5000) Integral_Eorr = 5000;   /* �����޷� */
////    if(Integral_Eorr<-5000) Integral_Eorr =-5000;
//    
//    Pwm = (Position_KP*Eorr)                        /* �������� */
//         +(Position_KI*Integral_Eorr)               /* ���ֻ��� */
//         +(Position_KD*(Eorr-Last_Eorr));           /* ΢�ֻ��� */
//    
//    Last_Eorr=Eorr;                                 /* �����ϴ�ƫ�� */
//    return Pwm;                                     /* ������ */
//}
int xian_fu(int data,int max)
{
    if(data<-max) data=-max;	
    if(data> max) data= max;	
    return data;
}
//��1���
void  MotorPositionSetA(MOTOR_CHANNEL_EN motor_channel,int Target_Position)
{
	  static int PWM = 0;
		Reality_PositionA += TIM4_Encoder_Get();  //��ȡʵ���ٶȣ�������/10ms
    if(My_abs(My_abs(Reality_PositionA)-My_abs(Target_Position))<5)
		//if(Reality_PositionA==Target_Position)       //����Ч����0�ٶ������󣬵��ھ�ֹ�����̥�����ر�Ӳ��Ӳ-ת������
		{
			 Pwm_Out=0;
		   PWM_SetCompare1(0);                         /* ֹͣ��� */  
       PROTOCOL_INFO.stop_flag_A = 1;      
		}
		else
		{
			PWM=Position_PID(Reality_PositionA,Target_Position);
      PWM = xian_fu(PWM,49);             //λ�û�������ƣ���Ϊ�ٶȻ�������
			Set_Pwm(motor_channel,PWM);
      PROTOCOL_INFO.stop_flag_A = 0;
//      PWM=Incremental_PID(Reality_Velocity,Target_Velocity);
//		  Set_Pwm(PWM);			
		}
			
//	static int PWM = 0;
//	
//	Reality_Velocity = TIM4_Encoder_Get();  //��ȡʵ���ٶȣ�������/10ms  1000ms
//	
//	rpm_real = (Reality_Velocity*100*60)  / 1320;   //ת/ÿ����
//	
//	if((Target_Velocity==0)&& (My_abs(Reality_Velocity)<=3))       //����Ч����0�ٶ������󣬵��ھ�ֹ�����̥�����ر�Ӳ��Ӳ-ת������
//	{
//		Pwm_Out=0;
//		PWM_SetCompare1(0);                         /* ֹͣ��� */            
//	}
//	else
//	{
//		PWM=Incremental_PID(Reality_Velocity,Target_Velocity);
//		Set_Pwm(PWM);
//	}
}
//��1���
void  MotorPositionSetB(MOTOR_CHANNEL_EN motor_channel,int Target_Position)
{
	static int PWM = 0;		
	Reality_PositionB += TIM3_Encoder_Get();;   //��ȡʵ��λ�ã����������ۼ�
  
//	PWM = Position_PID(Reality_PositionB,TargetPosition); //λ�û�����
//	PWM = xian_fu(PWM,Target_Velocity);             //λ�û�������ƣ���Ϊ�ٶȻ�������
    if(My_abs(My_abs(Reality_PositionB)-My_abs(Target_Position))<5)
		//if(Reality_PositionB==Target_Position)       //����Ч����0�ٶ������󣬵��ھ�ֹ�����̥�����ر�Ӳ��Ӳ-ת������
		{
			 Pwm_Out=0;
		   PWM_SetCompare2(0);                         /* ֹͣ��� */    
       PROTOCOL_INFO.stop_flag_B = 1;        
		}
		else
		{
			PWM=Position_PID(Reality_PositionB,Target_Position);
      PWM = xian_fu(PWM,50);             //λ�û�������ƣ���Ϊ�ٶȻ�������
			Set_Pwm(motor_channel,PWM);		
      PROTOCOL_INFO.stop_flag_B = 0;      
		}
}
//��2���
void  MotorPositionSetC(MOTOR_CHANNEL_EN motor_channel,int Target_Position)
{
	static int PWM = 0;		
	Reality_PositionC += TIM1_Encoder_Get();;   //��ȡʵ��λ�ã����������ۼ�
  
//	PWM = Position_PID(Reality_PositionB,TargetPosition); //λ�û�����
//	PWM = xian_fu(PWM,Target_Velocity);             //λ�û�������ƣ���Ϊ�ٶȻ�������
    if(My_abs(My_abs(Reality_PositionC)-My_abs(Target_Position))<5)
		//if(Reality_PositionC==Target_Position)       //����Ч����0�ٶ������󣬵��ھ�ֹ�����̥�����ر�Ӳ��Ӳ-ת������
		{
			 Pwm_Out=0;
		   PWM_SetCompare3(0);                         /* ֹͣ��� */  
       PROTOCOL_INFO.stop_flag_C = 1;       
		}
		else
		{
			PWM=Position_PID(Reality_PositionC,Target_Position);
      PWM = xian_fu(PWM,50);             //λ�û�������ƣ���Ϊ�ٶȻ�������
			Set_Pwm(motor_channel,PWM);		
      PROTOCOL_INFO.stop_flag_C = 0;      
		}
}
//��2���
void  MotorPositionSetD(MOTOR_CHANNEL_EN motor_channel,int Target_Position)
{
	static int PWM = 0;		
	Reality_PositionD += TIM2_Encoder_Get();;   //��ȡʵ��λ�ã����������ۼ�
  
//	PWM = Position_PID(Reality_PositionB,TargetPosition); //λ�û�����
//	PWM = xian_fu(PWM,Target_Velocity);             //λ�û�������ƣ���Ϊ�ٶȻ�������
    if(My_abs(My_abs(Reality_PositionD)-My_abs(Target_Position))<5)
		//if(Reality_PositionD==Target_Position)       //����Ч����0�ٶ������󣬵��ھ�ֹ�����̥�����ر�Ӳ��Ӳ-ת������
		{
			 Pwm_Out=0;
		   PWM_SetCompare4(0);                         /* ֹͣ��� */   
       PROTOCOL_INFO.stop_flag_D = 1;      
		}
		else
		{
			PWM=Position_PID(Reality_PositionD,Target_Position);
      PWM = xian_fu(PWM,49);           //λ�û�������ƣ���Ϊ�ٶȻ�������
			Set_Pwm(motor_channel,PWM);	
      PROTOCOL_INFO.stop_flag_D = 0;      
		}
}
//void rpm_xianfu(void)
//{
//    if (rpm>=250)
//    {
//        rpm=250;
//    }
//    if (rpm<=-250)
//        
//    {
//        rpm=-250;
//    }
//}

