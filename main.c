#include "SmartM_8051.h"
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>

//����״̬��
#define KEY_PORT            P3     //����IO
#define KEY_PORT_MASK       0x3C   //��������

#define KEY_SEARCH_STATUS   0      //��ѯ����״̬
#define KEY_ACK_STATUS      1      //ȷ�ϰ���״̬
#define KEY_REALEASE_STATUS 2      //�ͷŰ���״̬

//�豸����״̬��
#define MOTO_STOP   0
#define MOTO_RUN    1
#define MOTO_PASUE  2
#define LID_UP      3

UINT8  MOTO_STATE;


#define TIME_10MS           (65536-FOSC/1000)

//���Ե�ַ
#define IAP_ADDRESS 0x0400
#define TIME_HIGH_ADDRESS  IAP_ADDRESS
#define TIME_LOW_ADDRESS  IAP_ADDRESS+2
#define SPEED_HIGH_ADDRESS  0x0500
#define SPEED_LOW_ADDRESS  SPEED_HIGH_ADDRESS+2


UINT8 time_low,time_high;
UINT8 speed_low,speed_high;

BOOL  g_bTimer1IRQ=FALSE;

UINT16 RUN_TIME,MOTOR_SPEED,RUN_TIME_COUNTER;
STATIC UINT8  g_szTempBuf[4]={0},g_szTemp[2];
UINT16 MINUTE,SECOND;


/****************************************
*��������:KeyInit
*��    ��:��
*��    ��:��
*��    ��:������ʼ��
******************************************/
VOID KeyInit(VOID)
{
     P3M1|=  (1<<2)|(1<<3)|(1<<4)|(1<<5);
		 P3M0&=~((1<<2)|(1<<3)|(1<<4)|(1<<5));
}
/****************************************
*��������:LcdKey1
*��    ��:��
*��    ��:��
*��    ��:���ⰴ��1
******************************************/
VOID LcdKey1(VOID)
{
	 LcdFill(20,50,70,100,BLACK);     //KEY1����

	 MOTO_STATE=MOTO_RUN;	
//		speed_high=IapReadByte(TIME_HIGH_ADDRESS);  		// ��ȡ����ʱ�����ݵĸ߰�λ;
//		speed_low=IapReadByte(TIME_LOW_ADDRESS);				// ��ȡ����ʱ�����ݵĵͰ�λ;
//    Delay(10);     
//		
//		RUN_TIME=(speed_high<<8)|(speed_low);			//�ϲ���16λ���ݸ�ʽ
}
/****************************************
*��������:LcdKey2
*��    ��:��
*��    ��:��
*��    ��:���ⰴ��2
******************************************/
VOID LcdKey2(VOID)
{
	 LcdFill(180,50,230,100,BLACK);   //KEY2����
	 
	 MOTO_STATE=MOTO_STOP;	
//	 
//	  speed_high=(UINT8)(RUN_TIME>>8);
//		speed_low=(UINT8)(RUN_TIME);
//		

}
/****************************************
*��������:LcdKey3
*��    ��:��
*��    ��:��
*��    ��: �ٶ�����
******************************************/
VOID LcdKey3(VOID)
{
	 LcdFill(20,150,70,200,BLACK);    //KEY3����
		
//		if(MOTO_STATE!=MOTO_RUN)
//		{
//			 if(RUN_TIME>=1200)
//			 {
//					RUN_TIME=1200;
//			 }else
//			 {
//				 RUN_TIME++;
//			 }
//		 }	 		
//		
//		time_high=(UINT8)(RUN_TIME>>8);
//		time_low=(UINT8)RUN_TIME;

//		IapEraseSector(TIME_HIGH_ADDRESS);    //��������
//    Delay(10);     //����������Ҫ����ʱ����֤�����ü���·���ֵ�Ƿ�FF;
//    IapEraseSector(TIME_LOW_ADDRESS);    //��������
//    Delay(10);   
//		
//		IapProgramByte(TIME_HIGH_ADDRESS,time_high);			// д�����ʱ�����ݵĸ߰�λ;
//		IapProgramByte(TIME_LOW_ADDRESS,time_low);				// д�����ʱ�����ݵĵͰ�λ;
//		Delay(10); 		
//		
//		Delay(10); 		
//		UartSendByte(time_high);
//    Delay(10);     
//		UartSendByte(time_low); 
//		Delay(10);  		



	 if(MOTOR_SPEED>=1750)
	 {
			MOTOR_SPEED=1750;
	 }else
	 {
		 MOTOR_SPEED++;
	 }

	
		//������״̬������д��FLASH
		if(MOTO_STATE!=MOTO_RUN)
		{
	      speed_high=(UINT8)(MOTOR_SPEED>>8);
				speed_low=(UINT8)MOTOR_SPEED;
				Delay(10); 		
				UartSendByte(speed_high);
				Delay(10);     
				UartSendByte(speed_low); 
				Delay(10); 

				IapEraseSector(SPEED_HIGH_ADDRESS);    //��������
//				Delay(10);     //����������Ҫ����ʱ����֤�����ü���·���ֵ�Ƿ�FF;
				IapEraseSector(SPEED_LOW_ADDRESS);    //��������
				Delay(10);   
//				
				IapProgramByte(SPEED_HIGH_ADDRESS,speed_high);
				IapProgramByte(SPEED_LOW_ADDRESS,speed_low);
				
				UartSendByte(speed_high);
				Delay(10);     
				UartSendByte(speed_low); 
				Delay(10); 
		}
}

