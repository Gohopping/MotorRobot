/**
  ******************************************************************************
  * @file    protocol.c
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   Ұ��PID��������ͨѶЭ�����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "protocol.h"
#include "motor.h"
#include <string.h>
//#include "motor.h"
//#include "timer.h"
//#include "bsp_pid.h"

PROTOCOL_INFO_STR  PROTOCOL_INFO = {0};
MODE_OWN_INFO_STR   MODE_OWN_INFO[MOTOR_CHANNEL_NUM] = {0};
