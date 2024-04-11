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
Usart_DataTypeDef *servo_usart = &usart1; // 串口总线舵机对应的USART

void Serial_Init(void)//Uart3
{
	
	// 赋值结构体usart指针
    usart1.pUSARTx = USART1;
    // 初始化缓冲区(环形队列)
    RingBuffer_Init(&usart1SendRingBuf, USART_SEND_BUF_SIZE, usart1SendBuf);
    RingBuffer_Init(&usart1RecvRingBuf, USART_RECV_BUF_SIZE, usart1RecvBuf);
    usart1.recvBuf = &usart1RecvRingBuf;
    usart1.sendBuf = &usart1SendRingBuf;
	
	//开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
	
			//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
//Tx
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10;;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//Rx
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;//无校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位字长
	USART_Init(USART1, &USART_InitStructure);
	
	//开启RXNE标志位到NVIC的输出  配置为接收中断（表示有数据过来，CPU要中断进行接收）
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
/* 发送单个字节 */
void Serial_SendByte(USART_TypeDef *pUSARTx,uint8_t Byte)
{
	USART_SendData(pUSARTx, Byte);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}
/* 发送8位的字节流 */
void Serial_SendArray(USART_TypeDef *pUSARTx,uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(pUSARTx,Array[i]);
	}
}
/* 发送字符串 */
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
// 定义串口中断处理函数
void USART1_IRQHandler(void)
{
	uint8_t ucTemp;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		// 从串口读取一个字符
		//Serial_RxData = USART_ReceiveData(USART1);
		ucTemp = USART_ReceiveData(USART1);
		// 新的字符添加到串口的环形缓冲队列中
    RingBuffer_Push(usart1.recvBuf, ucTemp);
//		Serial_RxFlag = 1;
//    Ano_GetByte(Serial_RxData);
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
// 将串口发送缓冲区的内容全部发出去
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
//// 舵机控制相关的参数
// 时间间隔ms
// 可以尝试修改设置更小的时间间隔，例如500ms
uint16_t interval = 800;
// 舵机执行功率 mV 默认为0
uint16_t power = 0;
// 设置舵机角度的时候, 是否为阻塞式
// 0:不等待 1:等待舵机旋转到特定的位置;
uint8_t wait = 0;
// 是否为多圈模式
// 0: 单圈模式; 1: 多圈模式;
uint8_t is_mturn = 0;
//初始状态
void InitArm()
{
  //行驶初始状态
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
//取货
void PickUp()
{
  //取物体：
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
//存货
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

//卸货
void PutDown()
{
  //取物体：
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
  //行驶初始状态
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
/* 控制多个舵机的角度 */
void FSUSExample_SetNServoAngle(void)
{
	while (1)
	{
		// 控制舵机云台角度
//		FSUS_SetServoAngle(servo_usart, 0, 90.0, interval, power, wait);
//		FSUS_SetServoAngle(servo_usart, 1, 45.0, interval, power, wait);
//		// 阻塞式等待, 等待旋转到目标角度
//		// 注意要跟设定值相同
//		FSUS_Wait(servo_usart, 0, 90.0, is_mturn);
//		FSUS_Wait(servo_usart, 1, 45.0, is_mturn);

//		// 等待2s
//		SysTick_DelayMs(2000);

//		// 控制舵机旋转到另外一个角度
//		FSUS_SetServoAngle(servo_usart, 0, -90.0, interval, power, wait);
//		FSUS_SetServoAngle(servo_usart, 1, -45.0, interval, power, wait);
//		// 阻塞式等待, 等待旋转到目标角度
//		// 注意要跟设定值相同
//		FSUS_Wait(servo_usart, 0, -90.0, is_mturn);
//		FSUS_Wait(servo_usart, 1, -45.0, is_mturn);

//		// 等待2s
//		SysTick_DelayMs(2000);
//     //云台中间0度
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
//		//云台中间0度
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
		// 阻塞式等待, 等待旋转到目标角度
		//FSUS_Wait(servo_usart, 1, 90.0, is_mturn);//
		//SysTick_DelayMs(interval);
		
    //行驶初始状态
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
    //取物体：
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
   //放物体：
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