/****************************************
*��������:LcdKey4
*��    ��:��
*��    ��:��
*��    ��:�ٶȼ���
******************************************/
VOID LcdKey4(VOID)
{
	  LcdFill(180,150,230,200,BLACK);  //KEY4����	
		
//		if(MOTO_STATE!=MOTO_RUN)
//		{
//			 if(RUN_TIME<=1)
//			 {
//					RUN_TIME=1;
//			 }else
//			 {
//				 RUN_TIME--;
//			 }
//		 }	 		
//		time_high=(UINT8)(RUN_TIME>>8);
//		time_low=(UINT8)RUN_TIME;

//		IapEraseSector(TIME_HIGH_ADDRESS);    //��������
//    Delay(10);     //����������Ҫ����ʱ����֤�����ü���·���ֵ�Ƿ�FF;
//    IapEraseSector(TIME_LOW_ADDRESS);    //��������
//    Delay(10);   
//		
//		IapProgramByte(TIME_HIGH_ADDRESS,time_high);			// д�����ʱ�����ݵĸ߰�λ;
//		IapProgramByte(TIME_LOW_ADDRESS,time_low);				// д�����ʱ�����ݵĵͰ�λ;
//		Delay(10); 		
//		
//		Delay(10); 		
//		UartSendByte(time_high);
//    Delay(10);     
//		UartSendByte(time_low); 
//		Delay(10);  	

		 if(MOTOR_SPEED<=300)
		 {
					MOTOR_SPEED=300;
		 }else
		 {
					MOTOR_SPEED--;
		 }
		
	
		//������״̬������д��FLASH
		if(MOTO_STATE!=MOTO_RUN)
		{
	      speed_high=(UINT8)(MOTOR_SPEED>>8);
				speed_low=(UINT8)MOTOR_SPEED;
				Delay(10); 		
				UartSendByte(speed_high);
				Delay(10);     
				UartSendByte(speed_low); 
				Delay(10); 

				IapEraseSector(SPEED_HIGH_ADDRESS);    //��������
//				Delay(10);     //����������Ҫ����ʱ����֤�����ü���·���ֵ�Ƿ�FF;
				IapEraseSector(SPEED_LOW_ADDRESS);    //��������
				Delay(10);   
//				
				IapProgramByte(SPEED_HIGH_ADDRESS,speed_high);
				IapProgramByte(SPEED_LOW_ADDRESS,speed_low);
				
				UartSendByte(speed_high);
				Delay(10);     
				UartSendByte(speed_low); 
				Delay(10); 
		}
}
/****************************************
*��������:SECOND_MINUTE
*��    ��:��
*��    ��:��
*��    ��:����תΪ�����ʽ
******************************************/
VOID SECOND_MINUTE(UINT16 SEC)
{
		MINUTE=SEC/60;
		SECOND=SEC%60;	
	
}
/****************************************
*��������:Data_Moto_Speed_Init
*��    ��:��
*��    ��:��
*��    ��:
******************************************/
VOID Data_Moto_Speed_Init(VOID)
{ 
	
//		Delay(10); 
//		
//		RUN_TIME=999;
//		time_high=RUN_TIME>>8;
//		time_low=RUN_TIME;
//		speed_high=0x03;
//		speed_low=0xE8;		
//		
//	//��һ��доƬʱ����д��EEPROMʱ������ 	д��EEPROM�ٶ�����			 	
//	IapEraseSector(TIME_HIGH_ADDRESS);    //��������
//	IapEraseSector(TIME_LOW_ADDRESS);    //��������
//	IapEraseSector(SPEED_HIGH_ADDRESS);    //��������
//	IapEraseSector(SPEED_LOW_ADDRESS);    //��������
//	Delay(10);  //����������Ҫ����ʱ����֤�����ü���·���ֵ�Ƿ�FF;
//	IapProgramByte(TIME_HIGH_ADDRESS, time_high);			// д�����ʱ�����ݵĸ߰�λ;
//	IapProgramByte(TIME_LOW_ADDRESS, time_low);				// ��ȡ����ʱ�����ݵĵͰ�λ;
//	IapProgramByte(SPEED_HIGH_ADDRESS,speed_high );
//	IapProgramByte(SPEED_LOW_ADDRESS,speed_low );
//	Delay(10);  
		
		time_high=(IapReadByte(TIME_HIGH_ADDRESS));
		time_low=(IapReadByte(TIME_LOW_ADDRESS));
		speed_high=(IapReadByte(SPEED_HIGH_ADDRESS));
		speed_low=(IapReadByte(SPEED_LOW_ADDRESS));
		
		
		RUN_TIME=(time_high<<8)|(time_low);			//�ϲ���16λ���ݸ�ʽ
		MOTOR_SPEED=(speed_high<<8)|(speed_low);		
//		
		


    Delay(100);   

	
}
/****************************************
*��������:LcdKeyRst
*��    ��:��
*��    ��:��
*��    ��:���ⰴ���ָ�Ϊ�ͷ�״̬
******************************************/
VOID LcdKeyRst(VOID)
{
	 //ʱ���ʼ����ʾ
					
	 LcdFill(20,50,70,100,RED);             //KEY1
	 LcdFill(180,50,230,100,RED);           //KEY2
	 LcdFill(20,150,70,200,RED);            //KEY3
	 LcdFill(180,150,230,200,RED);		      //KEY4

	 LcdShowString(35,70, "K1",YELLOW,RED); //���'1'
	 LcdShowString(195,70,"K2",YELLOW,RED); //���'2'
	 LcdShowString(35,170,"K3",YELLOW,RED); //���'3'
	 LcdShowString(195,170,"K4",YELLOW,RED);//���'4'
	 

	 LcdFill(20,210,70,250,BLUE);             //KEY1	 
	 LcdShowString(25,215, "ʱ��:",YELLOW,BLUE);
	 
	 LcdFill(20,260,70,300,BLUE); 
	 LcdShowString(25,265,"�ٶ�:",YELLOW,BLUE); 
//	 sprintf(g_szTempBuf,"%04d",MOTOR_SPEED);
	 LcdShowString(75,265,g_szTempBuf,BLUE,WHITE); 

}

