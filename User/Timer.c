#include "stm32f4xx.h"                  // Device header

//ͨ�ö�ʱ��7��ʼ��
//��ʱ�����ʱ����㷽��:Tout=(((TIM_Period+1)*(TIM_Prescaler+1))/SystemCoreClock)us.
//SystemCoreClock=��ʱ������Ƶ��,��λ:Mhz
void TIM5_PWM_Init(uint16_t arr,uint16_t psc)
{
	  //��һ����RCC����ʱ�ӣ�������Ҫ�õ�TIM�����GPIO�����ʱ�Ӵ�
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	  //�ڶ���������GPIO
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
	
	  //������,ʱ��Դѡ��(�ڲ�ʱ��)
	  TIM_InternalClockConfig(TIM5);
	  //���Ĳ�������ʱ����Ԫ
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//��ʱ����Ƶ����,DIV1Ϊ1��Ƶ����Ƶ�ʲ��䣬DIV2Ϊ2��Ƶ��DIV4Ϊ4��Ƶ
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ��� TIM_CounterMode_UpΪ���ϼ�����TIM_CounterMode_DownΪ���¼�����TIM_CounterMode_CenterAlignedΪ����������������ģʽ
		//����ʱ�� = (PSC+1)*(ARR+1)/ʱ��Ƶ��  = 1ms    1Khz--->1ms
		TIM_TimeBaseInitStructure.TIM_Period = arr;		//�Զ���װ�ؼĴ���ARR
		TIM_TimeBaseInitStructure.TIM_Prescaler = psc;		//Ԥ��Ƶ��PSC
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; //�ظ���������ֵ
		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);   	//��ʼ����ʱ������,�����Զ���װֵ����Ƶϵ����������ʽ��
		//���岽,��������Ƚϵ�Ԫ
		TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_OCStructInit(&TIM_OCInitStructure);//������ȽϽṹ�帳һ��Ĭ�ϵ�ֵ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��PWM1��ģʽ PWM ģʽ1���������ϼ���ʱ��һ��TIMx_CNT<TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������� ��Ч��ƽΪ�ߵ�ƽ
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//���ʹ��
		//ͨ��1
		TIM_OCInitStructure.TIM_Pulse = 0;		    //CCR
		TIM_OC1Init(TIM5,&TIM_OCInitStructure);   //��ʼ��PWM�������������PWMģʽ��������ԣ�ʹ��
		//ͨ��2
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC2Init(TIM5,&TIM_OCInitStructure);
		//ͨ��3
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC3Init(TIM5,&TIM_OCInitStructure);
		 //ͨ��4
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC4Init(TIM5,&TIM_OCInitStructure);
		//������,������ʱ��
		TIM_Cmd(TIM5, ENABLE);
		TIM_CtrlPWMOutputs(TIM5,ENABLE);
		
		//��ʼ��pwmռ�ձ�Ϊ0����ֹ����ϵ��ת
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);
    GPIO_ResetBits(GPIOA,GPIO_Pin_2);
    GPIO_ResetBits(GPIOA,GPIO_Pin_3);
}

void PWM_SetCompare1(uint16_t Compare)
{
	//CH1ͨ�� ����CCR�Ĵ���ֵ�ĺ���,����ռ��
	TIM_SetCompare1(TIM5, Compare);
}

void PWM_SetCompare2(uint16_t Compare)
{
	//CH2ͨ�� ����CCR�Ĵ���ֵ�ĺ���,����ռ��
	TIM_SetCompare2(TIM5, Compare);
}

void PWM_SetCompare3(uint16_t Compare)
{
	//CH3ͨ�� ����CCR�Ĵ���ֵ�ĺ���,����ռ��
	TIM_SetCompare3(TIM5, Compare);
}

void PWM_SetCompare4(uint16_t Compare)
{
	//CH3ͨ�� ����CCR�Ĵ���ֵ�ĺ���,����ռ��
	TIM_SetCompare4(TIM5, Compare);
}

