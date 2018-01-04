#include "SmartM_8051.h"



/****************************************
*��������:SpiFlashInit
*��    ��:��
*��    ��:��
*��    ��:��ʼ��SPI FLASH��IO��
******************************************/
VOID SpiFlashInit(VOID)
{
     /* Flash CS P3.7�������*/
		 P3M1&=~(1<<7);
		 P3M0|=  1<<7;
		 
     SpiMasterInit();
}  
/****************************************
*��������:SpiFlashReadSR
*��    ��:��
*��    ��:æ���λ(1,æ;0,����)
*��    ��:��ȡSPI_FLASH��״̬�Ĵ���
BIT7  6   5   4   3   2   1   0
SPR   RV  TB BP2 BP1 BP0 WEL BUSY
SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
TB,BP2,BP1,BP0:FLASH����д��������
WEL:дʹ������
BUSY:æ���λ(1,æ;0,����)
Ĭ��:0x00
******************************************/
UINT8 SpiFlashReadSR(VOID)   
{  
	UINT8 rt=0;   
	SPI_FLASH_CS(0);                      //ʹ������   
	SpiWriteRead(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	rt=SpiWriteRead(0Xff);               //��ȡһ���ֽ�  
	SPI_FLASH_CS(1);                      //ȡ��Ƭѡ     
	return rt;   
} 
/****************************************
*��������:SpiFlashWriteSR
*��    ��:����״ֵ̬
*��    ��:��
*��    ��:дSPI_FLASH��״̬�Ĵ���
ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д
******************************************/
VOID SpiFlashWriteSR(UINT8 sr)   
{   
	SPI_FLASH_CS(0);                      //ʹ������   
	SpiWriteRead(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SpiWriteRead(sr);                    //д��һ���ֽ�  
	SPI_FLASH_CS(1);                      //ȡ��Ƭѡ     	      
}   
/****************************************
*��������:SpiFlashWriteEnable
*��    ��:��
*��    ��:��
*��    ��:SPI_FLASHдʹ�ܣ�WEL��λ
******************************************/
VOID SpiFlashWriteEnable(VOID)   
{
	SPI_FLASH_CS(0);                      //ʹ������   
  SpiWriteRead(W25X_WriteEnable);      //����дʹ��  
	SPI_FLASH_CS(1);                      //ȡ��Ƭѡ     	      
} 
/****************************************
*��������:SpiFlashWriteDisable
*��    ��:��
*��    ��:��
*��    ��:SPI_FLASHд��ֹ����WEL����  	
******************************************/
VOID SpiFlashWriteDisable(VOID)   
{  
	SPI_FLASH_CS(0);                       //ʹ������   
  SpiWriteRead(W25X_WriteDisable);      //����д��ָֹ��    
	SPI_FLASH_CS(1);                       //ȡ��Ƭѡ     	      
} 		  
/****************************************
*��������:SpiFlashReadID
*��    ��:��
*��    ��:			   
					0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
					0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
					0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
					0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
*��    ��:��ȡоƬID
******************************************/
UINT16 SpiFlashReadID(VOID)
{
	UINT16 Temp = 0;	  
	SPI_FLASH_CS(0);				    
	SpiWriteRead(0x90);             //���Ͷ�ȡID����	    
	SpiWriteRead(0x00); 	    
	SpiWriteRead(0x00); 	    
	SpiWriteRead(0x00); 	 			   
	Temp|=SpiWriteRead(0xFF)<<8;  
	Temp|=SpiWriteRead(0xFF);	 
	SPI_FLASH_CS(1);				    
	return Temp;
}   		    
/****************************************
*��������:SpiFlashRead
*��    ��:pBuffer        -���ݴ洢��
          ReadAddr       -��ʼ��ȡ�ĵ�ַ(24bit)
          NumByteToRead  -Ҫ��ȡ���ֽ���(���65535)
*��    ��:��
*��    ��:��ȡSPI FLASH  
******************************************/
VOID SpiFlashRead(UINT8* pBuffer,UINT32 ReadAddr,UINT16 NumByteToRead)   
{ 
 	UINT16 i;   										    
	SPI_FLASH_CS(0);                         //ʹ������   
  SpiWriteRead(W25X_ReadData);            //���Ͷ�ȡ����   
  SpiWriteRead((UINT8)((ReadAddr)>>16));  //����24bit��ַ    
  SpiWriteRead((UINT8)((ReadAddr)>>8));   
  SpiWriteRead((UINT8)(ReadAddr&0xFF));   
	
  for(i=0;i<NumByteToRead;i++)
	{ 
      pBuffer[i]=SpiWriteRead(0XFF);      //ѭ������  
  }
	
	SPI_FLASH_CS(1);  				    	      
}  
/****************************************
*��������:SpiFlashWritePage
*��    ��:pBuffer        -���ݴ洢��
          WriteAddr      -��ʼд��ĵ�ַ(24bit)
          NumByteToWrite -Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���
*��    ��:��
*��    ��:SPI��һҳ(0~65535)��д������256���ֽڵ�����
******************************************/
VOID SpiFlashWritePage(UINT8* pBuffer,UINT32 WriteAddr,UINT16 NumByteToWrite)
{
 	UINT16 i; 
	
  SpiFlashWriteEnable();                  //SET WEL 
	SPI_FLASH_CS(0);                           //ʹ������
	
  SpiWriteRead(W25X_PageProgram);           //����дҳ����   
  SpiWriteRead((UINT8)((WriteAddr)>>16));   //����24bit��ַ    
  SpiWriteRead((UINT8)((WriteAddr)>>8));   
  SpiWriteRead((UINT8)WriteAddr);
	
  for(i=0;i<NumByteToWrite;i++)SpiWriteRead(pBuffer[i]);//ѭ��д��  
	
	SPI_FLASH_CS(1);                            //ȡ��Ƭѡ 
	SpiFlashWaitBusy();					            //�ȴ�д�����
} 
/****************************************
*��������:SpiFlashWriteNoCheck
*��    ��:pBuffer        -���ݴ洢��
          WriteAddr      -��ʼд��ĵ�ַ(24bit)
          NumByteToWrite -Ҫд����ֽ���(���65535)
*��    ��:��
*��    ��:�޼���дSPI FLASH 
����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,
�����ڷ�0XFF��д������ݽ�ʧ��!
�����Զ���ҳ���� 
��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
******************************************/
VOID SpiFlashWriteNoCheck(UINT8* pBuffer,UINT32 WriteAddr,UINT16 NumByteToWrite)   
{ 			 		 
	UINT16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	
	while(1)
	{	   
		SpiFlashWritePage(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			      //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	    //����256���ֽ���
		}
	}	    
} 

/****************************************
*��������:SpiFlashWrite
*��    ��:pBuffer        -���ݴ洢��
          WriteAddr      -��ʼд��ĵ�ַ(24bit)
          NumByteToWrite -Ҫд����ֽ���(���65535)
*��    ��:��
*��    ��:дSPI FLASH  
��ָ����ַ��ʼд��ָ�����ȵ����ݲ�����������!
******************************************/
VOID SpiFlashWrite(UINT8* pBuffer,UINT32 WriteAddr,UINT16 NumByteToWrite)   
{ 
	UINT32   secpos;
	
 	secpos=WriteAddr/512;//������ַ  
  SpiFlashEraseSector(secpos);//�����������
  SpiFlashWriteNoCheck(pBuffer,WriteAddr,NumByteToWrite);//д����������  
}
/****************************************
*��������:SpiFlashEraseChip
*��    ��:��
*��    ��:��
*��    ��:��������оƬ		  
******************************************/
VOID SpiFlashEraseChip(VOID)   
{                                   
    SpiFlashWriteEnable();             //SET WEL 
    SpiFlashWaitBusy();   
  	SPI_FLASH_CS(0);                      //ʹ������   
    SpiWriteRead(W25X_ChipErase);        //����Ƭ��������  
	  SPI_FLASH_CS(1);                      //ȡ��Ƭѡ     	      
	  SpiFlashWaitBusy();   				      //�ȴ�оƬ��������
}   
/****************************************
*��������:SpiFlashEraseSector
*��    ��:Dst_Addr  -������ַ ����ʵ����������
*��    ��:��
*��    ��:����һ����������150����
******************************************/
VOID SpiFlashEraseSector(UINT32 Dst_Addr)   
{  
 
 	  Dst_Addr*=512;
    SpiFlashWriteEnable();                //SET WEL 	 
    SpiFlashWaitBusy();   
  	SPI_FLASH_CS(0);                         //ʹ������   
    SpiWriteRead(W25X_SectorErase);         //������������ָ�� 
    SpiWriteRead((UINT8)((Dst_Addr)>>16));  //����24bit��ַ    
    SpiWriteRead((UINT8)((Dst_Addr)>>8));   
    SpiWriteRead((UINT8)Dst_Addr);  
	  SPI_FLASH_CS(1);                         //ȡ��Ƭѡ     	      
    SpiFlashWaitBusy();   				         //�ȴ��������
}  
/****************************************
*��������:SpiFlashWaitBusy
*��    ��:��
*��    ��:��
*��    ��:�ȴ�����
******************************************/
VOID SpiFlashWaitBusy(VOID)   
{   
	while((SpiFlashReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
/****************************************
*��������:SpiFlashPowerDown
*��    ��:��
*��    ��:��
*��    ��:�������ģʽ
******************************************/
VOID SpiFlashPowerDown(VOID)   
{ 
  	SPI_FLASH_CS(0);                         //ʹ������   
    SpiWriteRead(W25X_PowerDown);           //���͵�������  
	  SPI_FLASH_CS(1);                         //ȡ��Ƭѡ     	      
    Delayus(3);                              //�ȴ�TPD  
}   
/****************************************
*��������:SpiFlashWakeUp
*��    ��:��
*��    ��:��
*��    ��:����
******************************************/
VOID SpiFlashWakeUp(VOID)   
{  
  	SPI_FLASH_CS(0);                            //ʹ������   
    SpiWriteRead(W25X_ReleasePowerDown);       //���ͻ���ָ�� 
	  SPI_FLASH_CS(1);                            //ȡ��Ƭѡ     	      
    Delayus(3);                                 //�ȴ�TRES1
}   


























