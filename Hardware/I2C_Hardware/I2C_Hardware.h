#ifndef __I2C_HARDWARE_H
#define __I2C_HARDWARE_H

#include "stm32f10x.h"
#include "stdint.h"

void I2C_HW_Init(void);
void I2C_HW_Start(void);
void I2C_HW_SendAddr(uint8_t addr);
void I2C_HW_SendByte(uint8_t data);
void I2C_HW_Stop(void);

#endif /* __I2C_HARDWARE_H */