/****************************************
*��������:Timer1Init
*��    ��:��
*��    ��:��
*��    ��:��ʱ��1��ʼ�� 
******************************************/
VOID Timer1Init(VOID)
{
//     AUXR|=0x40;
//		 TL1=(UINT8) TIME_10MS;
//		 TH1=(UINT8)(TIME_10MS>>8);
//		 TMOD|=0x10;
//		 TR1=1;

			AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
			TMOD &= 0x0F;		//���ö�ʱ��ģʽ
			TL1 = 0x40;		//���ö�ʱ��ֵ
			TH1 = 0xA2;		//���ö�ʱ��ֵ
			TF1 = 0;		//���TF1��־
			TR1 = 1;		//��ʱ��1��ʼ��ʱ

			ET1=1;
}



/****************************************
*��������:KeyRead
*��    ��:��
*��    ��:��
*��    ��:����ֵ��ȡ
******************************************/
VOID KeyRead(VOID)
{
	  STATIC UINT8 ucKeyStatus=KEY_SEARCH_STATUS;
           UINT8 ucKeyValue=0;
           UINT8 i=0;
	  
    /*   ��ȡ����ֵ */
	  ucKeyValue=(~KEY_PORT)&KEY_PORT_MASK;	  
  
    switch(ucKeyStatus)
    {
			/* ������ѯ״̬ */
			case KEY_SEARCH_STATUS:
					{
							 if(ucKeyValue)
							 {
									ucKeyStatus=KEY_ACK_STATUS;
							 }

					}break;
					
      /* ����ȷ��״̬ */
			case KEY_ACK_STATUS:   
					{
							 if(!ucKeyValue) //û�а�������
							 {
									ucKeyStatus=KEY_SEARCH_STATUS;
							 }
							 else//�а�������
							 {	

								  if(ucKeyValue & (1<<2))LcdKey1();
								  if(ucKeyValue & (1<<3))LcdKey2();
								  if(ucKeyValue & (1<<4))LcdKey3();
								  if(ucKeyValue & (1<<5))LcdKey4();

							 }

							 ucKeyStatus=KEY_REALEASE_STATUS;
							 
							 
					}break;
      /* �����ͷ�״̬ */
			case KEY_REALEASE_STATUS:
					{
							 if(!ucKeyValue)   //�����ͷ�
							 {
								   ucKeyStatus=KEY_SEARCH_STATUS;
								 
                   LcdKeyRst();              							 
							 }
					 
					}break;

					default:break;
    }
}

