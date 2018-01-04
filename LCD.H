#ifndef __LCD_H__
#define __LCD_H__

#define LCD_FONT_IN_FLASH 0
#define LCD_FONT_IN_SD    1
#define LCD_FONT_IN_ROM   2


#define LCD_TYPE_24          0
#define LCD_TYPE_28          1
#define LCD_TYPE_32          2

#define LCD_TYPE_UNKNOWN     255

//#define LCD_BUS_16BIT
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	   0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色

#define LCD_DIRECTION_0	   0
#define LCD_DIRECTION_180  1

#define FONT_IS_ASC16	   0
#define FONT_IS_HZK16	   1

#define LCD_WIDTH        239
#define LCD_HEIGHT		   319



#define LCD_DATAPORT_H P2//修改了
#define LCD_DATAPORT_L P0


#define LCD_RS(x)   P1_1=(x)
#define LCD_WR(x)   P3_6=(x)


#define HC573_LE(x)    P0_0=(x) //锁存控制

typedef struct _PIX
{
	UINT16 x;
	UINT16 y;
}PIX,*PPIX;

EXTERN_C VOID LcdWriteCmd(UINT8 VH,UINT8 VL);	 
EXTERN_C VOID LcdWriteData(UINT8 VH,UINT8 VL);
EXTERN_C VOID LcdWriteCmdData(UINT16 cmd,UINT16 val);
EXTERN_C VOID LcdAddressSet(UINT16 x1,UINT16 y1,UINT16 x2,UINT16 y2);
EXTERN_C VOID LcdCleanScreen(UINT16 usColor);
EXTERN_C VOID LcdInit(UINT8 ucFontPos,UINT8 ucLcdDirection);	
EXTERN_C VOID LcdFill(UINT16 x_s,UINT16 y_s,UINT16 x_e,UINT16 y_e,UINT16 color);
EXTERN_C VOID LcdDrawBigPoint(UINT16 x,UINT16 y,UINT16 color);
EXTERN_C VOID LcdShowFont(UINT16 x,UINT16 y,INT8 *pucCode,UINT16 color,UINT16 bgcolor);
EXTERN_C PIX  LcdShowString(UINT16 x,UINT16 y,INT8 *pucCode,UINT16 color,UINT16 bgcolor);
EXTERN_C PIX  LcdShowShortString(UINT16 x,UINT16 y,INT8 *pucCode,UINT16 color,UINT16 bgcolor);
EXTERN_C VOID LcdSetDirection(UINT8 ucDirection);
EXTERN_C UINT8 LcdGetDirection(VOID);


EXTERN_C VOID  LcdTypeSet(UINT8 ucLcdType);
EXTERN_C UINT8 LcdTypeGet(VOID);

#endif


