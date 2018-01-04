#include "SmartM_8051.h"
extern VOID KeyGet(VOID);
extern void Pwm_Run();

//按键状态机

#define KEY_PRESS_LOW				0
#define KEY_PRESS_HIGH			1

UINT8 KEY_PRESS_STATE=KEY_PRESS_LOW;

//#define KEY_SEARCH_STATUS   0      //查询按键状态
//#define KEY_ACK_STATUS      1      //确认按键状态
//#define KEY_REALEASE_STATUS 2      //释放按键状态

#define Time_Up    		0x04
#define Time_Down  		0x08
#define Speed_Up   		0x10
#define Speed_Down 		0x20
#define Start_Button  0x40
#define Stop_Button   0x80

//用于检测按键的变量
UINT8 Trg;		
UINT8 Cont;
UINT i=0;				//连续按键计数
//UINT8  CC[4];


UINT8  MOTO_STATE;
UINT16 RUN_TIME,MOTOR_SPEED,RUN_TIME_TEMP;

UINT8 time_low,time_high;
UINT8 speed_low,speed_high;
//STATIC UINT8  g_szTempBuf[4]={0},g_szTemp[2];
/****************************************
*函数名称:Write_Data_Flash
*输    入:无
*输    出:无
*功    能:速度时间数据写入FLASH中
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

				IapEraseSector(TIME_HIGH_ADDRESS);    //扇区擦除
				IapEraseSector(TIME_LOW_ADDRESS);    //扇区擦除
				IapEraseSector(SPEED_HIGH_ADDRESS);    //扇区擦除需要点延时，保证完成最好检测下返回值是否FF;
				IapEraseSector(SPEED_LOW_ADDRESS);    //扇区擦除
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
*函数名称:KeyInit
*输    入:无
*输    出:无
*功    能:按键初始化
******************************************/
VOID KeyInit(VOID)
{
//     P3M1|=  (1<<2)|(1<<3)|(1<<4)|(1<<5);
//		 P3M0&=~((1<<2)|(1<<3)|(1<<4)|(1<<5));
     P1M1|=  (1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<8);
		 P1M0&=~((1<<2)|(1<<3)|(1<<4)|(1<<5)|(1<<6)|(1<<7));
}
/****************************************
*函数名称:Key_Run
*输    入:无
*输    出:无
*功    能:启动电机按键
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
*函数名称:Key_Stop
*输    入:无
*输    出:无
*功    能:停止电机按键
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
*函数名称:Key_SpeedUp
*输    入:无
*输    出:无
*功    能: 速度增加按键
******************************************/
VOID Key_SpeedUp(VOID)
{
//	 LcdFill(20,150,70,200,BLACK);    //KEY3按下
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
*函数名称:Key_Speed_Down
*输    入:无
*输    出:无
*功    能:速度减少按键
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
	
		//非运行状态下数据写入FLASH
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
*函数名称:Key_Time_Up
*输    入:无
*输    出:无
*功    能:时间增加按键
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
*函数名称:Key_Time_Down
*输    入:无
*输    出:无
*功    能:时间减少按键
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
*函数名称:KeyGet
*输    入:无
*输    出:无
*功    能:按键值读取
******************************************/
VOID KeyGet(VOID)
{
//	    STATIC UINT8 ucKeyStatus=KEY_SEARCH_STATUS;
      UINT8 ucKeyValue=0;
      UINT8 ReadData;// = P3^0xff; 。
			ucKeyValue=(~KEY_PORT)&KEY_PORT_MASK;		//
			ReadData= KEY_PORT^KEY_PORT_MASK;   // 1 读P1的端口数据，取反，然后送到ReadData 临时变量里面保存起来
      Trg = ReadData & (ReadData ^ Cont); // 2 算法1，用来计算触发变量的。一个位与操作，一个异或操作
      Cont = ReadData; // 3 算法2，用来计算连续变量。

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
				if (Cont )		//防抖动
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
			
			//定时功能按键,非运行状态下允许数据修改写入
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
//								if (Cont & Time_Up) 		//连续按键状态
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
						//时间,非运行状态下允许数据修改写入
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
									if (Cont )   //连续按键状态
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
						//时间,非运行状态下允许数据修改写入
						if(MOTO_STATE==MOTO_STOP)
						{		
									if (Trg)
									{
											KEY_PRESS_STATE=KEY_PRESS_LOW;
											i=0;
									}			
									if (Cont )   //连续按键状态
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
			

			//速度按键,运行状态下允许数据修改，但不写入			
			if(ucKeyValue & (1<<4))
			{

								if (Trg)
								{
										KEY_PRESS_STATE=KEY_PRESS_LOW;
										i=0;
								}			
								if (Cont )		//连续按键状态
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
									if (Cont )   //连续按键状态
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

