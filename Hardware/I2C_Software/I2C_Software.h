#ifndef __I2C_SOFTWARE_H
#define __I2C_SOFTWARE_H

#include "stm32f10x.h"
#include "stdint.h"

#define APB2_GPIO RCC_APB2Periph_GPIOB // APB2外设
#define GPIOX GPIOB                    // GPIOB端口

#define SCL_Pin GPIO_Pin_8 // PB8 -> SCL
#define SDA_Pin GPIO_Pin_9 // PB9 -> SDA

#define I2C_ACK 0
#define I2C_NO_ACK 1
#define OLED_R_SDA() GPIO_ReadInputDataBit(GPIOX, SDA_Pin)
#define OLED_W_SCL(x) GPIO_WriteBit(GPIOX, SCL_Pin, (BitAction)(x))
#define OLED_W_SDA(x) GPIO_WriteBit(GPIOX, SDA_Pin, (BitAction)(x))

void I2C_SW_Init(void);               // 初始化模拟I2C引脚。
void I2C_SW_Start(void);              // 模拟I2C起始信号。
void I2C_SW_Stop(void);               // 模拟I2C停止信号。
uint8_t I2C_SW_WaitAck(void);         // 等待从机应答信号。
void I2C_SW_SendAck(uint8_t ack);     // 发送应答信号。
uint8_t I2C_SW_ReadByte(uint8_t ack); // I2C读取一个字节。
void I2C_SW_SendByte(uint8_t Byte);   // I2C发送一个字节。

#endif /* __I2C_SOFTWARE_H */
