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
AHB����Ƶ��  168MHz
APB1����Ƶ�� 84MHz
APB2����Ƶ�� 168MHz

�����1    ��������ʱ��TIM4
1. PB10 PB11 ������ƶ�
2. PA0 ���PWM
3. PD12��PD13(TIM4_CH1\TIM4_CH2 ����������)

�����2     ��������ʱ��TIM3
1. PA4 PA5 ������ƶ�
2. PA1 ���PWM
3. PA6��PA7 (TIM3_CH1\TIM3_CH2 ����������)

�����1    ��������ʱ��TIM1
1. PE7 PE8 ������ƶ�
2. PA2 ���PWM
3. PE9��PE11(TIM1_CH1\TIM1_CH2 ����������)

�����2     ��������ʱ��TIM2
1. PC6 PC7 ������ƶ�
2. PA3 ���PWM
3. PA15��PB3(TIM2_CH1\TIM2_CH2 ����������)
******************************************/

uint8_t RxData;
extern float Integral_Eorr;
short direction = 0;

int main(void)
{
	SysTick_Init();
	MOTOR_Init();             //������ƶ�
  TIM5_PWM_Init(100-1,84-1); //���PWM
	TIM_Encoder_Init();        //���������
	OLED_Init();               //OLED��ʼ��
	Serial_Init();             //���ڳ�ʼ��
  
  num = 0;         //�趨Ȧ��
  rpm_real = 200;
  short count = 0;
  
  /* ���ƶ������ĽǶ� */
//	FSUSExample_SetNServoAngle();
  InitArm();
//  Stock();
	while(1)
	{
		
		if(TIME_BASE.fbit.sys_flag_5ms)
		{
			TIME_BASE.fbit.sys_flag_5ms = 0;
			Target_Velocity=Rpm_Encoder_Cnt(rpm_real,11,30,10);   /* ��ת��ת��Ϊ10ms����������Ŀ���ٶ�   ת/ÿ����*/
			Target_Position_WW=Num_Encoder_Cnt(num,11,30);      /* ��Ȧ��ת��ΪĿ����������Ŀ��λ�� */
			Target_Position_CCW=Num_Encoder_Cnt(-num,11,30);
			//Data_send(Target_Position,Reality_Position,num,Pwm_Out);//Ŀ����������ʵ����������ת�٣�pwm��� Integral_Eorr
		OLED_ShowChar(2,1,'A');
		}
		if(TIME_BASE.fbit.sys_flag_10ms)
		{
			TIME_BASE.fbit.sys_flag_10ms = 0;
			//λ��ʽ
//			MotorPositionSetA(MOTOR0_CHANNEL_0);
//			MotorPositionSetB(MOTOR1_CHANNEL_1);
//      MotorPositionSetC(MOTOR2_CHANNEL_2);
//			MotorPositionSetD(MOTOR3_CHANNEL_3);
			/*************ǰ��*********/
//			Set_Pwm(MOTOR0_CHANNEL_0,30);
//			Set_Pwm(MOTOR1_CHANNEL_1,30);
//			Set_Pwm(MOTOR2_CHANNEL_2,30);
//			Set_Pwm(MOTOR3_CHANNEL_3,30);
			
			/*************����*********/
//			Set_Pwm(MOTOR0_CHANNEL_0,-30);
//			Set_Pwm(MOTOR1_CHANNEL_1,-30);
//			Set_Pwm(MOTOR2_CHANNEL_2,-30);
//			Set_Pwm(MOTOR3_CHANNEL_3,-30);
      /************�����˶�*********************/
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
            MotorPositionSetA(MOTOR0_CHANNEL_0,Target_Position_WW);    //��ǰ�� OK
            MotorPositionSetB(MOTOR1_CHANNEL_1,Target_Position_WW);    //��ǰ�� 
            MotorPositionSetC(MOTOR2_CHANNEL_2,Target_Position_WW);    //�����
            MotorPositionSetD(MOTOR3_CHANNEL_3,Target_Position_WW);    //�Һ���
            break;
          case RIGHT_SHIFT:
  //					MotorSpeedSetA(MOTOR0_CHANNEL_0,2*Target_Position_WW);        //��ǰ��  ��
  //  			  MotorSpeedSetB(MOTOR1_CHANNEL_1,0);                        //��ǰ��  ��
  //				  MotorSpeedSetC(MOTOR2_CHANNEL_2,2*Target_Position_WW);        //�����  ��
  //  		    MotorSpeedSetD(MOTOR3_CHANNEL_3,0);
  //          PickUp();
            MotorPositionSetA(MOTOR0_CHANNEL_0,Target_Position_WW);        //��ǰ��  ��
            MotorPositionSetB(MOTOR1_CHANNEL_1,Target_Position_CCW);                        //��ǰ��  ��
            MotorPositionSetC(MOTOR2_CHANNEL_2,Target_Position_WW);        //�����  ��
            MotorPositionSetD(MOTOR3_CHANNEL_3,Target_Position_CCW);
            break;
          case BACK:
  //					MotorSpeedSetA(MOTOR0_CHANNEL_0,Target_Position_WW);    //��ǰ�� OK
  //				  MotorSpeedSetB(MOTOR1_CHANNEL_1,Target_Position_CCW);    //��ǰ�� 
  //				  MotorSpeedSetC(MOTOR2_CHANNEL_2,Target_Position_WW);    //�����
  //				  MotorSpeedSetD(MOTOR3_CHANNEL_3,Target_Position_CCW);    //�Һ���
          
            MotorPositionSetA(MOTOR0_CHANNEL_0,Target_Position_CCW);        //��ǰ��  ��
            MotorPositionSetB(MOTOR1_CHANNEL_1,Target_Position_CCW);                        //��ǰ��  ��
            MotorPositionSetC(MOTOR2_CHANNEL_2,Target_Position_CCW);        //�����  ��
            MotorPositionSetD(MOTOR3_CHANNEL_3,Target_Position_CCW);
            break;
          case LEFT_SHIFT:
  //					MotorSpeedSetA(MOTOR0_CHANNEL_0,0);    //��ǰ�� OK
  //				  MotorSpeedSetB(MOTOR1_CHANNEL_1,0);    //��ǰ�� 
  //				  MotorSpeedSetC(MOTOR2_CHANNEL_2,0);    //�����
  //				  MotorSpeedSetD(MOTOR3_CHANNEL_3,0);    //�Һ���
          
            MotorPositionSetA(MOTOR0_CHANNEL_0,Target_Position_CCW);        //��ǰ��  ��
            MotorPositionSetB(MOTOR1_CHANNEL_1,Target_Position_WW);                        //��ǰ��  ��
            MotorPositionSetC(MOTOR2_CHANNEL_2,Target_Position_CCW);        //�����  ��
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
