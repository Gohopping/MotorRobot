#include "stm32f4xx.h"                  // Device header

//通用定时器7初始化
//定时器溢出时间计算方法:Tout=(((TIM_Period+1)*(TIM_Prescaler+1))/SystemCoreClock)us.
//SystemCoreClock=定时器工作频率,单位:Mhz
void TIM5_PWM_Init(uint16_t arr,uint16_t psc)
{
	  //第一步，RCC开启时钟，把我们要用的TIM外设和GPIO外设的时钟打开
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	  //第二步，配置GPIO
	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM5);
	
	  //第三步,时钟源选择(内部时钟)
	  TIM_InternalClockConfig(TIM5);
	  //第四步，配置时基单元
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//定时器分频参数,DIV1为1分频，即频率不变，DIV2为2分频，DIV4为4分频
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数 TIM_CounterMode_Up为向上计数，TIM_CounterMode_Down为向下计数，TIM_CounterMode_CenterAligned为中央对齐计数的三种模式
		//周期时间 = (PSC+1)*(ARR+1)/时钟频率  = 1ms    1Khz--->1ms
		TIM_TimeBaseInitStructure.TIM_Period = arr;		//自动重装载寄存器ARR
		TIM_TimeBaseInitStructure.TIM_Prescaler = psc;		//预分频器PSC
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //重复计数器的值
		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);   	//初始化定时器参数,包含自动重装值，分频系数，计数方式等
		//第五步,配置输出比较单元
		TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_OCStructInit(&TIM_OCInitStructure);//给输出比较结构体赋一个默认的值
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择PWM1的模式 PWM 模式1——在向上计数时，一旦TIMx_CNT<TIMx_CCR1时通道1为有效电平
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性 有效电平为高电平
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出使能
		//通道1
		TIM_OCInitStructure.TIM_Pulse = 0;		    //CCR
		TIM_OC1Init(TIM5,&TIM_OCInitStructure);   //初始化PWM输出参数，包含PWM模式、输出极性，使能
		//通道2
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC2Init(TIM5,&TIM_OCInitStructure);
		//通道3
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC3Init(TIM5,&TIM_OCInitStructure);
		 //通道4
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC4Init(TIM5,&TIM_OCInitStructure);
		//第六步,启动定时器
		TIM_Cmd(TIM5, ENABLE);
		TIM_CtrlPWMOutputs(TIM5,ENABLE);
		
		//初始化pwm占空比为0，防止电机上电就转
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);
    GPIO_ResetBits(GPIOA,GPIO_Pin_2);
    GPIO_ResetBits(GPIOA,GPIO_Pin_3);
}

void PWM_SetCompare1(uint16_t Compare)
{
	//CH1通道 更改CCR寄存器值的函数,更改占空
	TIM_SetCompare1(TIM5, Compare);
}

void PWM_SetCompare2(uint16_t Compare)
{
	//CH2通道 更改CCR寄存器值的函数,更改占空
	TIM_SetCompare2(TIM5, Compare);
}

void PWM_SetCompare3(uint16_t Compare)
{
	//CH3通道 更改CCR寄存器值的函数,更改占空
	TIM_SetCompare3(TIM5, Compare);
}

void PWM_SetCompare4(uint16_t Compare)
{
	//CH3通道 更改CCR寄存器值的函数,更改占空
	TIM_SetCompare4(TIM5, Compare);
}

