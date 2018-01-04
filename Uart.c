#include "SmartM_8051.h"

/****************************************
*函数名称:UartInit
*输    入:无
*输    出:无
*功    能:串口初始化
******************************************/
VOID UartInit(UINT32 usBaud)        //定义串口初始化函数
{ 
    P_SW1 &=~(S1_S0)|(S1_S1);       //串口1在P3.0、P3.1
		
    SCON = 0x50;                    //8位可变波特率
    T2L = (65536 - (FOSC/4/usBaud));   //设置波特率重装值
    T2H = (65536 - (FOSC/4/usBaud))>>8;
    AUXR = 0x14;                    //T2为1T模式, 并启动定时器2
    AUXR |= 0x01;                   //选择定时器2为串口1的波特率发生器
}
/****************************************
*函数名称:UartSendByte
*输    入:d 要发送的字节
*输    出:无
*功    能:串口发送单字节
******************************************/
VOID UartSendByte(UINT8 d)//串口发送单字节函数
{
			SBUF=d;                 //缓冲区装载要发送的字节
			while(TI==0);           //等待发送完毕,TI标志位会置1
				TI=0;                   //清零发送完成标志位
}
/****************************************
*函数名称:UartSendStr
*输    入:pStr -字符串
*输    出:无
*功    能:串口打印字符串
******************************************/
VOID UartSendStr(INT8 *pStr)
{
     while(*pStr && pStr)
		 {
		    UartSendByte(*pStr++);
		 
		 }
}

VOID UartSend(UINT8 *pBuf,UINT8 NumOfBytes)
{
     while(NumOfBytes--)
		 {
		     UartSendByte(*pBuf++);
		 }
 
}