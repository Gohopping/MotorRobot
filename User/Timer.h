#ifndef __TIMER_H
#define __TIMER_H

#include<stdint.h>

extern int Target_Velocity,Reality_Velocity;   /* Ŀ���ٶȣ�ʵ���ٶ� */

void TIM5_PWM_Init(uint16_t arr,uint16_t psc);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare2(uint16_t Compare);
void PWM_SetCompare3(uint16_t Compare);
void PWM_SetCompare4(uint16_t Compare);

#endif
