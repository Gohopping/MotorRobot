#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
#include "Serial.h"
#include "motor.h"
#include "DataType.h"

#define Motor_Max_Speed           330   //电机最大转速  脉冲

typedef struct
{
	_u8 Motor_Stall_Num;           //电机档位值
	_u8 Motor_Stall_Num_Voice;     //电机档位值
	
	_s16 Motor_Speed_ValueBuf;     //RF设置的转速
	_s16 Motor_TargetSpeed_value;  //电机目标转速
	_s16 Motor_RealSpeed_valueA;    //电机实时转速
	_s16 Motor_RealSpeed_valueB;    //电机实时转速
	_s16 Motor_RealSpeed_valueC;    //电机实时转速
	_s16 Motor_RealSpeed_valueD;    //电机实时转速
	
	double  Motor_Speed_Error;              //速度误差
	double  Motor_Speed_Error_Last;         //上一次的速度误差
	double  Motor_Speed_Error_Prev;         //上一次的速度误差
	double  Motor_Speed_Add;                //速度增加值
	double  Motor_Out_PWM;                  //PWM输出值
	double  Kp; 
	double  Ki;
	double  Kd;
	_u16    Motor_PID_Delay_Contron;        //延时控制
	_u8  Light_Mode;                        //灯光模式
	_u8  Light_Motor_Fault;                 //风扇灯故障
	_u8  uart_Rev_Long;            //串口1接收数据长度
	_u8  uart_Send_Long;
	
	_u8  road_direction;          //方向
	_u8  stop_flag_A;
	_u8  stop_flag_B;
	_u8  stop_flag_C;
	_u8  stop_flag_D;
  _u8  robot_flag;
  _u8  PickUp;
  _u8  PickDown;
}PROTOCOL_INFO_STR;

typedef struct
{
	double  Motor_Speed_Error;              //速度误差
	double  Motor_Speed_Error_Last;         //上一次的速度误差
	double  Motor_Speed_Error_Prev;         //上一次的速度误差
	double  Motor_Speed_Add;                //速度增加值
	double  Motor_Out_PWM;                  //PWM输出值
	double  Kp; 
	double  Ki;
	double  Kd;

}MODE_OWN_INFO_STR;

extern MODE_OWN_INFO_STR   MODE_OWN_INFO[MOTOR_CHANNEL_NUM];
extern PROTOCOL_INFO_STR 	 PROTOCOL_INFO;

#endif

