#include "SmartM_8051.h"


VOID PWMInit(UINT8 ucPwmClock);
VOID Pwm_Run();
VOID Pwm_Stop();
VOID Pwm_Start();
void BEEP(UINT8 cycle,UINT8 beep);
extern void Display_Time1();
#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5

#define Speed_Max  1750
#define Speed_Min  500
#define Duty_Max  65535
#define Duty_Min 	1000
//#define SS   			(Duty_Max-Duty_Min)//(Speed_Max/10-Speed_Min/10)
//#define Duty      (MOTOR_SPEED-Speed_Min)/10*SS		///注意运算顺序 防止溢出 +Duty_Min


//UINT8 g_ucPwmValue=0;
UINT16 ddDuty,Duty,SS;
UINT32 moto_real_time;


//计数，测量速度用变量
UINT16 cnt;                           //存储PCA计时溢出次数
UINT32 count0;                       //记录上一次的捕获值
UINT32 count1;                       //记录本次的捕获值
UINT32 length;                       //存储捕获到的脉冲宽度(count1 - count0)
UINT32 speed_temp;										//电机实测速度

//电机启动参数 start_step总步长，
UINT8 start_count=0,start_step=30;
UINT8 Bell_count=0;

UINT8  CC[6];
extern UINT8  MOTO_STATE;
extern UINT16 RUN_TIME,MOTOR_SPEED;
extern void WaitNms(UINT16 n);
/****************************************
*函数名称:BEEP
*输    入:蜂鸣器报警
*输    出:无
*功    能:
******************************************/
void BEEP(UINT8 cycle,UINT8 beep)
{
		while(cycle>0)
		{
				Bell_count=0;
				while(Bell_count<beep)
				{
//						Bell_Run;
						BELL=1;	
						WaitNms(10);
						Bell_count++;
				}
				Bell_count=0;
				while(Bell_count<beep)
				{
//						Bell_Stop;
						BELL=0;		
						WaitNms(10);	
						Bell_count++;
				}
				cycle--;
		}
}
/****************************************
*函数名称:PWMClock
*输    入:时钟
*输    出:无
*功    能:初始化时钟参数，指定PWM输出端口
******************************************/
VOID PWMInit(UINT8 ucPwmClock)
{
     //CCP_S1=0 CCP_S0=1 CCP0输出到P3.5
//		 P_SW1|=(1<<4);
//		 P_SW1&=~(1<<5);

//     P3M0|=1<<5;
//		 P3M1&=~(1<<5);
	
	//PWM输出到P1.1
    ACC = P_SW1;
    ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
    P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=1 CCP_S1=0
//  ACC |= CCP_S0;                  //(P3.4/ECI_2, P3.5/CCP0_2, P3.6/CCP1_2, P3.7/CCP2_2)
//  P_SW1 = ACC;  
//  
//  ACC = P_SW1;
//  ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=1
//  ACC |= CCP_S1;                  //(P2.4/ECI_3, P2.5/CCP0_3, P2.6/CCP1_3, P2.7/CCP2_3)
//  P_SW1 = ACC;  
//		PCA_IDLE_OFF();

     CMOD=ucPwmClock;			// 计数脉冲源=sysclk, 0X00 不分频 允许PCA定时器溢出中断
		 //PCA的16位计数器
		 CL=0;
		 CH=0;
		 
		 SS=(Duty_Max-Duty_Min)/(Speed_Max/10-Speed_Min/10);
}

