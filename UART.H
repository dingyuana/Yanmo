#ifndef __UART_H__
#define __UART_H__


#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

EXTERN_C VOID UartInit(UINT32 usBaud);
EXTERN_C VOID UartSendByte(UINT8 d);
EXTERN_C VOID UartSend(UINT8 *pBuf,UINT8 NumOfBytes);
EXTERN_C VOID UartSendStr(INT8 *pStr);



#endif