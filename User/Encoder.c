#include "stm32f4xx.h"                  // Device header

#define ENCODER_TIM_PSC  0          /*计数器分频*/
#define ENCODER_TIM_PERIOD  65535   /*计数器最大值*/
#define CNT_INIT 0                  /*计数器初值*/


void  TIM1_Encoder_Init();
void  TIM2_Encoder_Init();
void  TIM4_Encoder_Init();
void  TIM3_Encoder_Init();
  
void TIM_Encoder_Init()
{
  TIM1_Encoder_Init();
  TIM2_Encoder_Init();
  TIM4_Encoder_Init();
  TIM3_Encoder_Init();
}
//TIM4 Moter_A
void TIM4_Encoder_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIM4时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOD时钟
	//PD12 PD13配置成上拉输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//速度100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//将PD12、PD13引脚配置为TIM4 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	//内部时钟选择可忽略
	
	//定时器 时基单元配置
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInit(TIM4 , &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;/*连续向上计数模式（无效） 被编码器托管*/ 
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR 自动重装值 满量程计数 计数范围大，方便转换为负数
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC  预分频 不分频
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4 , &TIM_TimeBaseInitStructure);

	
  /*输入捕获配置*/
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);  //结构体赋初值
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	  /*编码器模式配置：同时捕获通道1与通道2(即4倍频)，极性均为Rising*/
	//定时器编码器接口配置  参数：定时器、编码器模式、通道1、2的电平极性
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); 	//TIM_EncoderMode_TI12 TI1\TI2都计数 Rising 通道不反相
	TIM_SetCounter(TIM4, 0);      /*CNT设初值*/
//	TIM_ClearFlag(TIM4,TIM_IT_Update);   /*中断标志清0*/
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); /*中断使能*/
	TIM_Cmd(TIM4, ENABLE);
}
void TIM3_Encoder_Init(void)  //Moter_B
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //使能TIM3时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

	//PD12 PD13配置成上拉输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽复用输出 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//将PA6、PA7引脚配置为TIM3 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);//将PA15引脚配置为TIM2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);//将PB3引脚配置为TIM2
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;/*连续向上计数模式*/ 
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR 自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC  预分频
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3 , &TIM_TimeBaseInitStructure);
	
	/*编码器模式配置：同时捕获通道1与通道2(即4倍频)，极性均为Rising*/
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	//编码器接口配置
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_SetCounter(TIM3, 0);      /*CNT设初值*/
	TIM_ClearFlag(TIM3,TIM_IT_Update);   /*中断标志清0*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); /*中断使能*/
	TIM_Cmd(TIM3, ENABLE);  
}
/**
* @brief TIM4 通道1通道2 正交编码器
* @param none
*/
void TIM2_Encoder_Init(void)                      
{ 
	GPIO_InitTypeDef GPIO_InitStruct;            /*GPIO*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct; /*时基*/
	TIM_ICInitTypeDef TIM_ICInitStruct;          /*输入通道*/
    
    /*GPIO初始化*/    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); /*使能GPIO时钟 AHB1*/    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); /*使能GPIO时钟 AHB1*/ 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   /*使能定时器时钟 APB1*/
  
	GPIO_StructInit(&GPIO_InitStruct);        
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;        /*复用功能*/
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	 /*速度100MHz*/
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;   
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;        
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);//将PA15引脚配置为TIM2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3 ,GPIO_AF_TIM2);//将PB3引脚配置为TIM2

	/*时基初始化*/
	TIM_DeInit(TIM2);  
	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
	TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC;       /*预分频 */        
	TIM_TimeBaseStruct.TIM_Period = 0xffff;       /*周期(重装载值)*/
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM_CounterMode_Up;  /*连续向上计数模式*/  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct); 

