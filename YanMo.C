/** * 
*----------Dragon be here!----------/ 
* �������������������� 
* ���������ߩ��������ߩ� 
* ���������������������� 
* ���������������������� 
* ���������ש������ס��� 
* ���������������������� 
* �������������ߡ������� 
* ���������������������� 
* ���������������������� 
* ���������������������ޱ��� 
* ������������������������BUG�� 
* �������������������������� 
* �������������������������ǩ� 
* ���������������������������� 
* �������������������ש����� 
* �������������ϩϡ����ϩ� 
* �������������ߩ������ߩ� 
* ���������������޳�û������������by:dingyuan */

#include "SmartM_8051.h"
#include <rtx51tny.h>                 /* RTX-51 tiny functions & defines      */



//#define KEY_PORT            P1     //����IO
//#define KEY_PORT_MASK       0xFC   //�������룬ʹ��2.3.4.5.6.7�����ܽ�
#define KEY_SEARCH_STATUS   0      //��ѯ����״̬
#define KEY_ACK_STATUS      1      //ȷ�ϰ���״̬
#define KEY_REALEASE_STATUS 2      //�ͷŰ���״̬


extern UINT8  MOTO_STATE;
extern UINT16 RUN_TIME,RUN_TIME_TEMP,MOTOR_SPEED;
extern void PWMInit(UINT8 ucPwmClock);
extern void Pwm_Run();
extern void Pwm_Stop();
extern void Pwm_Start();
extern void KeyGet(VOID);
extern void LCDInit();
extern void Display_Time1();
extern void KeyInit();
extern void Diaplay_SUB();
extern void Timer3Init();
extern void BEEP(UINT8 cycle,UINT8 beep);

UINT8  CCS[8];

struct time ctime = { 12,  0,  0 };   /* storage for clock time values        */
unsigned long counter0=0;  


/****************************************
*��������:Data_Moto_Speed_Init
*��    ��:��
*��    ��:��
*��    ��:����������ݳ�ʼ������FLAH�ж����ٶȺ͹���ʱ�����
******************************************/
VOID Data_Moto_Speed_Init(VOID)
{ 

		UINT8 time_low,time_high;
		UINT8 speed_low,speed_high;	
		Delay(10); 
		
		RUN_TIME=99;
		time_high=RUN_TIME>>8;
		time_low=RUN_TIME;
		speed_high=0x03;
		speed_low=0xE8;		
		
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
//		
		time_high=(IapReadByte(TIME_HIGH_ADDRESS));
		time_low=(IapReadByte(TIME_LOW_ADDRESS));
		speed_high=(IapReadByte(SPEED_HIGH_ADDRESS));
		speed_low=(IapReadByte(SPEED_LOW_ADDRESS));
		
		
		RUN_TIME=(time_high<<8)|(time_low);			//�ϲ���16λ���ݸ�ʽ
		RUN_TIME_TEMP=RUN_TIME;
		MOTOR_SPEED=(speed_high<<8)|(speed_low);	
//		MOTOR_SPEED=1620;		

    Delay(100);   	
}

/******************************************************************************/
/*        Task 0 'init': Initialize   ��ʼ���� �����߳�                      */
/******************************************************************************/
void init (void) _task_ INIT     /* program execution starts here        */
{    
//		 LED1=1;
	     LED2=1;
//		 Bell_Run;
		 BELL=1;
		 MOTOR_DISABLE;
   
		 UartInit (9600);                     /* ��ʼ��PWMʱ�ӣ�ָ���ܽ����     */	
		 PWMInit(SYSCLK_DIV_2);
		 KeyInit();					//������ʼ�� P3��������
		 LCDInit();	 				//192 64 ����Һ����ʼ��
		 Timer3Init();			//��������ʼ��
	
//				sprintf(CCS, "%u", TL3); 
//				UartSendStr("TL3:");
//				UartSendStr(CCS);	
//		 IRQ_ENABLE();
	
//			RUN_TIME=820;				//	������
//			MOTOR_SPEED=500;
			Data_Moto_Speed_Init();
			MOTO_STATE=MOTO_STOP;  
		 	BELL=0;
//		    Bell_Stop;
			Diaplay_Init();
//					if(TIME_UP){
//							UartSendStr("TIME_UP high"); 
//
//					}else
//					{
//							UartSendStr("TIME_UP low"); 
//					}
//			
//					if(TIME_DOWN){
//							UartSendStr("TIME_DOWN high"); 
//
//					}else
//					{
//							UartSendStr("TIME_DOWN low"); 
//					}
			
			
			Display_Time1();
//			UartSendStr("Initialize scuess!"); 
	 
			os_create_task (CLOCK);             /* start clock task                     */
			os_create_task (DISPLAY);           /* start command task                   */
			os_create_task (MOTO);            /* start lights task                    */
			os_create_task (KEYREAD);           /* start keyread task                   */
//			os_create_task (COUNTER);           /* start keyread task                   */
			os_delete_task (INIT);              /* stop init task (no longer needed)    *///  

}


