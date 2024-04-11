/*==========================================================================================
Copyright (c)  Frank.  All Rights Reserved
==========================================================================================
Model Name   :            
==========================================================================================
Start Date   :            2019.5.24
Update Date  :
Author       :            Frank
==========================================================================================*/
#include "GlobalFunction.h"


TIME_BASE_UN  TIME_BASE = {0};
_u16 time_base_count    = 0;

/*==========================================================================================
Function:     ��ȡʱ����־
Input: 	    
Output: 	
Related: 	
==========================================================================================*/
void f_get_timebase_flag(void)
{
  time_base_count++;
	
	if(0 == (time_base_count%5))
	{
		TIME_BASE.fbit.sys_flag_5ms = 1;
	}
	
	if(0 == (time_base_count%10))
	{
		TIME_BASE.fbit.sys_flag_10ms = 1;
	}
	
	if(0 == (time_base_count%50))
	{
		TIME_BASE.fbit.sys_flag_50ms = 1;
	}
	
	if(0 == (time_base_count%250))
	{
		TIME_BASE.fbit.sys_flag_250ms = 1;
	}
	
	if(0 == (time_base_count%1000))
	{
	  time_base_count = 0;
		TIME_BASE.fbit.sys_flag_1s = 1;
	}

}



/*==========================================================================================
Function:     ������ʱ��������������ȴ�
Input: 	    
Output: 	
Related: 	
==========================================================================================*/
void f_common_delay(_u16 us_count)
{
	volatile _u16 delay_count = us_count;
	while(--delay_count);

}