VOID PWMStart(UINT8 ucPwmSel,UINT8 ucPwmMode,UINT16 ucDuty)
{
     UINT16 usCMR=0;
//		 float  fDuty;
		 //		 fDuty=65536*fDuty;
//		 
//		 UINT16 DATA_MAX=65535,DATA_MIN=6555;
//		 UINT16 SPEED_MAX=1750,SPEED_MIN=500;
//		 
//		 A=(DATA_MAX-DATA_MIN)/(SPEED_MAX-SPEED_MIN);
//		 B=SPEED_MAX*A-DATA_MAX;
//		 fDuty=A*ucDuty-B;
		 
//		 fDuty=(float)ucDuty/100;
//		 
//		 fDuty=65536*fDuty;
//		 
//		 usCMR=(UINT16)fDuty;
//		原输入值为百分比，改为数值 ，最大65536 	 
	
		 usCMR=65535-ucDuty;
     if(ucPwmSel == PWM_SEL_0)
		 {
		    CCAP0L=(UINT8) usCMR;
				CCAP0H=(UINT8)(usCMR>>8);
				
				switch(ucPwmMode)
		    {
				 case PWM_MODE_0:CCAPM0=0x42;			// 模块0工作在PWM工作模式
					CCAPM1 = 0x21;  //PCA模块1为16位捕获模式(上升沿捕获,可测从高电平开始的整个周期),且产生捕获中断	  					 
					PCA_PWM2 = 0x40;                //PCA模块2工作于6位PWM
					CCAP2H = CCAP2L = 0x20;         //PWM2的占空比为50% ((40H-20H)/40H)
					CCAPM2 = 0x42;                  //PCA模块2为6位PWM模式

				      break;
				 case PWM_MODE_1:CCAPM0=0x53;
				      break;		
				 case PWM_MODE_2:CCAPM0=0x63;
				      break;
				 case PWM_MODE_3:CCAPM0=0x33;
				      break;			
         default:break;												 
		 
		    }
		 }
		 
//     if(ucPwmSel == PWM_SEL_1)
//		 {
//		    CCAP1L=(UINT8) usCMR;
//				CCAP1H=(UINT8)(usCMR>>8);
//				
//				switch(ucPwmMode)
//		    {
//				 case PWM_MODE_0:CCAPM1=0x42;
//				                 break;
//				 case PWM_MODE_1:CCAPM1=0x53;
//				                 break;		
//				 case PWM_MODE_2:CCAPM1=0x63;
//				                 break;
//				 case PWM_MODE_3:CCAPM1=0x33;
//				                 break;			
//         default:break;												 
//		 
//		    }				
//		 }
		 //启动计数器
		 CR=1;
}


/****************************************
*函数名称:Pwm_Stop
*输    入:无
*输    出:无
*功    能:Lcd屏获取触摸位置
******************************************/
VOID Pwm_Stop()
{
	 MOTOR_DISABLE	;	
	 CR=0;				

//			UartSendStr("Pwm_Stop"); 
}