/******************************************************************************/
/*        Task 2 'clock'      ����ʱ���߳� ������ʱ��Ƭ5MS                      */
/******************************************************************************/
void clock (void)  _task_ CLOCK  
{
  while (1)  
		{                        /* ʱ����ʾ             */
					if (++ctime.sec == 60)  
				  {         /* calculate the second                 */
							ctime.sec = 0;
							if (++ctime.min == 60)  
							{       /* calculate the minute                 */
									ctime.min = 0;
									if (++ctime.hour == 24)  
									{    /* calculate the hour                   */
										ctime.hour = 0;
									}
							 }
					}
					
					if((MOTO_STATE==MOTO_RUN)||(MOTO_STATE==MOTO_START)){
								RUN_TIME--;				//	����ʱ
//								BELL=!BELL;
//								os_send_signal (COUNTER);  
					}

					
					if(RUN_TIME<=0)	//	����ʱ���㣬�ָ�����״̬
					{
						MOTO_STATE=MOTO_STOP;		//״̬���ָ�ͣ��״̬
						RUN_TIME=RUN_TIME_TEMP;	//ʱ��ָ���ʼ
						Display_Time1();				//ˢ��Һ�����ָ���ʼ
						
					}
					
//					if(MOTO_STATE==MOTO_STOP)	
//							UartSendStr("MOTO_STOP !");  
//					else if (MOTO_STATE==MOTO_RUN)
//							UartSendStr("MOTO_RUN !");  
//					else if (MOTO_STATE==MOTO_START)
//							UartSendStr("MOTO_START !");  
//					else if (MOTO_STATE==MOTO_PASUE)
//							UartSendStr("MOTO_PASUE !");  
//					else if (MOTO_STATE==MOTO_ERROR)
//							UartSendStr("MOTO_ERROR !");  
//					else if (MOTO_STATE==LID_UP)
//							UartSendStr("LID_UP !");  
//					else
//							UartSendStr("wrong  state!");  
						
					
//						BELL=!BELL;
						LED2		=!LED2;//	�����ƣ���һ����   
						os_wait (K_IVL, 200, 0);          /* �ȴ�1���ӣ�ע���������߳�Ӱ�죬ʱ�䲢����ȷ       */
  }
}

/******************************************************************************/
/*        Task 3 'COUNTER': ���ת�ٲ�������CLOCKÿ�봥��һ��   */
/******************************************************************************/
//void counter (void) _task_ COUNTER  
//{
//UINT16 ttemp;
//UINT32 temp;
//  while (1)  {    
//			os_wait (K_SIG, 0, 0);            /* wait for signal                      */		
//			if(length==0)
//			{
//				UartSendStr("Moto Work Error");
//			}else{
////						
////				UartSendByte(0xFF);
////				UartSendByte(length>>16);
////				UartSendByte(length>>8);
////				UartSendByte(length);
////				��������������ʱ����12000=12����������Ϊһ����Ƭ������*1000 ��λ����
//				//length������ȣ���������
//				moto_real_time=length/12;			//
//				UartSendByte(moto_real_time>>16);
//				UartSendByte(moto_real_time>>8);
//				UartSendByte(moto_real_time);
//				
//				temp=60000000/moto_real_time;	
////				temp=100000/moto_real_time;	//60000000/moto_real_time;				//  ת�����ٶ�/��
//				MOTOR_SPEED=temp;									//16λ�������
//				UartSendByte(temp);
//				UartSendByte(temp>>8);
//				
//				Display_Time1();
//			}
//  }
//}
/******************************************************************************/
/*      Task 4 'moto': ���PWM�̣߳�����PCA����ģ�� */
/******************************************************************************/
void moto (void) _task_ MOTO  
{            /* moto pwm output operation     */
  while (1)  {                    
//			counter0++;                            
//  		UartSendByte(counter0);
			if(!LID_UP)
			{
					MOTO_STATE=MOTO_PASUE;			
					UartSendStr("_PASUE !");  
			}
			
			
			if(MOTO_STATE==MOTO_START)
			{
					Pwm_Start();
			}else if(MOTO_STATE==MOTO_RUN){
					Pwm_Run();
			}else if(MOTO_STATE==MOTO_PASUE){
					Pwm_Stop();
			}else if(MOTO_STATE==MOTO_STOP){
					Pwm_Stop();
			}else if(MOTO_STATE==MOTO_ERROR){  
					BEEP(1,200);	
					Pwm_Stop();
			}else
			{
			}
//					UartSendStr("PWM Started!"); 
			os_wait (K_IVL, 20, 0);			//100ms
		}			
}
/******************************************************************************/
/*        Task 5 'keyread': �����̣߳�30ʱ��150MS��������   */
/******************************************************************************/
void keyread (void) _task_ KEYREAD  
{
		while (1)  {                     
			KeyGet();
			os_wait (K_TMO, 30, 0);                    
		}
}
/******************************************************************************/
/*        Task 5 'keyread': process key stroke from pedestrian push button    */
/******************************************************************************/
void 	display (void) _task_ DISPLAY 
{
		while (1)  
		{      
				Diaplay_SUB();
				os_wait (K_TMO, 200, 0);  
		}
}