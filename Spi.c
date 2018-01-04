#include "SmartM_8051.h"


/************��SPCTL�Ĵ���������λ�궨��*************/
#define SSIG    0x80  //SS���ź���
#define SPEN    0x40  //SPIʹ��λ
#define DORD    0x20  //SPI���ݷ���LSB���ȷ���
#define MSTR    0x10  //����ģʽѡ��
#define CPOL    0x08  //SPICLK����ʱΪ�ߵ�ƽ
#define CPHA    0x04  //������SPICLK��ǰʱ��������,���ں�ʱ���ز���
#define SP_CLK0 0x00  //SPIʱ��Ƶ��ΪCPU_CLK/4
#define SP_CLK1 0x01  //SPIʱ��Ƶ��ΪCPU_CLK/16
#define SP_CLK2 0x02  //SPIʱ��Ƶ��ΪCPU_CLK/64
#define SP_CLK3 0x03  //SPIʱ��Ƶ��ΪCPU_CLK/128
/************��SPSTAT�Ĵ���������λ�궨��************/
#define SPIF    0x80  //������ɱ�־
#define WCOL    0x40  //SPIд��ͻ��־		


/****************************************
*��������:Spi0MasterInit
*��    ��:��
*��    ��:��
*��    ��:SPI0��ʼ��Ϊ����ģʽ
******************************************/
VOID SpiMasterInit(VOID)
{
#if 0
		 /* SPI MISO P1.4����*/
		P1M1|=  1<<4 ;
		P1M0&=~(1<<4);
		 
		 /* SPI MOSI,SCK,CS P1.2,P1.3,P1.5�������*/
		P1M1&=~((1<<2)|(1<<3)|(1<<5));
		P1M0|=  (1<<2)|(1<<3)|(1<<5);
		 
    SPI_CS(1);
		SPI_MOSI(1);
		SPI_SCK(1);
#else	
		P1M1&=~(1<<2);
		P1M0|= (1<<2);
    SPDAT  = 0;         //������ݼĴ���
    SPSTAT = SPIF|WCOL; //���SPI״̬�Ĵ���
		
#if FOSC==33000000UL		
	  SPCTL  = SPEN|MSTR|SP_CLK1|SSIG; //SPI����Ϊ����ģʽ
#endif

#if FOSC==24000000UL	
	  SPCTL  = SPEN|MSTR|SP_CLK0|SSIG; //SPI����Ϊ����ģʽ
#endif		
    
#endif		 
}

/****************************************
*��������:SpiWriteRead
*��    ��:ucData Ҫ���͵�����
*��    ��:�����ֽ�����
*��    ��:SPI��д����
******************************************/
#if 0
UINT8 SpiWriteRead(UINT8 d)
{
    UINT8 i;
	
	  SPI_SCK(0);
	
    for(i=0; i<8; i++)
    {
			  /*   �����ֽڸ�λ */
		    if(d & 0x80)
				{
					 SPI_MOSI(1);
				}
				else
				{
					 SPI_MOSI(0);
				}
				
				/*   �����ֽڸ�λ */
				d<<=1;
				
				SPI_SCK(1);
			
				if(SPI_MISO())
				{
					 d|=0x01;
				}
					
				SPI_SCK(0);

		
		}	
		
		return d;
}
#else




UINT8 SpiWriteRead(UINT8 d)
{
    SPDAT = d;                  //����SPI��������
    while (!(SPSTAT & SPIF));   //�ȴ��������
    SPSTAT = SPIF | WCOL;       //���SPI״̬λ
    return SPDAT;               //����SPI����
}
#endif

VOID SpiSetSpeedLow(VOID)
{
    SPCTL  &=~SP_CLK3; 
	  SPCTL |=SP_CLK3;
}

VOID SpiSetSpeedHigh(VOID)
{
    SPCTL  &=~SP_CLK3; 
#if FOSC==33000000UL		
	  SPCTL  |=SP_CLK1;
#endif

#if FOSC==24000000UL	
	  SPCTL  |=SP_CLK0;
#endif
}