/****************************************
*函数名称:Pwm_Start
*输    入:无
*输    出:无
*功    能:启动电机  按预估PWM值逐步启动，start_step步长
******************************************/
VOID Pwm_Start()
{
		MOTOR_ENABLE ;			//使能电机
		if(start_count<1)
		{
				BEEP(2,50);		
		}
	
	  if(start_count<start_step)
		{
//				ddDuty=(MOTOR_SPEED-Speed_Min)/10*start_count/start_step*SS+Duty_Min;///注意运算顺序 防止溢出
		    Duty=(MOTOR_SPEED-Speed_Min)/10*SS;	///注意运算顺序 防止溢出 +Duty_Min		 
			ddDuty=Duty/start_step*start_count+Duty_Min;
			PWMStart(PWM_SEL_0,PWM_MODE_0,ddDuty);		//PCA模块0为8位PWM模式
			start_count++;
//			UartSendByte(0xBB);
//			UartSendByte(start_count);
//			UartSendByte(Duty>>8);
//			UartSendByte(Duty);
//			UartSendByte(ddDuty>>8);
//			UartSendByte(ddDuty);
			
//				sprintf(CC, "%d", MOTOR_SPEED); 
//				UartSendStr("MOTOR_SPEED=");//
//				UartSendStr(CC); 
//				sprintf(CC, "%u", ddDuty); 
//				UartSendStr("Duty=");//
//				UartSendStr(CC); 
		}else{
				start_count=0;
				MOTO_STATE=MOTO_RUN;
				UartSendStr("MOTO_RUN!!!");
		}
		
//			PWMStart(PWM_SEL_1,PWM_MODE_0,ddDuty);		//PCA模块1为8位PWM模式
//			UartSendStr("Run"); 
}
/****************************************
*函数名称:Pwm_Run
*输    入:无
*输    出:无
*功    能:
******************************************/
VOID Pwm_Run()
{
				UartSendByte(0xEE);
				UartSendByte(length>>24);
				UartSendByte(length>>16);
				UartSendByte(length>>8);
				UartSendByte(length);
		//测量当前电机速度
			if(length>0x3F000)
			{
				UartSendStr("Moto Work Error");
				Pwm_Stop();
				MOTO_STATE=MOTO_ERROR;
				Display_Time1();
			}else{
//						
//				UartSendByte(0xFF);
//				UartSendByte(length>>16);
//				UartSendByte(length>>8);
//				UartSendByte(length);

				
//				UartSendByte(moto_real_time>>16);
//				UartSendByte(moto_real_time>>8);
//				UartSendByte(moto_real_time);
				
					//转换为单片机时长（12个晶振周期为一个单片机周期）*
					moto_real_time=length/12;				//主频24M，length脉冲宽度，计数个数	
					//计算电机工作周期时长，单位*1000转换为毫秒，*1000秒，*60分		
					//1000*1000*60/600
					speed_temp=60000000/moto_real_time;	
					//speed_temp=100000/moto_real_time;				//  除编码器精度600，转换成速度/分
//					UartSendByte(speed_temp);
//					UartSendByte(speed_temp>>8);
			
//					MOTOR_SPEED=speed_temp;									//电机速度送出到液晶，调试用	
//				  Display_Time1();
					
					
				//速度误差大于 重新调整
					if(abs(MOTOR_SPEED-speed_temp)>5)
					{
								/* PWM输出调整 */
								if(MOTOR_SPEED>speed_temp)
								{
										ddDuty=ddDuty-300;
//										UartSendByte(0xEE);
								}else{
										ddDuty=ddDuty+300;
//										UartSendByte(0xDD);
								}
//										UartSendByte(ddDuty>>8);
//										UartSendByte(ddDuty);
						
								if((ddDuty>Duty_Max)||(ddDuty<Duty_Min))
								{
										Pwm_Stop();
										MOTO_STATE=MOTO_ERROR;
										UartSendStr("PWM MOTO_ERROR!");
										Display_Time1();
								}else{
									 PWMStart(PWM_SEL_0,PWM_MODE_0,ddDuty);		//PCA模块0为8位PWM模式
								}
					}
			}
	
			
//		sprintf(CC, "%d", MOTOR_SPEED); 
//		UartSendStr("MOTOR_SPEED=");//
//		UartSendStr(CC); 
//		sprintf(CC, "%u", ddDuty); 
//		UartSendStr("Duty=");//
//		UartSendStr(CC); 
		
//			PWMStart(PWM_SEL_1,PWM_MODE_0,ddDuty);		//PCA模块1为8位PWM模式
//			UartSendStr("Run"); 
}

//*******************************//
//           中断服务            //
//*******************************//
VOID PWM_IRQHandler(VOID) interrupt 7
{
    if(CCF0)
		{
		   CCF0   = 0; 
		}

    if (CCF1)
    {
//				SendData(0x11);
//				SendData(0x22);
        CCF1 = 0;
        if (CF && ((CCAP1H & 0x80) == 0))
        {
            CF = 0;
            cnt++;
        }
        count0 = count1;            //备份上一次的捕获值
        ((BYTE *)&count1)[3] = CCAP1L;  //保存本次的捕获值
        ((BYTE *)&count1)[2] = CCAP1H;
        ((BYTE *)&count1)[1] = cnt;
        ((BYTE *)&count1)[0] = 0;
        length = count1 - count0;   //计算两次捕获的差值,即得到时间长度
        ((BYTE *)&length)[0] = 0;
				

//				UartSendByte(0x00);	
//				UartSendByte(cnt);
//					UartSendByte(CCAP1H);	
//					UartSendByte(CCAP1L);
//				UartSendByte(length>>16);
//				UartSendByte(length>>8);
//				UartSendByte(length);

    }
    if (CF)
    {
        CF = 0;
        cnt++;                      //PCA计时溢出次数+1
    }
}
