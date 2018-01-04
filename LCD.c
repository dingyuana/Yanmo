#include "SmartM_8051.h"


STATIC UINT8 g_ucFontPos     =LCD_FONT_IN_SD ;
STATIC UINT8 g_ucLcdDirection=LCD_DIRECTION_180;
STATIC UINT8 g_ucLcdType     =LCD_TYPE_UNKNOWN;


/****************************************
*��������:LcdWriteBus
*��    ��:VH -��8λ����
          VL -��8λ����
*��    ��:��
*��    ��:Lcd�������ݴ���
******************************************/			                     
VOID LcdWriteBus(UINT8 VH,UINT8 VL)   //��������д�뺯��
{	

  HC573_LE(1);
	LCD_DATAPORT_H=VL;	                //�Ƚ���8λ�����ͳ�
	HC573_LE(0);                        //����
	LCD_DATAPORT_H=VH;	                //���͸�8λ����		
	LCD_WR(0);
	LCD_WR(1); 

}
/****************************************
*��������:LcdWriteCmd
*��    ��:VH -��8λ����
          VL -��8λ����
*��    ��:��
*��    ��:Lcdд����
******************************************/		
VOID LcdWriteCmd(UINT8 VH,UINT8 VL)	 //��������-8λ����
{	
    LCD_RS(0);
	  LcdWriteBus(VH,VL);
}
/****************************************
*��������:LcdWriteCmd
*��    ��:VH -��8λ����
          VL -��8λ����
*��    ��:��
*��    ��:Lcdд����
******************************************/	
VOID LcdWriteData(UINT8 VH,UINT8 VL) //��������-8λ����
{
    LCD_RS(1);
	  LcdWriteBus(VH,VL);
}
/****************************************
*��������:LcdWriteCmdData
*��    ��:VH -��8λ����
          VL -��8λ����
*��    ��:��
*��    ��:Lcdд����
******************************************/	
VOID LcdWriteCmdData(UINT16 cmd,UINT16 val)		   //������������
{
	  LcdWriteCmd(cmd>>8,cmd);
    LcdWriteData(val>>8,val);
}
/****************************************
*��������:LcdAddressSet
*��    ��:x1 -������1
          y1 -������1
          x2 -������2
          y2 -������2
*��    ��:��
*��    ��:Lcd��ʾ��ַ
******************************************/	
VOID LcdAddressSet(UINT16 x1,UINT16 y1,UINT16 x2,UINT16 y2)
{
	  UINT16 Lcd_x1=x1,Lcd_y1=y1,Lcd_x2=x2,Lcd_y2=y2;
	
	   /*   �����Ļ�Ƿ�ת180�� */
	  if(g_ucLcdDirection == LCD_DIRECTION_180)
	  {
	     Lcd_x1=LCD_WIDTH-x2;
		   Lcd_x2=LCD_WIDTH-x1;
		   Lcd_y1=LCD_HEIGHT-y2;
		   Lcd_y2=LCD_HEIGHT-y1;
	  }
		
     /*   ���LCD���� */
		switch(g_ucLcdType)
		{
			case LCD_TYPE_24:
			{
						LcdWriteCmd(0x00,0x20);LcdWriteData(Lcd_x1>>8,Lcd_x1); //����X����λ��	  
						LcdWriteCmd(0x00,0x21);LcdWriteData(Lcd_y1>>8,Lcd_y1);  //����Y����λ��
 
						LcdWriteCmd(0x00,0x50);LcdWriteData(Lcd_x1>>8,Lcd_x1);//��ʼX

				
						LcdWriteCmd(0x00,0x52);LcdWriteData(Lcd_y1>>8,Lcd_y1); //��ʼY	  
						LcdWriteCmd(0x00,0x51);LcdWriteData(Lcd_x2>>8,Lcd_x2); //����X
						LcdWriteCmd(0x00,0x53);LcdWriteData(Lcd_y2>>8,Lcd_y2); 	//����Y
					
                        LcdWriteCmd(0x00,0x22);						
				
			}break;
			
			
			case LCD_TYPE_32:
			{
						LcdWriteCmd(0x00,0x44);LcdWriteData(Lcd_x2,Lcd_x1);	  
						LcdWriteCmd(0x00,0x45);LcdWriteData(Lcd_y1>>8,Lcd_y1);  
						LcdWriteCmd(0x00,0x46);LcdWriteData(Lcd_y2>>8,Lcd_y2);
				
						LcdWriteCmd(0x00,0x4E);LcdWriteData(Lcd_x1>>8,Lcd_x1);	  
						LcdWriteCmd(0x00,0x4F);LcdWriteData(Lcd_y1>>8,Lcd_y1); 
						LcdWriteCmd(0x00,0x22);						
			}break;

			default:break;				
		}
							 
}
/****************************************
*��������:LcdCleanScreen
*��    ��:usColor -��ɫ
*��    ��:��
*��    ��:Lcd����
******************************************/	
VOID LcdCleanScreen(UINT16 usColor)
{
	 UINT16 i,j;

	 LcdAddressSet(0,0,239,319);

   for(i=0;i<320;i++)
	 {
	    for (j=0;j<240;j++)
	   	{
        	 LcdWriteData(usColor>>8,usColor);
	    }
	 }
}
/****************************************
*��������:LcdInit
*��    ��:ucFontPos       -�ַ��洢λ��
          ucLcdDirection  -��ʾ����
*��    ��:��
*��    ��:Lcd��ʼ��
******************************************/	
VOID LcdInit(UINT8 ucFontPos,UINT8 ucLcdDirection)
{
  /* P2��������� */
  P2M1=0;
  P2M0=0xFF;
	
	/* LCD_WR P3.6���ţ�������� */
	P3M1&=~(1<<6);
	P3M0|=  1<<6 ;
	
	/* LCD_RS P1.1���ţ�������� */
	P1M1&=~(1<<1);
	P1M0|=  1<<1 ;	
	
	/* H573_LE P0.0���ţ�������� */
	P0M1&=~(1<<0);
	P0M0|=  1<<0 ;		
	
	g_ucFontPos = ucFontPos;

	g_ucLcdDirection = ucLcdDirection;
	
	if(g_ucLcdType == LCD_TYPE_24)
	{
		 LcdWriteCmdData(0x0001,0x0100); 
		 LcdWriteCmdData(0x0002,0x0700); 
		 LcdWriteCmdData(0x0003,0x1030); 
		 LcdWriteCmdData(0x0004,0x0000); 
		 LcdWriteCmdData(0x0008,0x0207);  
		 LcdWriteCmdData(0x0009,0x0000);
		 LcdWriteCmdData(0x000A,0x0000); 
		 LcdWriteCmdData(0x000C,0x0000); 
		 LcdWriteCmdData(0x000D,0x0000);
		 LcdWriteCmdData(0x000F,0x0000);
		 LcdWriteCmdData(0x0010,0x0000);   
		 LcdWriteCmdData(0x0011,0x0007);  
		 LcdWriteCmdData(0x0012,0x0000);  
		 LcdWriteCmdData(0x0013,0x0000); 
		 LcdWriteCmdData(0x0010,0x1290);   
		 LcdWriteCmdData(0x0011,0x0227);
		 LcdWriteCmdData(0x0012,0x001d); 
		 LcdWriteCmdData(0x0013,0x1500);
		 LcdWriteCmdData(0x0029,0x0018); 
		 LcdWriteCmdData(0x002B,0x000D); 
		 LcdWriteCmdData(0x0030,0x0004);
		 LcdWriteCmdData(0x0031,0x0307);
		 LcdWriteCmdData(0x0032,0x0002);
		 LcdWriteCmdData(0x0035,0x0206);
		 LcdWriteCmdData(0x0036,0x0408);
		 LcdWriteCmdData(0x0037,0x0507); 
		 LcdWriteCmdData(0x0038,0x0204);
		 LcdWriteCmdData(0x0039,0x0707); 
		 LcdWriteCmdData(0x003C,0x0405);
		 LcdWriteCmdData(0x003D,0x0F02); 
		 LcdWriteCmdData(0x0050,0x0000); 
		 LcdWriteCmdData(0x0051,0x00EF);
		 LcdWriteCmdData(0x0052,0x0000); 
		 LcdWriteCmdData(0x0053,0x013F);  
		 LcdWriteCmdData(0x0060,0xA700); 
		 LcdWriteCmdData(0x0061,0x0001); 
		 LcdWriteCmdData(0x006A,0x0000); 
		 LcdWriteCmdData(0x0080,0x0000); 
		 LcdWriteCmdData(0x0081,0x0000); 
		 LcdWriteCmdData(0x0082,0x0000); 
		 LcdWriteCmdData(0x0083,0x0000); 
		 LcdWriteCmdData(0x0084,0x0000); 
		 LcdWriteCmdData(0x0085,0x0000); 
		 LcdWriteCmdData(0x0090,0x0010); 
		 LcdWriteCmdData(0x0092,0x0600); 
		 LcdWriteCmdData(0x0093,0x0003); 
		 LcdWriteCmdData(0x0095,0x0110); 
		 LcdWriteCmdData(0x0097,0x0000); 
		 LcdWriteCmdData(0x0098,0x0000);
		 LcdWriteCmdData(0x0007,0x0133);	
	}

  if(g_ucLcdType == LCD_TYPE_32)
	{
			LcdWriteCmdData(0x0000,0x0001);    Delayms(1);//�򿪾���
			LcdWriteCmdData(0x0003,0xA8A4);    Delayms(1);   
			LcdWriteCmdData(0x000C,0x0000);    Delayms(1);   
			LcdWriteCmdData(0x000D,0x080C);    Delayms(1);   
			LcdWriteCmdData(0x000E,0x2B00);    Delayms(1);   
			LcdWriteCmdData(0x001E,0x00B7);    Delayms(1);   
			LcdWriteCmdData(0x0001,0x2B3F);    Delayms(1); //�����������320*240  0
			LcdWriteCmdData(0x0002,0x0600);    Delayms(1);
			LcdWriteCmdData(0x0010,0x0000);    Delayms(1);
			LcdWriteCmdData(0x0011,0x6070);    Delayms(1); //�������ݸ�ʽ  16λɫ 
			LcdWriteCmdData(0x0005,0x0000);    Delayms(1);
			LcdWriteCmdData(0x0006,0x0000);    Delayms(1);
			LcdWriteCmdData(0x0016,0xEF1C);    Delayms(1);
			LcdWriteCmdData(0x0017,0x0003);    Delayms(1);
			LcdWriteCmdData(0x0007,0x0233);    Delayms(1);       
			LcdWriteCmdData(0x000B,0x0000);    Delayms(1);
			LcdWriteCmdData(0x000F,0x0000);    Delayms(1);//ɨ�迪ʼ��ַ
			LcdWriteCmdData(0x0041,0x0000);    Delayms(1);
			LcdWriteCmdData(0x0042,0x0000);    Delayms(1);
			LcdWriteCmdData(0x0048,0x0000);    Delayms(1);
			LcdWriteCmdData(0x0049,0x013F);    Delayms(1);
			LcdWriteCmdData(0x004A,0x0000);    Delayms(1);
			LcdWriteCmdData(0x004B,0x0000);    Delayms(1);
			LcdWriteCmdData(0x0044,0xEF00);    Delayms(1);
			LcdWriteCmdData(0x0045,0x0000);    Delayms(1);
			LcdWriteCmdData(0x0046,0x013F);    Delayms(1);
			LcdWriteCmdData(0x0030,0x0707);    Delayms(1);
			LcdWriteCmdData(0x0031,0x0204);    Delayms(1);
			LcdWriteCmdData(0x0032,0x0204);    Delayms(1);
			LcdWriteCmdData(0x0033,0x0502);    Delayms(1);
			LcdWriteCmdData(0x0034,0x0507);    Delayms(1);
			LcdWriteCmdData(0x0035,0x0204);    Delayms(1);
			LcdWriteCmdData(0x0036,0x0204);    Delayms(1);
			LcdWriteCmdData(0x0037,0x0502);    Delayms(1);
			LcdWriteCmdData(0x003A,0x0302);    Delayms(1);
			LcdWriteCmdData(0x003B,0x0302);    Delayms(1);
			LcdWriteCmdData(0x0023,0x0000);    Delayms(1);
			LcdWriteCmdData(0x0024,0x0000);    Delayms(1);
			LcdWriteCmdData(0x0025,0x8000);    Delayms(1);
			LcdWriteCmdData(0x004f,0);        //����ַ0
			LcdWriteCmdData(0x004e,0);        //����ַ0
			LcdWriteCmd(0x00,0x22);		
	}

}	 
/****************************************
*��������:LcdDrawPoint
*��    ��:unFontPos       -�ַ��洢λ��
          unLcdDirection  -��ʾ����
*��    ��:��
*��    ��:Lcd��ʼ��
******************************************/
VOID LcdDrawPoint(UINT16 x,UINT16 y,UINT16 color)
{
	LcdAddressSet(x,y,x,y);//���ù��λ�� 
	LcdWriteData(color>>8,color);	 
}
/****************************************
*��������:LcdFill
*��    ��:x_s    -��������ʼ��ַ
          y_s    -��������ʼ��ַ
          x_e    -�����������ַ
          y_e    -�����������ַ
          color  -��ɫ
*��    ��:��
*��    ��:Lcd��ɫ���
******************************************/
VOID LcdFill(UINT16 x_s,UINT16 y_s,UINT16 x_e,UINT16 y_e,UINT16 color)
{          
	UINT16 i,j; 

	LcdAddressSet(x_s,y_s,x_e,y_e);      //���ù��λ�� 

	for(i=y_s;i<=y_e;i++)
	{													   	 	
		for(j=x_s;j<=x_e;j++)LcdWriteData(color>>8,color);//���ù��λ�� 	    
	} 					  	    
} 
/****************************************
*��������:LcdSetDirection
*��    ��:ucDirection ��ʾ����
*��    ��:��
*��    ��:Lcd��ʾ��������
******************************************/
VOID LcdSetDirection(UINT8 ucDirection)
{
	   g_ucLcdDirection = ucDirection;
}
/****************************************
*��������:LcdGetDirection
*��    ��:��
*��    ��:��
*��    ��:Lcd��ʾ�����ȡ
******************************************/
UINT8 LcdGetDirection(VOID)
{

	   return g_ucLcdDirection;
}
/****************************************
*��������:LcdRemakeFont
*��    ��:pucCode       -�ַ�������
          ucFontType    -����
*��    ��:��
*��    ��:��������
******************************************/
STATIC VOID LcdRemakeFont(UINT8 *pucCode,UINT8 ucFontType)
{
   UINT8   i=0,j=0,m=0,n=0,ucFontsize=0;
   IDATA UINT8   TempBuf[32]={0};

   if(ucFontType ==FONT_IS_ASC16)
   {
   	  ucFontsize=16;
   }
    
   if(ucFontType ==FONT_IS_HZK16)
   {
   	  ucFontsize=32;
   }

   for(i=0;i<ucFontsize;i++)
   {
   	   TempBuf[ucFontsize-1-i]=pucCode[i];
	     TempBuf[i]=pucCode[ucFontsize-1-i];
   }

   for(i=0;i<ucFontsize;i+=2)
   {
      m=0;n=0;

      for(j=0; j<8; j++)
	    {
	  	  if(TempBuf[i]  &1<<j)m|=1<<(7-j);
		    if(TempBuf[i+1]&1<<j)n|=1<<(7-j);
	    }

	    pucCode[i]  =m;
   	  pucCode[i+1]=n;
   }
}
/****************************************
*��������:LcdDrawBigPoint
*��    ��:x          -������
          y          -������
					color      -�����ɫ
*��    ��:��
*��    ��:Lcd��ֵ�
******************************************/
VOID LcdDrawBigPoint(UINT16 x,UINT16 y,UINT16 color)
{
	   LcdFill(x,y,x+10,y+10,color);
}
/****************************************
*��������:LcdShowFlashFont
*��    ��:x          -������
          y          -������
          pucCode    -�ַ�������
					color      -������ɫ
					bgcolor    -���屳��ɫ
*��    ��:��
*��    ��:Lcd��ʾʱ���ô洢��Flash�е�����
******************************************/
STATIC VOID LcdShowFlashFont(UINT16 x,UINT16 y,INT8 *pucCode,UINT16 color,UINT16 bgcolor)
{				  
   UINT32  FontOffset;
   UINT8   i=0,j=0;
	 
   IDATA UINT8   buf[32]={0}; 

   if(pucCode[0]&0x80)  //����
   {
	    FontOffset =(UINT32)FLASH_HZK16_ADDRESS;

      /* һ��Ҫǿ������ת��,�����������*/
	    FontOffset+=(UINT32)((94*(UINT32)((UINT8)pucCode[0]-0xA1)+((UINT8)pucCode[1]-0xA1))<<5);
		 
	    SpiFlashRead(buf,FontOffset,32);

      if(LcdGetDirection()!=LCD_DIRECTION_0)
	    {
	   	  LcdRemakeFont(buf,FONT_IS_HZK16);
	    }

	    LcdAddressSet(x,y,x+15,y+15); //��������    
		
	    for(j=0;j<32;j++)
	    {
				for(i=0;i<8;i++)
				{ 		     
						if((buf[j]&(1<<(7-i)))!=0)
						{
							LcdWriteData(color>>8,color);
						} 
						else
						{
							LcdWriteData(bgcolor>>8,bgcolor);
						}   
				}
	    } 
	    
   }//ASC
   else
   {
       FontOffset  =FLASH_ASC16_ADDRESS;

       FontOffset += pucCode[0] << 4;
		
	     SpiFlashRead(buf,FontOffset,16);

       if(LcdGetDirection()!=LCD_DIRECTION_0)
	     {
	   	    LcdRemakeFont(buf,FONT_IS_ASC16);
	     }
	
	     LcdAddressSet(x,y,x+7,y+15); //��������  
		
			 for(j=0;j<16;j++)
			 {
					for(i=0;i<8;i++)
					{ 		     
						if((buf[j]&(1<<(7-i)))!=0)
						{
							LcdWriteData(color>>8,color);
						} 
						else
						{
							LcdWriteData(bgcolor>>8,bgcolor);
						}   
					}
			 } 
   }
}
/****************************************
*��������:LcdShowFont
*��    ��:x          -������
          y          -������
          pucCode    -�ַ�������
					color      -������ɫ
					bgcolor    -���屳��ɫ
*��    ��:��
*��    ��:Lcd��ʾ��������
******************************************/
VOID LcdShowFont(UINT16 x,UINT16 y,INT8 *pucCode,UINT16 color,UINT16 bgcolor)
{
	 switch(g_ucFontPos)
	 {
			case LCD_FONT_IN_SD:
			{
			}break;

			case LCD_FONT_IN_FLASH:
			{
				 LcdShowFlashFont(x,y,pucCode,color,bgcolor);
			}break;

		  default:break;
	 }

}
/****************************************
*��������:LcdShowString
*��    ��:x          -������
          y          -������
          pucCode    -�ַ�������
					color      -������ɫ
					bgcolor    -���屳��ɫ
*��    ��:��
*��    ��:Lcd��ʾ�ַ���
******************************************/
PIX LcdShowString(UINT16 x,UINT16 y,INT8 *pucCode,UINT16 color,UINT16 bgcolor)
{

   PIX Pix;
	
   while(pucCode && *pucCode)
	 {
			 LcdShowFont(x,y,pucCode,color,bgcolor);

			 if(*pucCode & 0x80)
			 {
				   pucCode+=2;

				   x+=16;
			 }
			 else
			 {
				   pucCode++;

				   x+=16;
			 }

			 if(x>=LCD_WIDTH-16)		//����
			 {
				  x = 0;

				  y+=20;	
			 }	  
	 }
	 
	 Pix.x=x;
	 Pix.y=y;
	 
	 return Pix;

}
/****************************************
*��������:LcdShowShortString
*��    ��:x          -������
          y          -������
          pucCode    -�ַ�������
					color      -������ɫ
					bgcolor    -���屳��ɫ
*��    ��:��
*��    ��:Lcd��ʾ�ַ���
******************************************/
PIX LcdShowShortString(UINT16 x,UINT16 y,INT8 *pucCode,UINT16 color,UINT16 bgcolor)
{
   
   PIX Pix;
	
   while(pucCode && *pucCode)
	 {
			 LcdShowFont(x,y,pucCode,color,bgcolor);

			 if(*pucCode & 0x80)
			 {
				   pucCode+=2;

				   x+=16;
			 }
			 else
			 {
				   pucCode++;

				   x+=8;
			 }

			 if(x>=LCD_WIDTH-8)		//����
			 {
				  x = 0;

				  y+=20;	
			 }	  
	 }
	 
	 Pix.x=x;
	 Pix.y=y;
	 
	 return Pix;

}
/****************************************
*��������:LcdTypeSet
*��    ��:unLcdType Lcd����(2.4/2.8.3.2��)
*��    ��:��
*��    ��:Lcd����������
******************************************/
VOID LcdTypeSet(UINT8 ucLcdType)
{
	   g_ucLcdType = ucLcdType;
}
/****************************************
*��������:LcdTypeSet
*��    ��:��
*��    ��:Lcd����(2.4/2.8.3.2��)
*��    ��:Lcd�����ͻ�ȡ
******************************************/
UINT8 LcdTypeGet(VOID)
{
	   return g_ucLcdType;
	
}



