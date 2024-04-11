/***
	***************************************************************************
	*	@file  	delay.c
	*	@brief   delay接口相关函数
   ***************************************************************************
   *  @description
	*
	*  SysTick定时器配置为1ms中断，实现毫秒延时
	* 	
	***************************************************************************
***/

#include "delay.h"
#include "GlobalFunction.h"

static __IO uint32_t TimingDelay;  //计数变量
static __IO u32 sysTickCnt;

//	函数：延时初始化
//	说明：配置 SysTick 为1ms中断，并启动定时器
//
void SysTick_Init(void)
{
	SysTick_Config(SystemCoreClock / 1000);  //配置SysTick时钟为1ms中断

	 // 这里设置为1ms中断一次
//	if(SysTick_Config(SystemCoreClock / 1000)){
//		// 捕获异常
//		while(1);
//	}
   // 关闭嘀嗒定时器
//   SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//	函数：计时函数
//	说明：在 SysTick 中断服务函数里被调用
//
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0)
	{ 
		TimingDelay--;
	}
}

//	函数：毫秒延时
// 参数：nTime - 延时时间，单位ms
//	说明：每次调用都会重新给TimingDelay赋值，实现 n 毫秒的延时，最大延时 4294967295 ms。	
//
void Delay_ms(uint32_t nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}
// 等待计时完成
//void SysTick_Wait(){
//	// 定时器使能
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
//    // 等待直到计时器变为0
//    while (sysTickCnt > 0);
//	// 定时器失能
//	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//}
// 延时us
//void SysTick_DelayUs(__IO u32 nTime){
//    // 设置时钟中断为us级
//    SysTick_Config(SystemCoreClock / 1000000);
//    sysTickCnt = nTime;
//    // 等待计时完成
//    SysTick_Wait();
//    // 重新设置系统中断为ms级
//    SysTick_Config(SystemCoreClock / 1000);
//	// 定时器失能
//	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//}
// 延时ms
void SysTick_DelayMs(__IO u32 nTime){
//    sysTickCnt = nTime;
//    SysTick_Wait();
  	TimingDelay = nTime;
	while(TimingDelay != 0);
}
// 延时s
void SysTick_DelayS(__IO u32 nTime){
    SysTick_DelayMs(nTime * 1000);
}
// 设置倒计时(非阻塞式)
void SysTick_CountdownBegin(__IO u32 nTime){
    // 这里设置为1ms中断一次
	TimingDelay = nTime;
	// 定时器使能
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// 撤销倒计时
void SysTick_CountdownCancel(void){
    // 重置嘀嗒计时器的计数值
    TimingDelay = 0;
	// systick 定时器失能
//	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	
}

// 判断倒计时是否超时
uint8_t SysTick_CountdownIsTimeout(void){
    return TimingDelay == 0;
}
// 设置系统定时器中断的回调函数
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
