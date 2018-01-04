#ifndef __COMMON_H__
#define __COMMON_H__









/*
              ---      可能用到的宏      ---
*/
#define FOSC              24000000UL
//#define FOSC            33000000UL   
											//11059200UL	

#if (FOSC >= 24000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_30MHZ
#elif (FOSC >= 20000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_24MHZ
#elif (FOSC >= 12000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_20MHZ
#elif (FOSC >= 6000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_12MHZ
#elif (FOSC >= 3000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_6MHZ
#elif (FOSC >= 2000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_3MHZ
#elif (FOSC >= 1000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_2MHZ
#else
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_1MHZ
#endif


#define TRUE              0x01
#define FALSE             0x00



#define NOP()  _nop_()


#define IRQ_ENABLE()      EA=1	//总中断关闭
#define IRQ_DISABLE()	    EA=0	//开总中断

#define SWAP16(x)        ((((UINT16)(x))<<8)|(((UINT16)(x))>>8))
#define SWAP32(x)        ((((UINT32)SWAP16(x))<<16)|SWAP16((x)>>16))
#define LSB(x)           ((UINT8)(x))
#define MSB(x)           ((UINT8)(((UINT16)(x))>>8)) 
   

#define EN_HAL_DELAY      (1)                 //是否使能硬件定时器作延时




EXTERN_C VOID Delayus(UINT16 t);
EXTERN_C VOID Delayms(UINT16 t);



#endif