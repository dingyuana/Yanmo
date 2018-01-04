#include "8051.h"
#include "Typedef.h"
#include <intrins.h>
#include "Common.h"


VOID delay_1us(VOID)	
{
	UINT8 i;
#if FOSC == 33000000UL
	_nop_();
	_nop_();
	_nop_();
	i = 5;
	while (--i);
#endif

#if FOSC == 24000000UL
	_nop_();
	_nop_();
	i = 3;
	while (--i);
#endif
}

VOID delay_10us(VOID)
{
  UINT8 i;
#if FOSC == 33000000UL
	i = 80;
	while (--i);
#endif

#if FOSC == 24000000UL

	NOP();
	NOP();
	i = 57;
	while (--i);
#endif
}

VOID delay_100us(VOID)
{
	UINT8 i, j;
#if FOSC == 33000000UL


	i = 4;
	j = 50;
	do
	{
		while (--j);
	} while (--i);
#endif

#if FOSC == 24000000UL
	i = 3;
	j = 82;
	do
	{
		while (--j);
	} while (--i);
#endif
}

VOID delay_1ms(VOID)
{
	UINT8 i, j;
#if FOSC == 33000000UL


	i = 33;
	j = 22;
	do
	{
		while (--j);
	} while (--i);
#endif

#if FOSC == 24000000UL

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
#endif
}

VOID delay_10ms(VOID)		
{
	UINT8 i, j, k;
#if FOSC == 33000000UL


	NOP();
	NOP();
	
	i = 2;
	j = 65;
	k = 253;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
#endif

#if FOSC == 24000000UL

	_nop_();
	_nop_();
	i = 1;
	j = 234;
	k = 113;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
#endif
}

VOID delay_100ms(VOID)	
{
	UINT8 i, j, k;
#if FOSC == 33000000UL


	NOP();
	NOP();
	i = 13;
	j = 139;
	k = 12;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
#endif

#if FOSC == 24000000UL
	_nop_();
	_nop_();
	i = 10;
	j = 31;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
#endif
}

/******************************************************************
*函数名称:Delayus
*输    入:t 延时时间
*输    出:无
*说    明:微秒级延时 最低为10us
*******************************************************************/
VOID Delayus(UINT16 t)
{
   UINT8 t0=t/100;
   UINT8 t1=t%100/10;

   while(t0)
	 {
	     delay_100us();
			 
			 if(t0)t0--;
			 else  break;
	 }
	 
   while(t1)
	 {
	     delay_10us();
			 
			 if(t1)t1--;
			 else  break;
	 }	 


}
/******************************************************************
*函数名称:Delayms
*输    入:t 延时时间
*输    出:无
*说    明:毫秒级延时
*******************************************************************/
VOID Delayms(UINT16 t)
{
   UINT8 t0=t/100;
   UINT8 t1=t%100/10;
	 UINT8 t2=t%10;
	 
   while(t0)
	 {
	     delay_100ms();
			 
			 if(t0)t0--;
			 else  break;
	 }
	 
   while(t1)
	 {
	     delay_10ms();
			 
			 if(t1)t1--;
			 else  break;
	 }	 

   while(t2)
	 {
	     delay_1ms();
			 
			 if(t2)t2--;
			 else  break;
	 }	 
}

