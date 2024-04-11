#include "stm32f4xx.h"
#include "delay.h"
#include "Timer.h"
#include "Motor.h"
#include "Encoder.h"
#include "OLED.h"
#include "GlobalFunction.h"
#include "Serial.h"
#include "Ano.h"
#include "protocol.h"

/***************************************
AHB总线频率  168MHz
APB1总线频率 84MHz
APB2总线频率 168MHz

电机右1    编码器定时器TIM4
1. PB10 PB11 电机控制端
2. PA0 电机PWM
3. PD12、PD13(TIM4_CH1\TIM4_CH2 正交编码器)

电机右2     编码器定时器TIM3
1. PA4 PA5 电机控制端
2. PA1 电机PWM
3. PA6、PA7 (TIM3_CH1\TIM3_CH2 正交编码器)

电机左1    编码器定时器TIM1
1. PE7 PE8 电机控制端
2. PA2 电机PWM
3. PE9、PE11(TIM1_CH1\TIM1_CH2 正交编码器)

电机左2     编码器定时器TIM2
1. PC6 PC7 电机控制端
2. PA3 电机PWM
3. PA15、PB3(TIM2_CH1\TIM2_CH2 正交编码器)
******************************************/

uint8_t RxData;
extern float Integral_Eorr;
short direction = 0;

