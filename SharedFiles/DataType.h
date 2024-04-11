#ifndef __DATATYPE_H__
#define __DATATYPE_H__



#define  _u8   unsigned char
#define  _u16  unsigned short
#define  _u32  unsigned long 

#define  _s8   signed char
#define  _s16  signed short
#define  _s32  signed long

#define  _fl   float


//λ����
#define  BIT_SET(X,Y)	      {X |= 1<<Y}
#define  BIT_RESET(X,Y)	    {X &= ~(1<<Y)}
#define  BIT_RETURN(X,Y)    {X&(1<<Y)}


#define  _NULL     0

//���ֵ����
typedef enum
{
    B_FALSE = 0,
    B_TRUE
    
}BOOL_DEF;

typedef  void (*p_func)(void);


#endif
