#ifndef __SPI_H__
#define __SPI_H__

#define SPI_CS(x)    P1_2=(x)
#define SPI_MOSI(x)  P1_3=(x)
#define SPI_MISO()   P1_4
#define SPI_SCK(x)   P1_5=(x)

EXTERN_C VOID  SpiMasterInit(VOID);
EXTERN_C UINT8 SpiWriteRead(UINT8 d);


EXTERN_C VOID SpiSetSpeedLow(VOID);
EXTERN_C VOID SpiSetSpeedHigh(VOID);

#endif

