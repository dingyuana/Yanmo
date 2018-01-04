#ifndef __EEPROM_H__
#define __EEPROM_H__

EXTERN_C void Delay(UINT8 n);
EXTERN_C void IapIdle();
EXTERN_C UINT8 IapReadByte(UINT8 addr);
EXTERN_C void IapProgramByte(UINT8 addr, UINT8 dat);
EXTERN_C void IapEraseSector(UINT8 addr);

#endif