/****************************************
*��������:main
*��    ��:��
*��    ��:��
*��    ��:��������
******************************************/
VOID main(VOID)
{


   UartInit(9600);	
		
		
	 /*   LCD ��ʼ�� */
	 LcdTypeSet(LCD_TYPE_32);
	 LcdInit(LCD_FONT_IN_FLASH,LCD_DIRECTION_180);                                  
	 LcdCleanScreen(WHITE);     

	 
   /*   LED ��ʼ�� */
	 LedInit();

	 /*   SPI FLash ��ʼ�� */
	 SpiFlashInit();  
	 
	 MOTO_STATE=0;
	 RUN_TIME=820;				
	 MOTOR_SPEED=900;
	 RUN_TIME_COUNTER=0;
	 MINUTE=0;
	 SECOND=0;
	 MOTO_STATE=MOTO_STOP;


		LcdShowString(80,10,"��ĥ�����Ƴ���",BLUE,WHITE);
		LcdShowString(100,30,"V2.0 ",BLUE,WHITE);


	 /*   ���� ��ʼ�� */
	 KeyInit();
	 LcdKeyRst();
	 
	 Data_Moto_Speed_Init();	
	 
	 /*   ��ʱ�� ��ʼ�� */
	 Timer1Init(); 
	 
	 IRQ_ENABLE();				

	 while(1)
	 {	

				 
				 if(g_bTimer1IRQ)
				{
							g_bTimer1IRQ=FALSE;
							
							/* ������ȡ */
							KeyRead();		 
											
							sprintf(g_szTempBuf,"%04d",RUN_TIME);				//sprintfֻ�ܶ�˫�ֽ�UINT16���в���
							LcdShowString(100,120,g_szTempBuf,BLUE,WHITE);
						
							SECOND_MINUTE(RUN_TIME);									//�����ݸ�ʽת��Ϊ��:�� ��ʽ
							sprintf(g_szTempBuf,"%02d",MINUTE);				//sprintfֻ�ܶ�˫�ֽ�UINT16���в���
							LcdShowString(75,215,g_szTempBuf,BLUE,WHITE);
							sprintf(g_szTempBuf,":%02d",SECOND);
							LcdShowString(107,215,g_szTempBuf,BLUE,WHITE);
							
							sprintf(g_szTempBuf,"%04d",MOTOR_SPEED);
							LcdShowString(75,265,g_szTempBuf,BLUE,WHITE); 
				}
			 
	 }


}

/****************************************
*��������:TMR1_IRQHandler
*��    ��:��
*��    ��:��
*��    ��:��ʱ��1�жϷ�����
******************************************/
VOID Timer1_ISR(VOID) interrupt 3 using 1
{
    g_bTimer1IRQ=TRUE;

//	TL1=(UINT8) TIME_10MS;
//	TH1=(UINT8)(TIME_10MS>>8);

		/* ��ʱ��1��װ�� */
		TL1 = 0x40;		//���ö�ʱ��ֵ
		TH1 = 0xA2;		//���ö�ʱ��ֵ

		
		if(MOTO_STATE==MOTO_RUN)				//������״̬�� ���������ʱ��
		{
				if(RUN_TIME_COUNTER>=1000)		//1���ʱ
				{
						 RUN_TIME_COUNTER=0;
						 RUN_TIME--;
				}
				
				 RUN_TIME_COUNTER++;
		}
}
