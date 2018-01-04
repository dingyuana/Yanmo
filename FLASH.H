#ifndef __FLASH_H__
#define __FLASH_H__

//W25X系列/Q系列芯片列表	   
//W25Q80 ID  0XEF13
//W25Q16 ID  0XEF14
//W25Q32 ID  0XEF15
//W25Q32 ID  0XEF16	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
	   
#define	SPI_FLASH_CS(x)			P3_7=(x)

#define FLASH_ASC16_ADDRESS                 0
#define FLASH_HZK16_ADDRESS                 0x1000

#define FLASH_SYSTEM_CONFIG_ADDRESS         0x43000


#define FLASH_BITMAP1_SIZE_ADDRESS	        0x50000
#define FLASH_BITMAP2_SIZE_ADDRESS	        FLASH_BITMAP1_SIZE_ADDRESS+0x28000
#define FLASH_BITMAP3_SIZE_ADDRESS	        FLASH_BITMAP2_SIZE_ADDRESS+0x28000
#define FLASH_BITMAP4_SIZE_ADDRESS	        FLASH_BITMAP3_SIZE_ADDRESS+0x28000
#define FLASH_BITMAP5_SIZE_ADDRESS	        FLASH_BITMAP4_SIZE_ADDRESS+0x28000	
#define FLASH_BITMAP6_SIZE_ADDRESS	        FLASH_BITMAP5_SIZE_ADDRESS+0x28000

#define FLASH_BITMAPMAIN_SIZE_ADDRESS       FLASH_BITMAP6_SIZE_ADDRESS+0x28000
#define FLASH_BITMAPDS1302_SIZE_ADDRESS     FLASH_BITMAPMAIN_SIZE_ADDRESS+0x28000
#define FLASH_BITMAPDS18B20_SIZE_ADDRESS    FLASH_BITMAPDS1302_SIZE_ADDRESS+0x28000
#define FLASH_BITMAPBLUETOOTH_SIZE_ADDRESS  FLASH_BITMAPDS18B20_SIZE_ADDRESS+0x28000


////////////////////////////////////////////////////////////////////////////
 
//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

EXTERN_C VOID   SpiFlashInit(VOID);
EXTERN_C UINT16 SpiFlashReadID(VOID);  	    //读取FLASH ID
EXTERN_C UINT8	SpiFlashReadSR(VOID);        //读取状态寄存器 
EXTERN_C VOID SpiFlashWriteSR(UINT8 sr);  	//写状态寄存器
EXTERN_C VOID SpiFlashWriteEnable(VOID);  //写使能 
EXTERN_C VOID SpiFlashWriteDisable(VOID);	//写保护
EXTERN_C VOID SpiFlashWriteNoCheck(UINT8* pBuffer,UINT32 WriteAddr,UINT16 NumByteToWrite);
EXTERN_C VOID SpiFlashRead(UINT8* pBuffer,UINT32 ReadAddr,UINT16 NumByteToRead);   //读取flash
EXTERN_C VOID SpiFlashWrite(UINT8* pBuffer,UINT32 WriteAddr,UINT16 NumByteToWrite);//写入flash
EXTERN_C VOID SpiFlashEraseChip(VOID);    	  //整片擦除
EXTERN_C VOID SpiFlashEraseSector(UINT32 Dst_Addr);//扇区擦除
EXTERN_C VOID SpiFlashWaitBusy(VOID);           //等待空闲
EXTERN_C VOID SpiFlashPowerDown(VOID);           //进入掉电模式
EXTERN_C VOID SpiFlashWakeUp(VOID);			  //唤醒













#endif

