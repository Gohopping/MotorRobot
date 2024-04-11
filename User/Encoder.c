#include "stm32f4xx.h"                  // Device header

#define ENCODER_TIM_PSC  0          /*��������Ƶ*/
#define ENCODER_TIM_PERIOD  65535   /*���������ֵ*/
#define CNT_INIT 0                  /*��������ֵ*/


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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʹ��TIM4ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIODʱ��
	//PD12 PD13���ó���������ģʽ
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�ٶ�100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//��PD12��PD13��������ΪTIM4 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	//�ڲ�ʱ��ѡ��ɺ���
	
	//��ʱ�� ʱ����Ԫ����
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInit(TIM4 , &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;/*�������ϼ���ģʽ����Ч�� ���������й�*/ 
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR �Զ���װֵ �����̼��� ������Χ�󣬷���ת��Ϊ����
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC  Ԥ��Ƶ ����Ƶ
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4 , &TIM_TimeBaseInitStructure);

	
  /*���벶������*/
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);  //�ṹ�帳��ֵ
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	  /*������ģʽ���ã�ͬʱ����ͨ��1��ͨ��2(��4��Ƶ)�����Ծ�ΪRising*/
	//��ʱ���������ӿ�����  ��������ʱ����������ģʽ��ͨ��1��2�ĵ�ƽ����
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); 	//TIM_EncoderMode_TI12 TI1\TI2������ Rising ͨ��������
	TIM_SetCounter(TIM4, 0);      /*CNT���ֵ*/
//	TIM_ClearFlag(TIM4,TIM_IT_Update);   /*�жϱ�־��0*/
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); /*�ж�ʹ��*/
	TIM_Cmd(TIM4, ENABLE);
}
void TIM3_Encoder_Init(void)  //Moter_B
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ��TIM3ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

	//PD12 PD13���ó���������ģʽ
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //���츴����� 
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//��PA6��PA7��������ΪTIM3 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);//��PA15��������ΪTIM2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);//��PB3��������ΪTIM2
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;/*�������ϼ���ģʽ*/ 
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR �Զ���װֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC  Ԥ��Ƶ
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3 , &TIM_TimeBaseInitStructure);
	
	/*������ģʽ���ã�ͬʱ����ͨ��1��ͨ��2(��4��Ƶ)�����Ծ�ΪRising*/
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	//�������ӿ�����
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_SetCounter(TIM3, 0);      /*CNT���ֵ*/
	TIM_ClearFlag(TIM3,TIM_IT_Update);   /*�жϱ�־��0*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); /*�ж�ʹ��*/
	TIM_Cmd(TIM3, ENABLE);  
}
/**
* @brief TIM4 ͨ��1ͨ��2 ����������
* @param none
*/
void TIM2_Encoder_Init(void)                      
{ 
	GPIO_InitTypeDef GPIO_InitStruct;            /*GPIO*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct; /*ʱ��*/
	TIM_ICInitTypeDef TIM_ICInitStruct;          /*����ͨ��*/
    
    /*GPIO��ʼ��*/    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); /*ʹ��GPIOʱ�� AHB1*/    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); /*ʹ��GPIOʱ�� AHB1*/ 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   /*ʹ�ܶ�ʱ��ʱ�� APB1*/
  
	GPIO_StructInit(&GPIO_InitStruct);        
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15; 
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;        /*���ù���*/
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	 /*�ٶ�100MHz*/
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;   
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;        
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);//��PA15��������ΪTIM2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3 ,GPIO_AF_TIM2);//��PB3��������ΪTIM2

	/*ʱ����ʼ��*/
	TIM_DeInit(TIM2);  
	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
	TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC;       /*Ԥ��Ƶ */        
	TIM_TimeBaseStruct.TIM_Period = 0xffff;       /*����(��װ��ֵ)*/
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM_CounterMode_Up;  /*�������ϼ���ģʽ*/  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct); 

