#include "SmartM_8051.h"
extern VOID KeyGet(VOID);
extern void Pwm_Run();

//����״̬��

#define KEY_PRESS_LOW				0
#define KEY_PRESS_HIGH			1

UINT8 KEY_PRESS_STATE=KEY_PRESS_LOW;

//#define KEY_SEARCH_STATUS   0      //��ѯ����״̬
//#define KEY_ACK_STATUS      1      //ȷ�ϰ���״̬
//#define KEY_REALEASE_STATUS 2      //�ͷŰ���״̬

#define Time_Up    		0x04
#define Time_Down  		0x08
#define Speed_Up   		0x10
#define Speed_Down 		0x20
#define Start_Button  0x40
#define Stop_Button   0x80

//���ڼ�ⰴ���ı���
UINT8 Trg;		
UINT8 Cont;
UINT i=0;				//������������
//UINT8  CC[4];


UINT8  MOTO_STATE;
UINT16 RUN_TIME,MOTOR_SPEED,RUN_TIME_TEMP;

UINT8 time_low,time_high;
UINT8 speed_low,speed_high;
//STATIC UINT8  g_szTempBuf[4]={0},g_szTemp[2];
/****************************************
*��������:Write_Data_Flash
*��    ��:��
*��    ��:��
*��    ��:�ٶ�ʱ������д��FLASH��
******************************************/
Write_Data_Flash()
{
				time_high=RUN_TIME_TEMP>>8;
				time_low=RUN_TIME_TEMP;
	      speed_high=(UINT8)(MOTOR_SPEED>>8);
				speed_low=(UINT8)MOTOR_SPEED;
				Delay(10); 		
				UartSendByte(speed_high);
				Delay(10);     
				UartSendByte(speed_low); 
				Delay(10); 

				IapEraseSector(TIME_HIGH_ADDRESS);    //��������
				IapEraseSector(TIME_LOW_ADDRESS);    //��������
				IapEraseSector(SPEED_HIGH_ADDRESS);    //����������Ҫ����ʱ����֤�����ü���·���ֵ�Ƿ�FF;
				IapEraseSector(SPEED_LOW_ADDRESS);    //��������
				Delay(10);   
			
				IapProgramByte(TIME_HIGH_ADDRESS,time_high);
				IapProgramByte(TIME_LOW_ADDRESS,time_low);
				IapProgramByte(SPEED_HIGH_ADDRESS,speed_high);
				IapProgramByte(SPEED_LOW_ADDRESS,speed_low);
				
				UartSendByte(speed_high);
				Delay(10);     
				UartSendByte(speed_low); 
				Delay(10); 
	}
