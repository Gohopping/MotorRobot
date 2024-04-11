#ifndef __GLOBALFUNCTION_H__
#define __GLOBALFUNCTION_H__

#include "DataType.h"
#include "string.h"
#include "stdlib.h"



typedef struct
{
  _u8 sys_flag_1ms:1;
  _u8 sys_flag_5ms:1;
  _u8 sys_flag_10ms:1;
  _u8 sys_flag_50ms:1;
  _u8 sys_flag_250ms:1;
  _u8 sys_flag_1s:1;

}TIME_BASE_FLAG_STR;
typedef union
{
	TIME_BASE_FLAG_STR      fbit;	
    _u8                   byte;                  
}TIME_BASE_UN;

extern TIME_BASE_UN  TIME_BASE;

void f_get_timebase_flag(void);
void f_common_delay(_u16 us_count);


#endif