//	/*������ģʽ���ã�ͬʱ����ͨ��1��ͨ��2(��4��Ƶ)�����Ծ�ΪRising*/
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	TIM_ICStructInit(&TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;
//	TIM_ICInit(TIM2, &TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
//	TIM_ICInitStructure.TIM_ICFilter = 0xF;
//	TIM_ICInit(TIM2, &TIM_ICInitStructure);
		
	/*������ģʽ���ã�ͬʱ����ͨ��1��ͨ��2(��4��Ƶ)�����Ծ�ΪRising*/
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Falling, TIM_ICPolarity_Falling); 
	
	TIM_ICStructInit(&TIM_ICInitStruct);        
	TIM_ICInitStruct.TIM_ICFilter = 0xF;   /*����ͨ�����˲�����*/
	TIM_ICInit(TIM2, &TIM_ICInitStruct); /*����ͨ����ʼ��*/

	TIM_SetCounter(TIM2, CNT_INIT);      /*CNT���ֵ*/
	TIM_ClearFlag(TIM2,TIM_IT_Update);   /*�жϱ�־��0*/
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); /*�ж�ʹ��*/
	TIM_Cmd(TIM2,ENABLE);                /*ʹ��CR�Ĵ���*/
} 
void TIM1_Encoder_Init(void)  //Moter_D
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOEʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  //ʹ�ܶ�ʱ��1��ʱ��

		GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11,GPIO_AF_TIM1);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//��߷�Ӧ�ٶ�Ϊ100MHz
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		    //���Ÿ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   	  //���츴����� 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		    //����
		GPIO_Init(GPIOE, &GPIO_InitStructure);

		TIM_DeInit(TIM1);
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;/*�������ϼ���ģʽ*/ 
		TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR �Զ���װֵ
		TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC  Ԥ��Ƶ
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM1 , &TIM_TimeBaseInitStructure);

		/*������ģʽ���ã�ͬʱ����ͨ��1��ͨ��2(��4��Ƶ)�����Ծ�ΪRising*/
//		TIM_ICInitTypeDef TIM_ICInitStructure;
//		TIM_ICStructInit(&TIM_ICInitStructure);
//		TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
//		TIM_ICInitStructure.TIM_ICFilter = 0xF;
//		TIM_ICInit(TIM1, &TIM_ICInitStructure);
//		TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
//		TIM_ICInitStructure.TIM_ICFilter = 0xF;
//		TIM_ICInit(TIM1, &TIM_ICInitStructure);
	
		//�������ӿ�����
		TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
		
		/*������ģʽ���ã�ͬʱ����ͨ��1��ͨ��2(��4��Ƶ)�����Ծ�ΪRising*/
		TIM_ICInitTypeDef TIM_ICInitStructure;
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 0xF;
		TIM_ICInit(TIM1, &TIM_ICInitStructure);

		TIM_ClearFlag(TIM1,TIM_IT_Update);   /*�жϱ�־��0*/
		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); /*�ж�ʹ��*/
		
		TIM_SetCounter(TIM1, 0);      /*CNT���ֵ*/
		TIM_Cmd(TIM1, ENABLE);
}
int TIM4_Encoder_Get(void)
{
	  int16_t Temp;
    Temp=TIM_GetCounter(TIM4);
    TIM_SetCounter(TIM4,0);
    return Temp;
//	int encoderNum = 0;
//	encoderNum = (int)((int16_t)(TIM4->CNT)); /*CNTΪuint32, תΪint16*/
//	TIM_SetCounter(TIM4, CNT_INIT);/*CNT���ֵ*/
//	return encoderNum;
}

int TIM3_Encoder_Get(void)
{
  int16_t encoderNum = 0;
//	encoderNum = TIM_GetCounter(TIM3); 
//	TIM_SetCounter(TIM3, CNT_INIT);/*CNT���ֵ*/
//	return encoderNum;
	encoderNum=TIM3->CNT; //��ȡ����
	encoderNum=(int)((int16_t)(TIM3->CNT));
	TIM3->CNT=0; //��ȡ����������
	return encoderNum;
}

int TIM1_Encoder_Get(void)
{
  int16_t Encoder_TIM;
  Encoder_TIM =	TIM_GetCounter(TIM1); 
  //Encoder_TIM=(int)((int16_t)(TIM1->CNT));
  TIM_SetCounter(TIM1, CNT_INIT);/*CNT���ֵ*/
  return Encoder_TIM; //����ֵ
}

int TIM2_Encoder_Get(void)
{
	int16_t Encoder_TIM;
  Encoder_TIM =	TIM_GetCounter(TIM2); 
  TIM_SetCounter(TIM2, CNT_INIT);/*CNT���ֵ*/
  return Encoder_TIM; //����ֵ
}
/**************************************************************************
��    ��: ����ת����Ӧ������������
��    ��: num��ת����ppr����������ratio�����ٱ�
�� �� ֵ: ��������� 
**************************************************************************/
long Num_Encoder_Cnt(float num,uint16_t ppr,float ratio)
{
	  int PulseNumber = 0;
	  PulseNumber = num*ratio*ppr*4;
    return PulseNumber;                                 
}

/**************************************************************************
��    ��: ����ת�ٶ�Ӧ������������
��    ��: rpm��ת�٣�ppr����������ratio�����ٱȣ�cnt_time������ʱ��(ms)
�� �� ֵ: ��������� 
**************************************************************************/
long Rpm_Encoder_Cnt(float rpm,uint16_t ppr,uint16_t ratio,uint16_t cnt_time)
{
    return (rpm*ratio*ppr*4)/(60*1000/cnt_time);               
}

