#ifndef __PID_H
#define __PID_H

#include<stdint.h>

int Incremental_PID(int reality,int target);
int Position_PID(int reality,int target);

#endif
