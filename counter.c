
#include "SmartM_8051.h"

UINT8  CCWS[4];

VOID Timer3Init(VOID) ;

/****************************************
 定时器3 用于外部计数，测量转速
*函数名称:Timer3Init
*输    入:无
*输    出:无
*功    能:定时器3初始化,输入口P0.0 
******************************************/
VOID Timer3Init(VOID)   
{
		UINT16 DS;
		UINT8  DF;;
//		DF=0x31;
//		DS=6553;
	
//    UartSendStr(" T4T3M Begin Init 6553!\r\n");
    T4T3M |= 0x02;              //定时器3为1T模式
    T4T3M |= 0x04;              //定时器3计数器模式
	  TL3=0x26;
	  TH3=01;
	
//				sprintf(CCWS, "%u", DS); 
//				UartSendStr(CCWS);	
//		DS=TH3;
//		DS=(TH3<<8);
//		DS=TL3;//|(TH3<<8);
//				sprintf(CCWS, "%u", DS); 
//    UartSendStr(" \r\n");
//    UartSendStr(" TL3= ");
//				UartSendStr(CCWS);	
//		DF=(UINT8)TL3;
//    UartSendStr(" TL3= ");
//				UartSendByte(TL3);	
//    UartSendStr(" \r\n");
				
//		DS=DF;//|(DF<<8);//|(TH3<<8);
//				sprintf(CCWS, "%u", DS); 
//    UartSendStr(" \r\n");
//		UartSendByte(TH0);
//    UartSendStr(" DFDF= ");
//				UartSendStr(CCWS);	
				
				
//		DS=(TH3<<8)|TL3;
//    UartSendStr(" \r\n");
//    UartSendStr(" T4T3M Init Setp 2 !\r\n");
//				sprintf(CCWS, "%u", DS); 
//				UartSendStr(CCWS);	
		IE2 = 0x20;
		
//				UartSendStr("TL3:");
//				UartSendStr(CCWS);	
//	    TMOD = 0x40;                    //设置定时器为模式0(16位自动重装载)
//    TL1 =0;// T1MS;                     //初始化计时值
//    TH1 =0;// T1MS >> 8;
//    TR1 = 1;  
//    UartSendStr(" \r\n");
//    UartSendStr(" T4T3M Init finish!\r\n");

}