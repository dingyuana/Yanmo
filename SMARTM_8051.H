#ifndef __SMARTM_8051_H__
#define __SMARTM_8051_H__
#include <stdio.h>
#include <stdlib.h>
#include "8051.h"
#include <intrins.h>
#include "Typedef.h"
#include "Common.h"
#include "Key.h"
#include "EEPROM.h"
#include "UART.h"
//#include "delay.h"
//#include "LCD.h"
#include "PWM.h"


//#include "spi.h"
//#include "flash.h"

//管脚定义  
//速度编码器 P1.0
//PWM输出    P1.1


#define KEY_PORT            P1     //按键IO
#define KEY_PORT_MASK       0xFC   //按键掩码，使用2.3.4.5.6.7六个管脚

//sbit	 LED1=P5^5;//P2^6;
sbit	 LED2=P5^4;
sbit	 BELL=P2^6;							//蜂鸣器
sbit	 MOTO_KEY=P2^7;			//电机使能，过滤单片机上电时管脚高电压导致电机冲击
sbit	 LID_UP=P3^3;							//门检测开关  低电平报警
//sbit   TIME_UP=P1^2;
//sbit   TIME_DOWN=P1^3;


sbit PSB=P2^0;//并口时，PSB=1;串口时，PSB=0
sbit E2=P2^1; 		//串口为时钟SCLK
sbit E1=P2^2;  		//串口为时钟SCLK
sbit RW=P2^3;		 	//串口为SID
sbit RS=P2^4;			//串口时为CS
sbit REST=P2^5;

#define MOTOR_ENABLE  MOTO_KEY=1;
#define MOTOR_DISABLE MOTO_KEY=0;
#define Bell_Run 			BELL=1;
#define Bell_Stop			BELL=0;

//EEPROM地址
#define IAP_ADDRESS 0xF000
#define TIME_HIGH_ADDRESS  IAP_ADDRESS
#define TIME_LOW_ADDRESS  IAP_ADDRESS+2
//#define SPEED_HIGH_ADDRESS  IAP_ADDRESS+0x200   //0xF200
//#define SPEED_LOW_ADDRESS   IAP_ADDRESS+0x202  	//SPEED_HIGH_ADDRESS+2
#define SPEED_HIGH_ADDRESS  IAP_ADDRESS+4   //0xF200
#define SPEED_LOW_ADDRESS   IAP_ADDRESS+6  	//SPEED_HIGH_ADDRESS+2

//设备工作状态机
#define MOTO_STOP   0
#define MOTO_RUN    1
#define MOTO_PASUE  2
#define LID_UP      3
#define MOTO_START  4
#define MOTO_ERROR  5
//static UINT8 MOTO_STOP=0;
//static UINT8 MOTO_RUN=1;
//static UINT8 MOTO_PASUE=2;
//static UINT8 MOTO_START=3;
//static UINT8 LID_UP=4;

#define INIT      0                   /* task number of task:  init           */
#define DISPLAY   1                   /* task number of task:  command        */
#define CLOCK     2                   /* task number of task:  clock          */
#define COUNTER   3                   /* task number of task:  counter       */
#define MOTO    4                   /* task number of task:  signal         */
#define KEYREAD   5                   /* task number of task:  keyread        */
#define GET_ESC   6                   /* task number of task:  get_escape     */

//PWM相关变量

#define SYSCLK_DIV_1  0x09     				//允许CF位的中断
#define SYSCLK_DIV_2  0x03
#define SYSCLK_DIV_4  0x0A
#define SYSCLK_DIV_6  0x0D
#define SYSCLK_DIV_8  0x0E
#define SYSCLK_DIV_12 0x00



struct time  {                        /* structure of the time record         */
  UINT8 hour;                 /* hour                                 */
  UINT8 min;                  /* minute                               */
  UINT8 sec;                  /* second                               */
};


#endif