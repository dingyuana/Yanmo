/** * 
*----------Dragon be here!----------/ 
* 　　　┏┓　　　┏┓ 
* 　　┏┛┻━━━┛┻┓ 
* 　　┃　　　　　　　┃ 
* 　　┃　　　━　　　┃ 
* 　　┃　┳┛　┗┳　┃ 
* 　　┃　　　　　　　┃ 
* 　　┃　　　┻　　　┃ 
* 　　┃　　　　　　　┃ 
* 　　┗━┓　　　┏━┛ 
* 　　　　┃　　　┃神兽保佑 
* 　　　　┃　　　┃代码无BUG！ 
* 　　　　┃　　　┗━━━┓ 
* 　　　　┃　　　　　　　┣┓ 
* 　　　　┃　　　　　　　┏┛ 
* 　　　　┗┓┓┏━┳┓┏┛ 
* 　　　　　┃┫┫　┃┫┫ 
* 　　　　　┗┻┛　┗┻┛ 
* ━━━━━━神兽出没━━━━━━by:dingyuan */

#include "SmartM_8051.h"
#include <rtx51tny.h>                 /* RTX-51 tiny functions & defines      */



//#define KEY_PORT            P1     //按键IO
//#define KEY_PORT_MASK       0xFC   //按键掩码，使用2.3.4.5.6.7六个管脚
#define KEY_SEARCH_STATUS   0      //查询按键状态
#define KEY_ACK_STATUS      1      //确认按键状态
#define KEY_REALEASE_STATUS 2      //释放按键状态


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
*函数名称:Data_Moto_Speed_Init
*输    入:无
*输    出:无
*功    能:电机工作数据初始化，从FLAH中读出速度和工作时间参数
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
		
//	//第一次写芯片时用于写入EEPROM时间数据 	写入EEPROM速度数据			 	
//	IapEraseSector(TIME_HIGH_ADDRESS);    //扇区擦除
//	IapEraseSector(TIME_LOW_ADDRESS);    //扇区擦除
//	IapEraseSector(SPEED_HIGH_ADDRESS);    //扇区擦除
//	IapEraseSector(SPEED_LOW_ADDRESS);    //扇区擦除
//	Delay(10);  //擦除扇区需要点延时，保证完成最好检测下返回值是否FF;
//	IapProgramByte(TIME_HIGH_ADDRESS, time_high);			// 写入存贮时间数据的高八位;
//	IapProgramByte(TIME_LOW_ADDRESS, time_low);				// 读取存贮时间数据的低八位;
//	IapProgramByte(SPEED_HIGH_ADDRESS,speed_high );
//	IapProgramByte(SPEED_LOW_ADDRESS,speed_low );
//	Delay(10);  
//		
		time_high=(IapReadByte(TIME_HIGH_ADDRESS));
		time_low=(IapReadByte(TIME_LOW_ADDRESS));
		speed_high=(IapReadByte(SPEED_HIGH_ADDRESS));
		speed_low=(IapReadByte(SPEED_LOW_ADDRESS));
		
		
		RUN_TIME=(time_high<<8)|(time_low);			//合并成16位数据格式
		RUN_TIME_TEMP=RUN_TIME;
		MOTOR_SPEED=(speed_high<<8)|(speed_low);	
//		MOTOR_SPEED=1620;		

    Delay(100);   	
}

/******************************************************************************/
/*        Task 0 'init': Initialize   初始化后 消灭本线程                      */
/******************************************************************************/
void init (void) _task_ INIT     /* program execution starts here        */
{    
//		 LED1=1;
	     LED2=1;
//		 Bell_Run;
		 BELL=1;
		 MOTOR_DISABLE;
   
		 UartInit (9600);                     /* 初始化PWM时钟，指定管脚输出     */	
		 PWMInit(SYSCLK_DIV_2);
		 KeyInit();					//按键初始化 P3？？？？
		 LCDInit();	 				//192 64 串口液晶初始化
		 Timer3Init();			//计数器初始化
	
//				sprintf(CCS, "%u", TL3); 
//				UartSendStr("TL3:");
//				UartSendStr(CCS);	
//		 IRQ_ENABLE();
	
//			RUN_TIME=820;				//	调试用
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
/*        Task 2 'clock'      基本时钟线程 ，基本时间片5MS                      */
/******************************************************************************/
void clock (void)  _task_ CLOCK  
{
  while (1)  
		{                        /* 时钟显示             */
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
								RUN_TIME--;				//	倒计时
//								BELL=!BELL;
//								os_send_signal (COUNTER);  
					}

					
					if(RUN_TIME<=0)	//	倒计时归零，恢复待机状态
					{
						MOTO_STATE=MOTO_STOP;		//状态机恢复停机状态
						RUN_TIME=RUN_TIME_TEMP;	//时间恢复初始
						Display_Time1();				//刷新液晶，恢复初始
						
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
						LED2		=!LED2;//	呼吸灯，不一定用   
						os_wait (K_IVL, 200, 0);          /* 等待1秒钟，注意受其它线程影响，时间并不精确       */
  }
}

/******************************************************************************/
/*        Task 3 'COUNTER': 电机转速测量，由CLOCK每秒触发一次   */
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
////				计算电机工作周期时长，12000=12个晶振周期为一个单片机周期*1000 单位毫秒
//				//length脉冲宽度，计数个数
//				moto_real_time=length/12;			//
//				UartSendByte(moto_real_time>>16);
//				UartSendByte(moto_real_time>>8);
//				UartSendByte(moto_real_time);
//				
//				temp=60000000/moto_real_time;	
////				temp=100000/moto_real_time;	//60000000/moto_real_time;				//  转换成速度/分
//				MOTOR_SPEED=temp;									//16位方便输出
//				UartSendByte(temp);
//				UartSendByte(temp>>8);
//				
//				Display_Time1();
//			}
//  }
//}
/******************************************************************************/
/*      Task 4 'moto': 电机PWM线程，利用PCA功能模块 */
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
/*        Task 5 'keyread': 按键线程，30时间150MS基本合适   */
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