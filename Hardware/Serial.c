#include "stm32f4xx.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "Ano.h"
#include "delay.h"
#include "Serial.h"
#include "fashion_star_uart_servo.h"

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;


uint8_t usart1SendBuf[USART_SEND_BUF_SIZE + 1];
uint8_t usart1RecvBuf[USART_RECV_BUF_SIZE + 1];
RingBufferTypeDef usart1SendRingBuf;
RingBufferTypeDef usart1RecvRingBuf;
Usart_DataTypeDef usart1;
Usart_DataTypeDef *servo_usart = &usart1; // �������߶����Ӧ��USART

void Serial_Init(void)//Uart3
{
	
	// ��ֵ�ṹ��usartָ��
    usart1.pUSARTx = USART1;
    // ��ʼ��������(���ζ���)
    RingBuffer_Init(&usart1SendRingBuf, USART_SEND_BUF_SIZE, usart1SendBuf);
    RingBuffer_Init(&usart1RecvRingBuf, USART_RECV_BUF_SIZE, usart1RecvBuf);
    usart1.recvBuf = &usart1RecvRingBuf;
    usart1.sendBuf = &usart1SendRingBuf;
	
	//����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
	
			//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
//Tx
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10;;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//Rx
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;//��У��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ�ֳ�
	USART_Init(USART1, &USART_InitStructure);
	
	//����RXNE��־λ��NVIC�����  ����Ϊ�����жϣ���ʾ�����ݹ�����CPUҪ�жϽ��н��գ�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}
/* ���͵����ֽ� */
void Serial_SendByte(USART_TypeDef *pUSARTx,uint8_t Byte)
{
	USART_SendData(pUSARTx, Byte);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}
/* ����8λ���ֽ��� */
void Serial_SendArray(USART_TypeDef *pUSARTx,uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(pUSARTx,Array[i]);
	}
}
/* �����ַ��� */
void Serial_SendString(USART_TypeDef *pUSARTx,char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(pUSARTx,String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

//void Serial_SendNumber(uint32_t Number, uint8_t Length)
//{
//	uint8_t i;
//	for (i = 0; i < Length; i ++)
//	{
//		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
//	}
//}

//int fputc(int ch, FILE *f)
//{
//	Serial_SendByte(ch);
//	return ch;
//}

//void Serial_Printf(char *format, ...)
//{
//	char String[100];
//	va_list arg;
//	va_start(arg, format);
//	vsprintf(String, format, arg);
//	va_end(arg);
//	Serial_SendString(String);
//}

uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}
// ���崮���жϴ�����
void USART1_IRQHandler(void)
{
	uint8_t ucTemp;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		// �Ӵ��ڶ�ȡһ���ַ�
		//Serial_RxData = USART_ReceiveData(USART1);
		ucTemp = USART_ReceiveData(USART1);
		// �µ��ַ���ӵ����ڵĻ��λ��������
    RingBuffer_Push(usart1.recvBuf, ucTemp);
//		Serial_RxFlag = 1;
//    Ano_GetByte(Serial_RxData);
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
// �����ڷ��ͻ�����������ȫ������ȥ
void Usart_SendAll(Usart_DataTypeDef *usart)
{
    uint8_t value;
    while (RingBuffer_GetByteUsed(usart->sendBuf))
    {
        value = RingBuffer_Pop(usart->sendBuf);
        // printf("Usart_SendAll pop: %d", value);
        Serial_SendByte(usart->pUSARTx, value);
    }
}
//// ���������صĲ���
// ʱ����ms
// ���Գ����޸����ø�С��ʱ����������500ms
uint16_t interval = 800;
// ���ִ�й��� mV Ĭ��Ϊ0
uint16_t power = 0;
// ���ö���Ƕȵ�ʱ��, �Ƿ�Ϊ����ʽ
// 0:���ȴ� 1:�ȴ������ת���ض���λ��;
uint8_t wait = 0;
// �Ƿ�Ϊ��Ȧģʽ
// 0: ��Ȧģʽ; 1: ��Ȧģʽ;
uint8_t is_mturn = 0;
//��ʼ״̬
void InitArm()
{
  //��ʻ��ʼ״̬
    FSUS_SetServoAngle(servo_usart, 1, -90, interval, power, wait);
//    FSUS_Wait(servo_usart, 1, -90.0, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 2, 55, interval, power, wait);//20
//    FSUS_Wait(servo_usart, 2, 20, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 3, 100, interval, power, wait);//-80
//    FSUS_Wait(servo_usart, 3, -80, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 4, 80, interval, power, wait);//0
//    FSUS_Wait(servo_usart,  4, 0, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 5, 20, interval, power, wait);//0
//    FSUS_Wait(servo_usart,  5, 0, is_mturn);
    SysTick_DelayMs(interval);
}
//ȡ��
void PickUp()
{
  //ȡ���壺
    FSUS_SetServoAngle(servo_usart, 1, 0, interval, power, wait);
//    FSUS_Wait(servo_usart, 1, 0.0, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 2, 23, interval, power, wait);//70
//    FSUS_Wait(servo_usart, 2, 70, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 3, -20, 1100, power, wait);//-80
//    FSUS_Wait(servo_usart, 3, -80, is_mturn);
    SysTick_DelayMs(1100);
    FSUS_SetServoAngle(servo_usart, 4, -45, interval, power, wait);//45
//    FSUS_Wait(servo_usart,  4, 60, is_mturn);
    SysTick_DelayMs(interval);
	    FSUS_SetServoAngle(servo_usart, 4, -60, interval, power, wait);//45
//    FSUS_Wait(servo_usart,  4, 60, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 5, 8, interval, power, wait);//-30
//    FSUS_Wait(servo_usart,  5, -30, is_mturn);
    SysTick_DelayMs(interval);
	    FSUS_SetServoAngle(servo_usart, 5, 22, interval, power, wait);//-30
//    FSUS_Wait(servo_usart,  5, -30, is_mturn);
    SysTick_DelayMs(interval);
		    FSUS_SetServoAngle(servo_usart, 5, 8, interval, power, wait);//-30
//    FSUS_Wait(servo_usart,  5, -30, is_mturn);
    SysTick_DelayMs(interval);
}



void modeyidong()
{

}
//���
void Stock()
{
	    FSUS_SetServoAngle(servo_usart, 2, 0, interval, power, wait);//20
//    FSUS_Wait(servo_usart, 2, 20, is_mturn);
    SysTick_DelayMs(interval);
		FSUS_SetServoAngle(servo_usart, 1, 90, interval, power, wait);
//    FSUS_Wait(servo_usart, 1, -90.0, is_mturn);
    SysTick_DelayMs(interval);

    FSUS_SetServoAngle(servo_usart, 3, 100, interval, power, wait);//-80
//    FSUS_Wait(servo_usart, 3, -80, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 4, 90, interval, power, wait);//0
//    FSUS_Wait(servo_usart,  4, 0, is_mturn);
    SysTick_DelayMs(interval);
	    FSUS_SetServoAngle(servo_usart, 5, 8, interval, power, wait);//-30
//    FSUS_Wait(servo_usart,  5, -30, is_mturn);
    SysTick_DelayMs(interval);
}

//ж��
void PutDown()
{
  //ȡ���壺
    FSUS_SetServoAngle(servo_usart, 1, 0, interval, power, wait);
//    FSUS_Wait(servo_usart, 1, 0.0, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 2, 23, interval, power, wait);//70
//    FSUS_Wait(servo_usart, 2, 70, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 3, -20, 1100, power, wait);//-80
//    FSUS_Wait(servo_usart, 3, -80, is_mturn);
    SysTick_DelayMs(1100);
    FSUS_SetServoAngle(servo_usart, 4, -45, interval, power, wait);//45
//    FSUS_Wait(servo_usart,  4, 60, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 5, 20, interval, power, wait);//-30
//    FSUS_Wait(servo_usart,  5, -30, is_mturn);
    SysTick_DelayMs(interval);
  
}

void Recover()
{
	  FSUS_SetServoAngle(servo_usart, 2, 90, interval, power, wait);//20
//    FSUS_Wait(servo_usart, 2, 20, is_mturn);
    SysTick_DelayMs(interval);
  //��ʻ��ʼ״̬
    FSUS_SetServoAngle(servo_usart, 1, 0, interval, power, wait);
//    FSUS_Wait(servo_usart, 1, -90.0, is_mturn);
    SysTick_DelayMs(interval);

    FSUS_SetServoAngle(servo_usart, 3, 100, interval, power, wait);//-80
//    FSUS_Wait(servo_usart, 3, -80, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 4, 90, interval, power, wait);//0
//    FSUS_Wait(servo_usart,  4, 0, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 5, 20, interval, power, wait);//0
//    FSUS_Wait(servo_usart,  5, 0, is_mturn);
    SysTick_DelayMs(interval);
}
/* ���ƶ������ĽǶ� */
void FSUSExample_SetNServoAngle(void)
{
	while (1)
	{
		// ���ƶ����̨�Ƕ�
//		FSUS_SetServoAngle(servo_usart, 0, 90.0, interval, power, wait);
//		FSUS_SetServoAngle(servo_usart, 1, 45.0, interval, power, wait);
//		// ����ʽ�ȴ�, �ȴ���ת��Ŀ��Ƕ�
//		// ע��Ҫ���趨ֵ��ͬ
//		FSUS_Wait(servo_usart, 0, 90.0, is_mturn);
//		FSUS_Wait(servo_usart, 1, 45.0, is_mturn);

//		// �ȴ�2s
//		SysTick_DelayMs(2000);

//		// ���ƶ����ת������һ���Ƕ�
//		FSUS_SetServoAngle(servo_usart, 0, -90.0, interval, power, wait);
//		FSUS_SetServoAngle(servo_usart, 1, -45.0, interval, power, wait);
//		// ����ʽ�ȴ�, �ȴ���ת��Ŀ��Ƕ�
//		// ע��Ҫ���趨ֵ��ͬ
//		FSUS_Wait(servo_usart, 0, -90.0, is_mturn);
//		FSUS_Wait(servo_usart, 1, -45.0, is_mturn);

//		// �ȴ�2s
//		SysTick_DelayMs(2000);
//     //��̨�м�0��
//		FSUS_SetServoAngle(servo_usart, 1, 90.0, interval, power, wait);
//		//FSUS_Wait(servo_usart, 1, 0.0, is_mturn);
//		SysTick_DelayMs(interval);
//		//
//		FSUS_SetServoAngle(servo_usart, 2, 0.0, interval, power, wait);
//		//FSUS_Wait(servo_usart, 2, -50.0, is_mturn);
//		SysTick_DelayMs(interval);
//		
//		FSUS_SetServoAngle(servo_usart, 3, 0.0, interval, power, wait);
//		//FSUS_Wait(servo_usart, 3, 30.0, is_mturn);
//		SysTick_DelayMs(interval);
//		
//		FSUS_SetServoAngle(servo_usart, 4, 0.0, interval, power, wait);
//		//FSUS_Wait(servo_usart, 4, 30.0, is_mturn);
//		SysTick_DelayMs(interval);
//		
//		FSUS_SetServoAngle(servo_usart, 5, 0.0, interval, power, wait);
//		//FSUS_Wait(servo_usart, 4, 30.0, is_mturn);
//		SysTick_DelayMs(interval);
//		
//		//��̨�м�0��
//		FSUS_SetServoAngle(servo_usart, 1, 0.0, interval, power, wait);
//		//FSUS_Wait(servo_usart, 1, 0.0, is_mturn);
//		SysTick_DelayMs(interval);
//		//
//		FSUS_SetServoAngle(servo_usart, 2, -30.0, interval, power, wait);
//		//FSUS_Wait(servo_usart, 2, -50.0, is_mturn);
//		SysTick_DelayMs(interval);
//		
//		FSUS_SetServoAngle(servo_usart, 3, 100.0, interval, power, wait);
//		//FSUS_Wait(servo_usart, 3, 30.0, is_mturn);
//		SysTick_DelayMs(interval);
//		
//		FSUS_SetServoAngle(servo_usart, 4, -40.0, interval, power, wait);
//		//FSUS_Wait(servo_usart, 4, 30.0, is_mturn);
//		SysTick_DelayMs(interval);
//		
//		FSUS_SetServoAngle(servo_usart, 5, 0.0, interval, power, wait);
//		//FSUS_Wait(servo_usart, 4, 30.0, is_mturn);
//		SysTick_DelayMs(interval);
//		
//		FSUS_SetServoAngle(servo_usart, 5, -30.0, interval, power, wait);
//		//FSUS_Wait(servo_usart, 4, 30.0, is_mturn);
//		SysTick_DelayMs(interval);
    
    //FSUS_SetServoAngle(servo_usart, 1, 91.0, interval, power, wait);
		// ����ʽ�ȴ�, �ȴ���ת��Ŀ��Ƕ�
		//FSUS_Wait(servo_usart, 1, 90.0, is_mturn);//
		//SysTick_DelayMs(interval);
		
    //��ʻ��ʼ״̬
    FSUS_SetServoAngle(servo_usart, 1, -90, interval, power, wait);
//    FSUS_Wait(servo_usart, 1, -90.0, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 2, 20, interval, power, wait);
//    FSUS_Wait(servo_usart, 2, 20, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 3, -80, interval, power, wait);
//    FSUS_Wait(servo_usart, 3, -80, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 4, 0, interval, power, wait);
//    FSUS_Wait(servo_usart,  4, 0, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 5, 0, interval, power, wait);
//    FSUS_Wait(servo_usart,  5, 0, is_mturn);
    SysTick_DelayMs(interval);
    //ȡ���壺
    FSUS_SetServoAngle(servo_usart, 1, 0, interval, power, wait);
//    FSUS_Wait(servo_usart, 1, 0.0, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 2, 70, interval, power, wait);
//    FSUS_Wait(servo_usart, 2, 70, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 3, -80, interval, power, wait);
//    FSUS_Wait(servo_usart, 3, -80, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 4, 60, interval, power, wait);
//    FSUS_Wait(servo_usart,  4, 60, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 5, -30, interval, power, wait);
//    FSUS_Wait(servo_usart,  5, -30, is_mturn);
    SysTick_DelayMs(interval);
   //�����壺
       FSUS_SetServoAngle(servo_usart, 1, -90, interval, power, wait);
//    FSUS_Wait(servo_usart, 1, 0.0, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 2, 40, interval, power, wait);
//    FSUS_Wait(servo_usart, 2, 70, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 3, -120, interval, power, wait);
//    FSUS_Wait(servo_usart, 3, -80, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 4, 70, interval, power, wait);
//    FSUS_Wait(servo_usart,  4, 60, is_mturn);
    SysTick_DelayMs(interval);
    FSUS_SetServoAngle(servo_usart, 5, 0, interval, power, wait);
//    FSUS_Wait(servo_usart,  5, -30, is_mturn);
    SysTick_DelayMs(interval);
	}
}
