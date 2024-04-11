#ifndef __MOTOR_H
#define __MOTOR_H

#include<stdint.h>

typedef enum
{
	MOTOR0_CHANNEL_0 = 0,
	MOTOR1_CHANNEL_1,
	MOTOR2_CHANNEL_2,
	MOTOR3_CHANNEL_3,

  MOTOR_CHANNEL_NUM

}MOTOR_CHANNEL_EN;

typedef enum
{
	FORWARD = 0,
	RIGHT_SHIFT,
	BACK,
	LEFT_SHIFT,

  DIRECTION_NUM

}DIRECTION_EN;

extern int Pwm_Out;
extern float Incremental_KP,Incremental_KI,Incremental_KD;
extern float Position_KP,Position_KI,Position_KD; 
extern int num;
extern int rpm_real;
extern int Target_Position_WW,Target_Position_CCW,Reality_PositionA,Reality_PositionB,Reality_PositionC,Reality_PositionD;   /* 目标位置，实际位置 */

void MOTOR_Init();
void rpm_xianfu(void);
void MotorPositionSetA(MOTOR_CHANNEL_EN motor_channel,int TargetPosition);
void  MotorPositionSetB(MOTOR_CHANNEL_EN motor_channel,int TargetPosition);
void MotorPositionSetC(MOTOR_CHANNEL_EN motor_channel,int TargetPosition);
void  MotorPositionSetD(MOTOR_CHANNEL_EN motor_channel,int TargetPosition);
void Set_Pwm(MOTOR_CHANNEL_EN motor_channel,int moto);


#endif
