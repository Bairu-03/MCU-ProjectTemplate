#include "EEPROM.h"

/**
  * @brief  向EEPROM写入一字节数据.
  * @param  offsetAddress : 要写入数据的偏移地址.
  *               取值范围 : 0x0000 - 0x03ff
  * @param  data : 要写入的数据.
  * @retval 写入操作状态
  */
EEPROM_Status_TypeDef Write_EEPROM(uint32_t offsetAddress, uint8_t data)
{
    uint8_t flagstatus;
    uint32_t address = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + offsetAddress;

    if (IS_FLASH_DATA_EEPROM_ADDRESS(address))
    {
        FLASH_Unlock(FLASH_MemType_Data);
        if (FLASH_GetFlagStatus(FLASH_FLAG_DUL) != RESET) // Data EEPROM已解锁
        {
            FLASH_ProgramByte(address, data);
            // 等待操作完成, 并返回状态
            flagstatus = (uint8_t)FLASH_WaitForLastOperation(FLASH_MemType_Data);
            FLASH_Lock(FLASH_MemType_Data);
        }
        else
        {
            flagstatus = (uint8_t)EEPROM_Unlock_Failed;
        }
    }
    else
    {
        flagstatus = (uint8_t)EEPROM_Address_Error;
    }
    return (EEPROM_Status_TypeDef)flagstatus;
}

/**
  * @brief  从EEPROM读取一字节数据.
  * @param  offsetAddress : 要读取数据的偏移地址.
  *               取值范围 : 0x0000 - 0x03ff
  * @param  data : 出参, 指向读取到的数据.
  * @retval 读取操作状态
  */
EEPROM_Status_TypeDef Read_EEPROM(uint32_t offsetAddress, uint8_t *data)
{
    uint8_t flagstatus;
    uint32_t address = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + offsetAddress;

    if (IS_FLASH_DATA_EEPROM_ADDRESS(address))
    {
        FLASH_Unlock(FLASH_MemType_Data);
        if (FLASH_GetFlagStatus(FLASH_FLAG_DUL) != RESET) // Data EEPROM已解锁
        {
            *data = FLASH_ReadByte(address);
            // 等待操作完成, 并返回状态
            flagstatus = (uint8_t)FLASH_WaitForLastOperation(FLASH_MemType_Data);
            FLASH_Lock(FLASH_MemType_Data);
        }
        else
        {
            flagstatus = (uint8_t)EEPROM_Unlock_Failed;
        }
    }
    else
    {
        flagstatus = (uint8_t)EEPROM_Address_Error;
    }
    return (EEPROM_Status_TypeDef)flagstatus;
}