//	/*编码器模式配置：同时捕获通道1与通道2(即4倍频)，极性均为Rising*/
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	TIM_ICStructInit(&TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;
//	TIM_ICInit(TIM2, &TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;
//	TIM_ICInit(TIM2, &TIM_ICInitStructure);
		
	/*编码器模式配置：同时捕获通道1与通道2(即4倍频)，极性均为Rising*/
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Falling, TIM_ICPolarity_Falling); 
	
	TIM_ICStructInit(&TIM_ICInitStruct);        
	TIM_ICInitStruct.TIM_ICFilter = 0xF;   /*输入通道的滤波参数*/
	TIM_ICInit(TIM2, &TIM_ICInitStruct); /*输入通道初始化*/

	TIM_SetCounter(TIM2, CNT_INIT);      /*CNT设初值*/
	TIM_ClearFlag(TIM2,TIM_IT_Update);   /*中断标志清0*/
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); /*中断使能*/
	TIM_Cmd(TIM2,ENABLE);                /*使能CR寄存器*/
} 
void TIM1_Encoder_Init(void)  //Moter_D
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOE时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  //使能定时器1的时钟

		GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11,GPIO_AF_TIM1);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//最高反应速度为100MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		    //引脚复用模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   	  //推挽复用输出 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		    //上拉
		GPIO_Init(GPIOE, &GPIO_InitStructure);

		TIM_DeInit(TIM1);
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;/*连续向上计数模式*/ 
		TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR 自动重装值
		TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC  预分频
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM1 , &TIM_TimeBaseInitStructure);

		/*编码器模式配置：同时捕获通道1与通道2(即4倍频)，极性均为Rising*/
//		TIM_ICInitTypeDef TIM_ICInitStructure;
//		TIM_ICStructInit(&TIM_ICInitStructure);
//		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
//		TIM_ICInitStructure.TIM_ICFilter = 0xF;
//		TIM_ICInit(TIM1, &TIM_ICInitStructure);
//		TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
//		TIM_ICInitStructure.TIM_ICFilter = 0xF;
//		TIM_ICInit(TIM1, &TIM_ICInitStructure);
	
		//编码器接口配置
		TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
		
		/*编码器模式配置：同时捕获通道1与通道2(即4倍频)，极性均为Rising*/
		TIM_ICInitTypeDef TIM_ICInitStructure;
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 0xF;
		TIM_ICInit(TIM1, &TIM_ICInitStructure);

		TIM_ClearFlag(TIM1,TIM_IT_Update);   /*中断标志清0*/
		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); /*中断使能*/
		
		TIM_SetCounter(TIM1, 0);      /*CNT设初值*/
		TIM_Cmd(TIM1, ENABLE);
}
int TIM4_Encoder_Get(void)
{
	  int16_t Temp;
    Temp=TIM_GetCounter(TIM4);
    TIM_SetCounter(TIM4,0);
    return Temp;
//	int encoderNum = 0;
//	encoderNum = (int)((int16_t)(TIM4->CNT)); /*CNT为uint32, 转为int16*/
//	TIM_SetCounter(TIM4, CNT_INIT);/*CNT设初值*/
//	return encoderNum;
}

int TIM3_Encoder_Get(void)
{
  int16_t encoderNum = 0;
//	encoderNum = TIM_GetCounter(TIM3); 
//	TIM_SetCounter(TIM3, CNT_INIT);/*CNT设初值*/
//	return encoderNum;
	encoderNum=TIM3->CNT; //读取计数
	encoderNum=(int)((int16_t)(TIM3->CNT));
	TIM3->CNT=0; //读取完后计数清零
	return encoderNum;
}

int TIM1_Encoder_Get(void)
{
  int16_t Encoder_TIM;
  Encoder_TIM =	TIM_GetCounter(TIM1); 
  //Encoder_TIM=(int)((int16_t)(TIM1->CNT));
  TIM_SetCounter(TIM1, CNT_INIT);/*CNT设初值*/
  return Encoder_TIM; //返回值
}

int TIM2_Encoder_Get(void)
{
	int16_t Encoder_TIM;
  Encoder_TIM =	TIM_GetCounter(TIM2); 
  TIM_SetCounter(TIM2, CNT_INIT);/*CNT设初值*/
  return Encoder_TIM; //返回值
}
/**************************************************************************
功    能: 计算转数对应编码器脉冲数
输    入: num：转数；ppr：码盘数；ratio：减速比
返 回 值: 电机脉冲数 
**************************************************************************/
long Num_Encoder_Cnt(float num,uint16_t ppr,float ratio)
{
	  int PulseNumber = 0;
	  PulseNumber = num*ratio*ppr*4;
    return PulseNumber;                                 
}

/**************************************************************************
功    能: 计算转速对应编码器脉冲数
输    入: rpm：转速；ppr：码盘数；ratio：减速比；cnt_time：计数时间(ms)
返 回 值: 电机脉冲数 
**************************************************************************/
long Rpm_Encoder_Cnt(float rpm,uint16_t ppr,uint16_t ratio,uint16_t cnt_time)
{
    return (rpm*ratio*ppr*4)/(60*1000/cnt_time);               
}

