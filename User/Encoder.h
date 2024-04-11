#ifndef __ENCODER_H
#define __ENCODER_H

#include<stdint.h>

void TIM_Encoder_Init();
void TIM3_Encoder_Init(void);  //Moter_B
void TIM4_Encoder_Init(void);
int TIM1_Encoder_Get(void);
int TIM2_Encoder_Get(void);
int TIM3_Encoder_Get(void);
int TIM4_Encoder_Get(void);
long Num_Encoder_Cnt(float num,uint16_t ppr,float ratio);
long Rpm_Encoder_Cnt(float rpm,uint16_t ppr,uint16_t ratio,uint16_t cnt_time);

#endif
