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
//#define Duty      (MOTOR_SPEED-Speed_Min)/10*SS		///ע������˳�� ��ֹ��� +Duty_Min


//UINT8 g_ucPwmValue=0;
UINT16 ddDuty,Duty,SS;
UINT32 moto_real_time;


//�����������ٶ��ñ���
UINT16 cnt;                           //�洢PCA��ʱ�������
UINT32 count0;                       //��¼��һ�εĲ���ֵ
UINT32 count1;                       //��¼���εĲ���ֵ
UINT32 length;                       //�洢���񵽵�������(count1 - count0)
UINT32 speed_temp;										//���ʵ���ٶ�

//����������� start_step�ܲ�����
UINT8 start_count=0,start_step=30;
UINT8 Bell_count=0;

UINT8  CC[6];
extern UINT8  MOTO_STATE;
extern UINT16 RUN_TIME,MOTOR_SPEED;
extern void WaitNms(UINT16 n);
/****************************************
*��������:BEEP
*��    ��:����������
*��    ��:��
*��    ��:
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
*��������:PWMClock
*��    ��:ʱ��
*��    ��:��
*��    ��:��ʼ��ʱ�Ӳ�����ָ��PWM����˿�
******************************************/
VOID PWMInit(UINT8 ucPwmClock)
{
     //CCP_S1=0 CCP_S0=1 CCP0�����P3.5
//		 P_SW1|=(1<<4);
//		 P_SW1&=~(1<<5);

//     P3M0|=1<<5;
//		 P3M1&=~(1<<5);
	
	//PWM�����P1.1
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

     CMOD=ucPwmClock;			// ��������Դ=sysclk, 0X00 ����Ƶ ����PCA��ʱ������ж�
		 //PCA��16λ������
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
//		ԭ����ֵΪ�ٷֱȣ���Ϊ��ֵ �����65536 	 
	
		 usCMR=65535-ucDuty;
     if(ucPwmSel == PWM_SEL_0)
		 {
		    CCAP0L=(UINT8) usCMR;
				CCAP0H=(UINT8)(usCMR>>8);
				
				switch(ucPwmMode)
		    {
				 case PWM_MODE_0:CCAPM0=0x42;			// ģ��0������PWM����ģʽ
					CCAPM1 = 0x21;  //PCAģ��1Ϊ16λ����ģʽ(�����ز���,�ɲ�Ӹߵ�ƽ��ʼ����������),�Ҳ��������ж�	  					 
					PCA_PWM2 = 0x40;                //PCAģ��2������6λPWM
					CCAP2H = CCAP2L = 0x20;         //PWM2��ռ�ձ�Ϊ50% ((40H-20H)/40H)
					CCAPM2 = 0x42;                  //PCAģ��2Ϊ6λPWMģʽ

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
		 //����������
		 CR=1;
}


/****************************************
*��������:Pwm_Stop
*��    ��:��
*��    ��:��
*��    ��:Lcd����ȡ����λ��
******************************************/
VOID Pwm_Stop()
{
	 MOTOR_DISABLE	;	
	 CR=0;				

//			UartSendStr("Pwm_Stop"); 
}

/****************************************
*��������:Pwm_Start
*��    ��:��
*��    ��:��
*��    ��:�������  ��Ԥ��PWMֵ��������start_step����
******************************************/
VOID Pwm_Start()
{
		MOTOR_ENABLE ;			//ʹ�ܵ��
		if(start_count<1)
		{
				BEEP(2,50);		
		}
	
	  if(start_count<start_step)
		{
//				ddDuty=(MOTOR_SPEED-Speed_Min)/10*start_count/start_step*SS+Duty_Min;///ע������˳�� ��ֹ���
		    Duty=(MOTOR_SPEED-Speed_Min)/10*SS;	///ע������˳�� ��ֹ��� +Duty_Min		 
			ddDuty=Duty/start_step*start_count+Duty_Min;
			PWMStart(PWM_SEL_0,PWM_MODE_0,ddDuty);		//PCAģ��0Ϊ8λPWMģʽ
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
		
//			PWMStart(PWM_SEL_1,PWM_MODE_0,ddDuty);		//PCAģ��1Ϊ8λPWMģʽ
//			UartSendStr("Run"); 
}
/****************************************
*��������:Pwm_Run
*��    ��:��
*��    ��:��
*��    ��:
******************************************/
VOID Pwm_Run()
{
				UartSendByte(0xEE);
				UartSendByte(length>>24);
				UartSendByte(length>>16);
				UartSendByte(length>>8);
				UartSendByte(length);
		//������ǰ����ٶ�
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
				
					//ת��Ϊ��Ƭ��ʱ����12����������Ϊһ����Ƭ�����ڣ�*
					moto_real_time=length/12;				//��Ƶ24M��length�����ȣ���������	
					//��������������ʱ������λ*1000ת��Ϊ���룬*1000�룬*60��		
					//1000*1000*60/600
					speed_temp=60000000/moto_real_time;	
					//speed_temp=100000/moto_real_time;				//  ������������600��ת�����ٶ�/��
//					UartSendByte(speed_temp);
//					UartSendByte(speed_temp>>8);
			
//					MOTOR_SPEED=speed_temp;									//����ٶ��ͳ���Һ����������	
//				  Display_Time1();
					
					
				//�ٶ������� ���µ���
					if(abs(MOTOR_SPEED-speed_temp)>5)
					{
								/* PWM������� */
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
									 PWMStart(PWM_SEL_0,PWM_MODE_0,ddDuty);		//PCAģ��0Ϊ8λPWMģʽ
								}
					}
			}
	
			
//		sprintf(CC, "%d", MOTOR_SPEED); 
//		UartSendStr("MOTOR_SPEED=");//
//		UartSendStr(CC); 
//		sprintf(CC, "%u", ddDuty); 
//		UartSendStr("Duty=");//
//		UartSendStr(CC); 
		
//			PWMStart(PWM_SEL_1,PWM_MODE_0,ddDuty);		//PCAģ��1Ϊ8λPWMģʽ
//			UartSendStr("Run"); 
}

//*******************************//
//           �жϷ���            //
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
        count0 = count1;            //������һ�εĲ���ֵ
        ((BYTE *)&count1)[3] = CCAP1L;  //���汾�εĲ���ֵ
        ((BYTE *)&count1)[2] = CCAP1H;
        ((BYTE *)&count1)[1] = cnt;
        ((BYTE *)&count1)[0] = 0;
        length = count1 - count0;   //�������β���Ĳ�ֵ,���õ�ʱ�䳤��
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
        cnt++;                      //PCA��ʱ�������+1
    }
}