int main(void)
{
	SysTick_Init();
	MOTOR_Init();             //电机控制端
  TIM5_PWM_Init(100-1,84-1); //电机PWM
	TIM_Encoder_Init();        //电机编码器
	OLED_Init();               //OLED初始化
	Serial_Init();             //串口初始化
  
  num = 0;         //设定圈数
  rpm_real = 200;
  short count = 0;
  
  /* 控制多个舵机的角度 */
//	FSUSExample_SetNServoAngle();
  InitArm();
//  Stock();
	while(1)
	{
		
		if(TIME_BASE.fbit.sys_flag_5ms)
		{
			TIME_BASE.fbit.sys_flag_5ms = 0;
			Target_Velocity=Rpm_Encoder_Cnt(rpm_real,11,30,10);   /* 将转速转化为10ms的脉冲数，目标速度   转/每分钟*/
			Target_Position_WW=Num_Encoder_Cnt(num,11,30);      /* 将圈数转化为目标脉冲数，目标位置 */
			Target_Position_CCW=Num_Encoder_Cnt(-num,11,30);
			//Data_send(Target_Position,Reality_Position,num,Pwm_Out);//目标脉冲数，实际脉冲数，转速，pwm输出 Integral_Eorr
		OLED_ShowChar(2,1,'A');
		}
		if(TIME_BASE.fbit.sys_flag_10ms)
		{
			TIME_BASE.fbit.sys_flag_10ms = 0;
			//位置式
//			MotorPositionSetA(MOTOR0_CHANNEL_0);
//			MotorPositionSetB(MOTOR1_CHANNEL_1);
//      MotorPositionSetC(MOTOR2_CHANNEL_2);
//			MotorPositionSetD(MOTOR3_CHANNEL_3);
			/*************前进*********/
//			Set_Pwm(MOTOR0_CHANNEL_0,30);
//			Set_Pwm(MOTOR1_CHANNEL_1,30);
//			Set_Pwm(MOTOR2_CHANNEL_2,30);
//			Set_Pwm(MOTOR3_CHANNEL_3,30);
			
			/*************后退*********/
//			Set_Pwm(MOTOR0_CHANNEL_0,-30);
//			Set_Pwm(MOTOR1_CHANNEL_1,-30);
//			Set_Pwm(MOTOR2_CHANNEL_2,-30);
//			Set_Pwm(MOTOR3_CHANNEL_3,-30);
      /************方形运动*********************/
      if(((PROTOCOL_INFO.stop_flag_A == 1) || (PROTOCOL_INFO.stop_flag_B == 1))
			|| ((PROTOCOL_INFO.stop_flag_C == 1) || (PROTOCOL_INFO.stop_flag_D == 1)))
			{
        if((0 == PROTOCOL_INFO.PickUp) && (0 == PROTOCOL_INFO.PickDown))
        {
          count++;
          if(count >= 100)
          {
            count = 0;
            direction++;
            Reality_PositionA = 0;
            Reality_PositionB = 0;
            Reality_PositionC = 0;
            Reality_PositionD = 0;
            if(RIGHT_SHIFT == direction)
            {
              PROTOCOL_INFO.PickUp = 1;
            }
            if(BACK == direction)
            {
              PROTOCOL_INFO.PickDown = 1;
            }
          }
        }
			}
      if((0 == PROTOCOL_INFO.PickUp) && (0 == PROTOCOL_INFO.PickDown))
      {
          switch(direction)
        {
          case FORWARD:
            MotorPositionSetA(MOTOR0_CHANNEL_0,Target_Position_WW);    //右前轮 OK
            MotorPositionSetB(MOTOR1_CHANNEL_1,Target_Position_WW);    //左前轮 
            MotorPositionSetC(MOTOR2_CHANNEL_2,Target_Position_WW);    //左后轮
            MotorPositionSetD(MOTOR3_CHANNEL_3,Target_Position_WW);    //右后轮
            break;
          case RIGHT_SHIFT:
  //					MotorSpeedSetA(MOTOR0_CHANNEL_0,2*Target_Position_WW);        //右前轮  ↑
  //  			  MotorSpeedSetB(MOTOR1_CHANNEL_1,0);                        //左前轮  ↑
  //				  MotorSpeedSetC(MOTOR2_CHANNEL_2,2*Target_Position_WW);        //左后轮  ↑
  //  		    MotorSpeedSetD(MOTOR3_CHANNEL_3,0);
  //          PickUp();
            MotorPositionSetA(MOTOR0_CHANNEL_0,Target_Position_WW);        //右前轮  ↑
            MotorPositionSetB(MOTOR1_CHANNEL_1,Target_Position_CCW);                        //左前轮  ↑
            MotorPositionSetC(MOTOR2_CHANNEL_2,Target_Position_WW);        //左后轮  ↑
            MotorPositionSetD(MOTOR3_CHANNEL_3,Target_Position_CCW);
            break;
          case BACK:
  //					MotorSpeedSetA(MOTOR0_CHANNEL_0,Target_Position_WW);    //右前轮 OK
  //				  MotorSpeedSetB(MOTOR1_CHANNEL_1,Target_Position_CCW);    //左前轮 
  //				  MotorSpeedSetC(MOTOR2_CHANNEL_2,Target_Position_WW);    //左后轮
  //				  MotorSpeedSetD(MOTOR3_CHANNEL_3,Target_Position_CCW);    //右后轮
          
            MotorPositionSetA(MOTOR0_CHANNEL_0,Target_Position_CCW);        //右前轮  ↑
            MotorPositionSetB(MOTOR1_CHANNEL_1,Target_Position_CCW);                        //左前轮  ↑
            MotorPositionSetC(MOTOR2_CHANNEL_2,Target_Position_CCW);        //左后轮  ↑
            MotorPositionSetD(MOTOR3_CHANNEL_3,Target_Position_CCW);
            break;
          case LEFT_SHIFT:
  //					MotorSpeedSetA(MOTOR0_CHANNEL_0,0);    //右前轮 OK
  //				  MotorSpeedSetB(MOTOR1_CHANNEL_1,0);    //左前轮 
  //				  MotorSpeedSetC(MOTOR2_CHANNEL_2,0);    //左后轮
  //				  MotorSpeedSetD(MOTOR3_CHANNEL_3,0);    //右后轮
          
            MotorPositionSetA(MOTOR0_CHANNEL_0,Target_Position_CCW);        //右前轮  ↑
            MotorPositionSetB(MOTOR1_CHANNEL_1,Target_Position_WW);                        //左前轮  ↑
            MotorPositionSetC(MOTOR2_CHANNEL_2,Target_Position_CCW);        //左后轮  ↑
            MotorPositionSetD(MOTOR3_CHANNEL_3,Target_Position_WW);
            break;
          default:
            direction = 5;
            break;
        }
      }
		}
		if(TIME_BASE.fbit.sys_flag_50ms)
		{
			TIME_BASE.fbit.sys_flag_50ms = 0;
      if(PROTOCOL_INFO.PickUp)
      {
        PROTOCOL_INFO.PickUp = 0;
        PickUp();
				//modeyidong();
        Stock();
      }
      if(PROTOCOL_INFO.PickDown)
      {
        PROTOCOL_INFO.PickDown = 0;
        PutDown();
        //Recover();
      }
	 	}
		if(TIME_BASE.fbit.sys_flag_250ms)
		{
			TIME_BASE.fbit.sys_flag_250ms = 0;
		}
	}
}
