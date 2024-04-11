/**
  ******************************************************************************
  * @file    protocol.c
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   野火PID调试助手通讯协议解析
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
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
