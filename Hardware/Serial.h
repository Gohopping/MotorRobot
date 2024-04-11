#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>
#include "DataType.h"
#include "stm32f4xx.h" 
#include "ring_buffer.h"

#define USART_RECV_BUF_SIZE 2048
#define USART_SEND_BUF_SIZE 2048

typedef struct
{  
    USART_TypeDef *pUSARTx;
    // 发送端缓冲区
    RingBufferTypeDef *sendBuf;
		// 接收端缓冲区
    RingBufferTypeDef *recvBuf;
} Usart_DataTypeDef;

extern Usart_DataTypeDef usart1;
extern Usart_DataTypeDef *servo_usart;
void Serial_Init(void);
void Serial_SendByte(USART_TypeDef *pUSARTx,uint8_t Byte);
void Serial_SendArray(USART_TypeDef *pUSARTx,uint8_t *Array, uint16_t Length);
void Serial_SendString(USART_TypeDef *pUSARTx,char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
void Usart_SendAll(Usart_DataTypeDef *usart);
void Stock(void);
void PutDown(void);
void PickUp(void);
void modeyidong(void);
void InitArm(void);
void Recover(void);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);


void FSUSExample_SetNServoAngle(void);

#endif
