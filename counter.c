
#include "SmartM_8051.h"

UINT8  CCWS[4];

VOID Timer3Init(VOID) ;

/****************************************
 ��ʱ��3 �����ⲿ����������ת��
*��������:Timer3Init
*��    ��:��
*��    ��:��
*��    ��:��ʱ��3��ʼ��,�����P0.0 
******************************************/
VOID Timer3Init(VOID)   
{
		UINT16 DS;
		UINT8  DF;;
//		DF=0x31;
//		DS=6553;
	
//    UartSendStr(" T4T3M Begin Init 6553!\r\n");
    T4T3M |= 0x02;              //��ʱ��3Ϊ1Tģʽ
    T4T3M |= 0x04;              //��ʱ��3������ģʽ
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
//	    TMOD = 0x40;                    //���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
//    TL1 =0;// T1MS;                     //��ʼ����ʱֵ
//    TH1 =0;// T1MS >> 8;
//    TR1 = 1;  
//    UartSendStr(" \r\n");
//    UartSendStr(" T4T3M Init finish!\r\n");

}