#include "stm32f4xx.h"                  // Device header
#include "Motor.h"

float Integral_Eorr=0;

/****
函数：增量式PID控制
参数：实际值  目标值
返回值：pwm
ΔPWM = Kp[e(k)-e(k-1)] + Ki*e(k) + Kd[e(k)-2e(k-1)+e(k-2)]
****/
int Incremental_PID(int reality,int target)
{ 	
	 static float Eorr,Pwm,Last_Eorr=0,Prev_Eorr=0,PwmOut;
    
	 Eorr=target-reality;                                   /* 本次偏差 计算偏差 */

	 Pwm = (Incremental_KP*(Eorr-Last_Eorr))                 /* 比例环节 */
           +(Incremental_KI*Eorr)                          /* 积分环节 */
           +(Incremental_KD*(Eorr-2*Last_Eorr+Prev_Eorr)); /* 微分环节 */ 
    
	 Prev_Eorr=Last_Eorr;                                 /* 保存上上次偏差 */
	 Last_Eorr=Eorr;	                                    /* 保存上一次偏差 */
   
   //限制PWM 保护电机驱动	
//	 if(Pwm >= 50)
//	 {
//		 Pwm = 50;
//	 }
//	 if(Pwm <= -50)
//	 {
//		 Pwm = -50;
//	 }
	 
	 PwmOut += Pwm;
	 return PwmOut;                                            /* 输出结果 */
}


int Position_PID(int reality,int target)
{
	static float Eorr,Pwm,Last_Eorr;

	Eorr=target-reality;                            /* 计算偏差 */
	Integral_Eorr+=Eorr;	                        /* 偏差累积 */

	if(Integral_Eorr> 5000) Integral_Eorr = 5000;   /* 积分限幅 */
	if(Integral_Eorr<-5000) Integral_Eorr =-5000;

	Pwm = (Position_KP*Eorr)                        /* 比例环节 */
		 +(Position_KI*Integral_Eorr)               /* 积分环节 */
		 +(Position_KD*(Eorr-Last_Eorr));           /* 微分环节 */

	Last_Eorr=Eorr;                                 /* 保存上次偏差 */
	return Pwm;                                     /* 输出结果 */
}

