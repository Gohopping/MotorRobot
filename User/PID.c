#include "stm32f4xx.h"                  // Device header
#include "Motor.h"

float Integral_Eorr=0;

/****
����������ʽPID����
������ʵ��ֵ  Ŀ��ֵ
����ֵ��pwm
��PWM = Kp[e(k)-e(k-1)] + Ki*e(k) + Kd[e(k)-2e(k-1)+e(k-2)]
****/
int Incremental_PID(int reality,int target)
{ 	
	 static float Eorr,Pwm,Last_Eorr=0,Prev_Eorr=0,PwmOut;
    
	 Eorr=target-reality;                                   /* ����ƫ�� ����ƫ�� */

	 Pwm = (Incremental_KP*(Eorr-Last_Eorr))                 /* �������� */
           +(Incremental_KI*Eorr)                          /* ���ֻ��� */
           +(Incremental_KD*(Eorr-2*Last_Eorr+Prev_Eorr)); /* ΢�ֻ��� */ 
    
	 Prev_Eorr=Last_Eorr;                                 /* �������ϴ�ƫ�� */
	 Last_Eorr=Eorr;	                                    /* ������һ��ƫ�� */
   
   //����PWM �����������	
//	 if(Pwm >= 50)
//	 {
//		 Pwm = 50;
//	 }
//	 if(Pwm <= -50)
//	 {
//		 Pwm = -50;
//	 }
	 
	 PwmOut += Pwm;
	 return PwmOut;                                            /* ������ */
}


int Position_PID(int reality,int target)
{
	static float Eorr,Pwm,Last_Eorr;

	Eorr=target-reality;                            /* ����ƫ�� */
	Integral_Eorr+=Eorr;	                        /* ƫ���ۻ� */

	if(Integral_Eorr> 5000) Integral_Eorr = 5000;   /* �����޷� */
	if(Integral_Eorr<-5000) Integral_Eorr =-5000;

	Pwm = (Position_KP*Eorr)                        /* �������� */
		 +(Position_KI*Integral_Eorr)               /* ���ֻ��� */
		 +(Position_KD*(Eorr-Last_Eorr));           /* ΢�ֻ��� */

	Last_Eorr=Eorr;                                 /* �����ϴ�ƫ�� */
	return Pwm;                                     /* ������ */
}

