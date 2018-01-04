#include "SmartM_8051.h"
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>

//按键状态机
#define KEY_PORT            P3     //按键IO
#define KEY_PORT_MASK       0x3C   //按键掩码

#define KEY_SEARCH_STATUS   0      //查询按键状态
#define KEY_ACK_STATUS      1      //确认按键状态
#define KEY_REALEASE_STATUS 2      //释放按键状态

//设备工作状态机
#define MOTO_STOP   0
#define MOTO_RUN    1
#define MOTO_PASUE  2
#define LID_UP      3

UINT8  MOTO_STATE;


#define TIME_10MS           (65536-FOSC/1000)

//测试地址
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
*函数名称:KeyInit
*输    入:无
*输    出:无
*功    能:按键初始化
******************************************/
VOID KeyInit(VOID)
{
     P3M1|=  (1<<2)|(1<<3)|(1<<4)|(1<<5);
		 P3M0&=~((1<<2)|(1<<3)|(1<<4)|(1<<5));
}
/****************************************
*函数名称:LcdKey1
*输    入:无
*输    出:无
*功    能:虚拟按键1
******************************************/
VOID LcdKey1(VOID)
{
	 LcdFill(20,50,70,100,BLACK);     //KEY1按下

	 MOTO_STATE=MOTO_RUN;	
//		speed_high=IapReadByte(TIME_HIGH_ADDRESS);  		// 读取存贮时间数据的高八位;
//		speed_low=IapReadByte(TIME_LOW_ADDRESS);				// 读取存贮时间数据的低八位;
//    Delay(10);     
//		
//		RUN_TIME=(speed_high<<8)|(speed_low);			//合并成16位数据格式
}
/****************************************
*函数名称:LcdKey2
*输    入:无
*输    出:无
*功    能:虚拟按键2
******************************************/
VOID LcdKey2(VOID)
{
	 LcdFill(180,50,230,100,BLACK);   //KEY2按下
	 
	 MOTO_STATE=MOTO_STOP;	
//	 
//	  speed_high=(UINT8)(RUN_TIME>>8);
//		speed_low=(UINT8)(RUN_TIME);
//		

}
/****************************************
*函数名称:LcdKey3
*输    入:无
*输    出:无
*功    能: 速度增加
******************************************/
VOID LcdKey3(VOID)
{
	 LcdFill(20,150,70,200,BLACK);    //KEY3按下
		
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

//		IapEraseSector(TIME_HIGH_ADDRESS);    //扇区擦除
//    Delay(10);     //擦除扇区需要点延时，保证完成最好检测下返回值是否FF;
//    IapEraseSector(TIME_LOW_ADDRESS);    //扇区擦除
//    Delay(10);   
//		
//		IapProgramByte(TIME_HIGH_ADDRESS,time_high);			// 写入存贮时间数据的高八位;
//		IapProgramByte(TIME_LOW_ADDRESS,time_low);				// 写入存贮时间数据的低八位;
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

	
		//非运行状态下数据写入FLASH
		if(MOTO_STATE!=MOTO_RUN)
		{
	      speed_high=(UINT8)(MOTOR_SPEED>>8);
				speed_low=(UINT8)MOTOR_SPEED;
				Delay(10); 		
				UartSendByte(speed_high);
				Delay(10);     
				UartSendByte(speed_low); 
				Delay(10); 

				IapEraseSector(SPEED_HIGH_ADDRESS);    //扇区擦除
//				Delay(10);     //擦除扇区需要点延时，保证完成最好检测下返回值是否FF;
				IapEraseSector(SPEED_LOW_ADDRESS);    //扇区擦除
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
*函数名称:LcdKey4
*输    入:无
*输    出:无
*功    能:速度减少
******************************************/
VOID LcdKey4(VOID)
{
	  LcdFill(180,150,230,200,BLACK);  //KEY4按下	
		
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

//		IapEraseSector(TIME_HIGH_ADDRESS);    //扇区擦除
//    Delay(10);     //擦除扇区需要点延时，保证完成最好检测下返回值是否FF;
//    IapEraseSector(TIME_LOW_ADDRESS);    //扇区擦除
//    Delay(10);   
//		
//		IapProgramByte(TIME_HIGH_ADDRESS,time_high);			// 写入存贮时间数据的高八位;
//		IapProgramByte(TIME_LOW_ADDRESS,time_low);				// 写入存贮时间数据的低八位;
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
		
	
		//非运行状态下数据写入FLASH
		if(MOTO_STATE!=MOTO_RUN)
		{
	      speed_high=(UINT8)(MOTOR_SPEED>>8);
				speed_low=(UINT8)MOTOR_SPEED;
				Delay(10); 		
				UartSendByte(speed_high);
				Delay(10);     
				UartSendByte(speed_low); 
				Delay(10); 

				IapEraseSector(SPEED_HIGH_ADDRESS);    //扇区擦除
//				Delay(10);     //擦除扇区需要点延时，保证完成最好检测下返回值是否FF;
				IapEraseSector(SPEED_LOW_ADDRESS);    //扇区擦除
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
*函数名称:SECOND_MINUTE
*输    入:无
*输    出:无
*功    能:秒数转为分秒格式
******************************************/
VOID SECOND_MINUTE(UINT16 SEC)
{
		MINUTE=SEC/60;
		SECOND=SEC%60;	
	
}
/****************************************
*函数名称:Data_Moto_Speed_Init
*输    入:无
*输    出:无
*功    能:
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
		
		time_high=(IapReadByte(TIME_HIGH_ADDRESS));
		time_low=(IapReadByte(TIME_LOW_ADDRESS));
		speed_high=(IapReadByte(SPEED_HIGH_ADDRESS));
		speed_low=(IapReadByte(SPEED_LOW_ADDRESS));
		
		
		RUN_TIME=(time_high<<8)|(time_low);			//合并成16位数据格式
		MOTOR_SPEED=(speed_high<<8)|(speed_low);		
//		
		


    Delay(100);   

	
}
/****************************************
*函数名称:LcdKeyRst
*输    入:无
*输    出:无
*功    能:虚拟按键恢复为释放状态
******************************************/
VOID LcdKeyRst(VOID)
{
	 //时间初始化显示
					
	 LcdFill(20,50,70,100,RED);             //KEY1
	 LcdFill(180,50,230,100,RED);           //KEY2
	 LcdFill(20,150,70,200,RED);            //KEY3
	 LcdFill(180,150,230,200,RED);		      //KEY4

	 LcdShowString(35,70, "K1",YELLOW,RED); //填充'1'
	 LcdShowString(195,70,"K2",YELLOW,RED); //填充'2'
	 LcdShowString(35,170,"K3",YELLOW,RED); //填充'3'
	 LcdShowString(195,170,"K4",YELLOW,RED);//填充'4'
	 

	 LcdFill(20,210,70,250,BLUE);             //KEY1	 
	 LcdShowString(25,215, "时间:",YELLOW,BLUE);
	 
	 LcdFill(20,260,70,300,BLUE); 
	 LcdShowString(25,265,"速度:",YELLOW,BLUE); 
//	 sprintf(g_szTempBuf,"%04d",MOTOR_SPEED);
	 LcdShowString(75,265,g_szTempBuf,BLUE,WHITE); 

}

/****************************************
*函数名称:Timer1Init
*输    入:无
*输    出:无
*功    能:定时器1初始化 
******************************************/
VOID Timer1Init(VOID)
{
//     AUXR|=0x40;
//		 TL1=(UINT8) TIME_10MS;
//		 TH1=(UINT8)(TIME_10MS>>8);
//		 TMOD|=0x10;
//		 TR1=1;

			AUXR |= 0x40;		//定时器时钟1T模式
			TMOD &= 0x0F;		//设置定时器模式
			TL1 = 0x40;		//设置定时初值
			TH1 = 0xA2;		//设置定时初值
			TF1 = 0;		//清除TF1标志
			TR1 = 1;		//定时器1开始计时

			ET1=1;
}



/****************************************
*函数名称:KeyRead
*输    入:无
*输    出:无
*功    能:按键值读取
******************************************/
VOID KeyRead(VOID)
{
	  STATIC UINT8 ucKeyStatus=KEY_SEARCH_STATUS;
           UINT8 ucKeyValue=0;
           UINT8 i=0;
	  
    /*   获取按键值 */
	  ucKeyValue=(~KEY_PORT)&KEY_PORT_MASK;	  
  
    switch(ucKeyStatus)
    {
			/* 按键查询状态 */
			case KEY_SEARCH_STATUS:
					{
							 if(ucKeyValue)
							 {
									ucKeyStatus=KEY_ACK_STATUS;
							 }

					}break;
					
      /* 按键确认状态 */
			case KEY_ACK_STATUS:   
					{
							 if(!ucKeyValue) //没有按键按下
							 {
									ucKeyStatus=KEY_SEARCH_STATUS;
							 }
							 else//有按键按下
							 {	

								  if(ucKeyValue & (1<<2))LcdKey1();
								  if(ucKeyValue & (1<<3))LcdKey2();
								  if(ucKeyValue & (1<<4))LcdKey3();
								  if(ucKeyValue & (1<<5))LcdKey4();

							 }

							 ucKeyStatus=KEY_REALEASE_STATUS;
							 
							 
					}break;
      /* 按键释放状态 */
			case KEY_REALEASE_STATUS:
					{
							 if(!ucKeyValue)   //按键释放
							 {
								   ucKeyStatus=KEY_SEARCH_STATUS;
								 
                   LcdKeyRst();              							 
							 }
					 
					}break;

					default:break;
    }
}

/****************************************
*函数名称:main
*输    入:无
*输    出:无
*功    能:函数主体
******************************************/
VOID main(VOID)
{


   UartInit(9600);	
		
		
	 /*   LCD 初始化 */
	 LcdTypeSet(LCD_TYPE_32);
	 LcdInit(LCD_FONT_IN_FLASH,LCD_DIRECTION_180);                                  
	 LcdCleanScreen(WHITE);     

	 
   /*   LED 初始化 */
	 LedInit();

	 /*   SPI FLash 初始化 */
	 SpiFlashInit();  
	 
	 MOTO_STATE=0;
	 RUN_TIME=820;				
	 MOTOR_SPEED=900;
	 RUN_TIME_COUNTER=0;
	 MINUTE=0;
	 SECOND=0;
	 MOTO_STATE=MOTO_STOP;


		LcdShowString(80,10,"研磨机控制程序",BLUE,WHITE);
		LcdShowString(100,30,"V2.0 ",BLUE,WHITE);


	 /*   按键 初始化 */
	 KeyInit();
	 LcdKeyRst();
	 
	 Data_Moto_Speed_Init();	
	 
	 /*   定时器 初始化 */
	 Timer1Init(); 
	 
	 IRQ_ENABLE();				

	 while(1)
	 {	

				 
				 if(g_bTimer1IRQ)
				{
							g_bTimer1IRQ=FALSE;
							
							/* 按键读取 */
							KeyRead();		 
											
							sprintf(g_szTempBuf,"%04d",RUN_TIME);				//sprintf只能对双字节UINT16进行操作
							LcdShowString(100,120,g_szTempBuf,BLUE,WHITE);
						
							SECOND_MINUTE(RUN_TIME);									//将数据格式转换为分:秒 格式
							sprintf(g_szTempBuf,"%02d",MINUTE);				//sprintf只能对双字节UINT16进行操作
							LcdShowString(75,215,g_szTempBuf,BLUE,WHITE);
							sprintf(g_szTempBuf,":%02d",SECOND);
							LcdShowString(107,215,g_szTempBuf,BLUE,WHITE);
							
							sprintf(g_szTempBuf,"%04d",MOTOR_SPEED);
							LcdShowString(75,265,g_szTempBuf,BLUE,WHITE); 
				}
			 
	 }


}

/****************************************
*函数名称:TMR1_IRQHandler
*输    入:无
*输    出:无
*功    能:定时器1中断服务函数
******************************************/
VOID Timer1_ISR(VOID) interrupt 3 using 1
{
    g_bTimer1IRQ=TRUE;

//	TL1=(UINT8) TIME_10MS;
//	TH1=(UINT8)(TIME_10MS>>8);

		/* 定时器1重装载 */
		TL1 = 0x40;		//设置定时初值
		TH1 = 0xA2;		//设置定时初值

		
		if(MOTO_STATE==MOTO_RUN)				//非运行状态下 才允许调整时间
		{
				if(RUN_TIME_COUNTER>=1000)		//1秒计时
				{
						 RUN_TIME_COUNTER=0;
						 RUN_TIME--;
				}
				
				 RUN_TIME_COUNTER++;
		}
}