/****************************************
*��������:KeyInit
*��    ��:��
*��    ��:��
*��    ��:������ʼ��
******************************************/
VOID KeyInit(VOID)
{
//     P3M1|=  (1<<2)|(1<<3)|(1<<4)|(1<<5);
//		 P3M0&=~((1<<2)|(1<<3)|(1<<4)|(1<<5));
     P1M1|=  (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<8);
		 P1M0&=~((1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
}
/****************************************
*��������:Key_Run
*��    ��:��
*��    ��:��
*��    ��:�����������
******************************************/
VOID Key_Run(VOID)
{	
		if(MOTO_STATE==MOTO_STOP)
		{
					RUN_TIME=RUN_TIME_TEMP;	
					UartSendStr("_RUN!");  
		}
		 MOTO_STATE=MOTO_START;		//MOTO_RUN;	
}
/****************************************
*��������:Key_Stop
*��    ��:��
*��    ��:��
*��    ��:ֹͣ�������
******************************************/
VOID Key_Stop(VOID)
{
	if((MOTO_STATE==MOTO_RUN))
	{
				MOTO_STATE=MOTO_PASUE;			
				UartSendStr("_PASUE !");  
	}else if((MOTO_STATE==MOTO_PASUE)||(MOTO_STATE==MOTO_ERROR)|(MOTO_STATE==MOTO_START))
	{
				MOTO_STATE=MOTO_STOP;			
					UartSendStr("_STOP!");  
				RUN_TIME=RUN_TIME_TEMP;
	}else{
	}

}
/****************************************
*��������:Key_SpeedUp
*��    ��:��
*��    ��:��
*��    ��: �ٶ����Ӱ���
******************************************/
VOID Key_SpeedUp(VOID)
{
//	 LcdFill(20,150,70,200,BLACK);    //KEY3����
//else{
		 if(KEY_PRESS_STATE==KEY_PRESS_HIGH)
		 { 
				MOTOR_SPEED=MOTOR_SPEED+50;
		 }else if(KEY_PRESS_STATE==KEY_PRESS_LOW){	
				MOTOR_SPEED=MOTOR_SPEED+10;
		 }			
	
		 if(MOTOR_SPEED>=1750)
		 {
				MOTOR_SPEED=1750;
		 }	 
//	 }
	 
//		sprintf(CC, "%u", MOTOR_SPEED); 
//				UartSendStr("SPEED_UP:");
//				UartSendStr(CC);	

//	      speed_high=(UINT8)(MOTOR_SPEED>>8);
//				speed_low=(UINT8)MOTOR_SPEED;

		if(MOTO_STATE==MOTO_RUN)
		{	 
				Pwm_Run();
		}

		if(MOTO_STATE==MOTO_STOP)
		{
	      speed_high=(UINT8)(MOTOR_SPEED>>8);
				speed_low=(UINT8)MOTOR_SPEED;
				Write_Data_Flash();
		}
}

/****************************************
*��������:Key_Speed_Down
*��    ��:��
*��    ��:��
*��    ��:�ٶȼ��ٰ���
******************************************/
VOID Key_Speed_Down(VOID)
{
			
	   if(KEY_PRESS_STATE==KEY_PRESS_HIGH)
			{ 
					MOTOR_SPEED=MOTOR_SPEED-50;	
			 }else if(KEY_PRESS_STATE==KEY_PRESS_LOW){		
					MOTOR_SPEED=MOTOR_SPEED-10;
			 }
		
  		if(MOTOR_SPEED<=500)
		 {
					MOTOR_SPEED=500;
		 }
		
		
//		sprintf(CC, "%u", MOTOR_SPEED); 
//				UartSendStr("SPEED_DOWN:");
//				UartSendStr(CC);		
	
		//������״̬������д��FLASH
		if(MOTO_STATE==MOTO_RUN)
		{	 
				Pwm_Run();
		}

		if(MOTO_STATE==MOTO_STOP)
		{
	      speed_high=(UINT8)(MOTOR_SPEED>>8);
				speed_low=(UINT8)MOTOR_SPEED;
				Write_Data_Flash();
		}
}
/****************************************
*��������:Key_Time_Up
*��    ��:��
*��    ��:��
*��    ��:ʱ�����Ӱ���
******************************************/
VOID Key_Time_Up(VOID)
{
				 if(RUN_TIME_TEMP>=900)
				 {
							RUN_TIME_TEMP=900;
				 }else
				 {
							if(KEY_PRESS_STATE==KEY_PRESS_HIGH)
							{ 
									RUN_TIME_TEMP=RUN_TIME_TEMP+10;
							 }else if(KEY_PRESS_STATE==KEY_PRESS_LOW){		
									RUN_TIME_TEMP=RUN_TIME+1;
							 }
				 }
//				sprintf(CC, "%u", RUN_TIME_TEMP); 
//				UartSendStr("TIME_UP:");
//				UartSendStr(CC);	
//				 
				 Write_Data_Flash();
				 RUN_TIME=RUN_TIME_TEMP;	
		
}
/****************************************
*��������:Key_Time_Down
*��    ��:��
*��    ��:��
*��    ��:ʱ����ٰ���
******************************************/
VOID Key_Time_Down(VOID)
{

				 if(RUN_TIME_TEMP<=10)
				 {
							RUN_TIME_TEMP=10;
				 }else
				 {
							if(KEY_PRESS_STATE==KEY_PRESS_HIGH)
							{ 
									RUN_TIME_TEMP=RUN_TIME_TEMP-10;
							 }else if(KEY_PRESS_STATE==KEY_PRESS_LOW){		
									RUN_TIME_TEMP=RUN_TIME_TEMP-1;
							 }
				 }	
//				sprintf(CC, "%u", RUN_TIME_TEMP); 
//				UartSendStr("TIME_UP:");
//				UartSendStr(CC);	
				 
				 Write_Data_Flash();
				 RUN_TIME=RUN_TIME_TEMP;
		
}
/****************************************
*��������:KeyGet
*��    ��:��
*��    ��:��
*��    ��:����ֵ��ȡ
******************************************/
VOID KeyGet(VOID)
{
//	    STATIC UINT8 ucKeyStatus=KEY_SEARCH_STATUS;
      UINT8 ucKeyValue=0;
      UINT8 ReadData;// = P3^0xff; ��
			ucKeyValue=(~KEY_PORT)&KEY_PORT_MASK;		//
			ReadData= KEY_PORT^KEY_PORT_MASK;   // 1 ��P1�Ķ˿����ݣ�ȡ����Ȼ���͵�ReadData ��ʱ�������汣������
      Trg = ReadData & (ReadData ^ Cont); // 2 �㷨1���������㴥�������ġ�һ��λ�������һ��������
      Cont = ReadData; // 3 �㷨2��������������������

			if(ucKeyValue & (1<<7))
			{	
						if(MOTO_STATE!=MOTO_STOP)//||(MOTO_STATE==MOTO_RUN))
						{	
								Key_Stop();
								Display_Time1();
						}
			}
			
//			if(ucKeyValue & (1<<2))
//			{					
//						if(MOTO_STATE!=MOTO_RUN)
//						{	
//								Key_Run();
//								Display_Time1();
//						}
//			}
//			
			if(ucKeyValue & (1<<6))
			{
				if (Trg)
				{
						i=0;
				}			
				if (Cont )		//������
				{
						if(i<1)
						{
								i++;
						}else{
								if(MOTO_STATE==MOTO_STOP)
								{
									Key_Run();
									Display_Time1();
								}
						}
				}								
			}
			
			//��ʱ���ܰ���,������״̬�����������޸�д��
//			if(ucKeyValue & (1<<2))
////			if(Trg & Time_Up)
//			{
//						
////						if(MOTO_STATE!=MOTO_RUN)
//						{	
//								if (Trg & Time_Up)
//								{
//										KEY_PRESS_STATE=KEY_PRESS_LOW;
//										i=0;
//										UartSendStr("TIME UP1!");
//								}			
//								if (Cont & Time_Up) 		//��������״̬
//								{
//										if(i<=6)
//										{
//												KEY_PRESS_STATE=KEY_PRESS_LOW;
//												i++;
//												UartSendStr("TIME UP2!");
//										}else{
//												KEY_PRESS_STATE=KEY_PRESS_HIGH;
//												UartSendStr("TIME UP+++!");
//										}
//								}			
//								Key_Time_Up();
//								Display_Time1();
//						}

//			}

			if(ucKeyValue & (1<<2))
			{
						//ʱ��,������״̬�����������޸�д��
//						if((MOTO_STATE==MOTO_RUN)|(MOTO_STATE==MOTO_START))
//						{		
//						}else
						if(MOTO_STATE==MOTO_STOP){
									if (Trg)
									{
											KEY_PRESS_STATE=KEY_PRESS_LOW;
											i=0;
//										UartSendStr("TIME UP1!");
									}			
									if (Cont )   //��������״̬
									{
											if(i<=6)
											{
													KEY_PRESS_STATE=KEY_PRESS_LOW;
													i++;
//												UartSendStr("TIME UP2!");
											}else{
													KEY_PRESS_STATE=KEY_PRESS_HIGH;
												UartSendStr("TIME UP+++!");
											}
									}			
									Key_Time_Up();
									Display_Time1();
						}
			}
			
			if(ucKeyValue & (1<<3))
			{
						//ʱ��,������״̬�����������޸�д��
						if(MOTO_STATE==MOTO_STOP)
						{		
									if (Trg)
									{
											KEY_PRESS_STATE=KEY_PRESS_LOW;
											i=0;
									}			
									if (Cont )   //��������״̬
									{
											if(i<=6)
											{
													KEY_PRESS_STATE=KEY_PRESS_LOW;
													i++;
											}else{
													KEY_PRESS_STATE=KEY_PRESS_HIGH;
											}
									}			
									Key_Time_Down();
									Display_Time1();
						}
			}
			

			//�ٶȰ���,����״̬�����������޸ģ�����д��			
			if(ucKeyValue & (1<<4))
			{

								if (Trg)
								{
										KEY_PRESS_STATE=KEY_PRESS_LOW;
										i=0;
								}			
								if (Cont )		//��������״̬
								{
										if(i<=6)
										{
												KEY_PRESS_STATE=KEY_PRESS_LOW;
												i++;
										}else{
												KEY_PRESS_STATE=KEY_PRESS_HIGH;
										}
								}			
								Key_SpeedUp();
								Display_Time1();
					
			}

			if(ucKeyValue & (1<<5))
			{
									if (Trg)
									{
											KEY_PRESS_STATE=KEY_PRESS_LOW;
											i=0;
									}			
									if (Cont )   //��������״̬
									{
											if(i<=6)
											{
													KEY_PRESS_STATE=KEY_PRESS_LOW;
													i++;
											}else{
													KEY_PRESS_STATE=KEY_PRESS_HIGH;
											}
									}			
									Key_Speed_Down();
									Display_Time1();
						}

}

