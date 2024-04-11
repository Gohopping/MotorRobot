/***
	***************************************************************************
	*	@file  	delay.c
	*	@brief   delay�ӿ���غ���
   ***************************************************************************
   *  @description
	*
	*  SysTick��ʱ������Ϊ1ms�жϣ�ʵ�ֺ�����ʱ
	* 	
	***************************************************************************
***/

#include "delay.h"
#include "GlobalFunction.h"

static __IO uint32_t TimingDelay;  //��������
static __IO u32 sysTickCnt;

//	��������ʱ��ʼ��
//	˵�������� SysTick Ϊ1ms�жϣ���������ʱ��
//
void SysTick_Init(void)
{
	SysTick_Config(SystemCoreClock / 1000);  //����SysTickʱ��Ϊ1ms�ж�

	 // ��������Ϊ1ms�ж�һ��
//	if(SysTick_Config(SystemCoreClock / 1000)){
//		// �����쳣
//		while(1);
//	}
   // �ر���શ�ʱ��
//   SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//	��������ʱ����
//	˵������ SysTick �жϷ������ﱻ����
//
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0)
	{ 
		TimingDelay--;
	}
}

//	������������ʱ
// ������nTime - ��ʱʱ�䣬��λms
//	˵����ÿ�ε��ö������¸�TimingDelay��ֵ��ʵ�� n �������ʱ�������ʱ 4294967295 ms��	
//
void Delay_ms(uint32_t nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}
// �ȴ���ʱ���
//void SysTick_Wait(){
//	// ��ʱ��ʹ��
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
//    // �ȴ�ֱ����ʱ����Ϊ0
//    while (sysTickCnt > 0);
//	// ��ʱ��ʧ��
//	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//}
// ��ʱus
//void SysTick_DelayUs(__IO u32 nTime){
//    // ����ʱ���ж�Ϊus��
//    SysTick_Config(SystemCoreClock / 1000000);
//    sysTickCnt = nTime;
//    // �ȴ���ʱ���
//    SysTick_Wait();
//    // ��������ϵͳ�ж�Ϊms��
//    SysTick_Config(SystemCoreClock / 1000);
//	// ��ʱ��ʧ��
//	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//}
// ��ʱms
void SysTick_DelayMs(__IO u32 nTime){
//    sysTickCnt = nTime;
//    SysTick_Wait();
  	TimingDelay = nTime;
	while(TimingDelay != 0);
}
// ��ʱs
void SysTick_DelayS(__IO u32 nTime){
    SysTick_DelayMs(nTime * 1000);
}
// ���õ���ʱ(������ʽ)
void SysTick_CountdownBegin(__IO u32 nTime){
    // ��������Ϊ1ms�ж�һ��
	TimingDelay = nTime;
	// ��ʱ��ʹ��
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// ��������ʱ
void SysTick_CountdownCancel(void){
    // ������઼�ʱ���ļ���ֵ
    TimingDelay = 0;
	// systick ��ʱ��ʧ��
//	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	
}

// �жϵ���ʱ�Ƿ�ʱ
uint8_t SysTick_CountdownIsTimeout(void){
    return TimingDelay == 0;
}
// ����ϵͳ��ʱ���жϵĻص�����
void SysTick_Handler(void)
{
//	if(sysTickCnt > 0){
//        sysTickCnt--;
//    }else{
//		sysTickCnt = 0;
//	}
  TimingDelay--;
  f_get_timebase_flag();
}
