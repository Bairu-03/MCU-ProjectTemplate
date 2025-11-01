#ifndef __FLASH_H
#define __FLASH_H

#include "stm8l15x.h"

typedef enum
{
    EEPROM_Status_Write_Protection_Error = (uint8_t)0x01,
    EEPROM_Status_TimeOut                = (uint8_t)0x02,
    EEPROM_Unlock_Failed                 = (uint8_t)0x08,
    EEPROM_Address_Error                 = (uint8_t)0x10,
    EEPROM_Status_Successful_Operation   = (uint8_t)0x40
} EEPROM_Status_TypeDef;

EEPROM_Status_TypeDef Write_EEPROM(uint32_t offsetAddress, uint8_t data);
EEPROM_Status_TypeDef Read_EEPROM(uint32_t offsetAddress, uint8_t *data);

#endif /* __FLASH_H */
