#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

void SysTick_Init(void);				//��ʱ������ʼ��
// ��ʱ us
//void SysTick_DelayUs(__IO u32 nTime);
// ��ʱ ms
void SysTick_DelayMs(__IO u32 nTime);
// ��ʱ s
void SysTick_DelayS(__IO u32 nTime);

// ����ʱ
// ���õ���ʱ(������ʽ)
void SysTick_CountdownBegin(__IO u32 nTime);
// ��������ʱ
void SysTick_CountdownCancel(void);
// �жϵ���ʱ�Ƿ�ʱ
uint8_t SysTick_CountdownIsTimeout(void);

#endif //__DELAY_H

