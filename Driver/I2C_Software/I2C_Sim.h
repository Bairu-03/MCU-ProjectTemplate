/*
 * I2C_Sim.h
 *
 *  Created on: 2024年7月19日
 *      Author: Bairu
 */

#ifndef I2C_Sim_H_
#define I2C_Sim_H_

#include "driverlib.h"
#include <stdint.h>

// I2C应答信号
#define  I2C_ACK      0
#define  I2C_NO_ACK   1

#define  I2C_delay()  __delay_cycles(16)

#define    SCL_OUT(SCL_port, SCL_pin)     GPIO_setAsOutputPin((SCL_port), (SCL_pin))
#define    SCL_HIGH(SCL_port, SCL_pin)    GPIO_setOutputHighOnPin((SCL_port), (SCL_pin))
#define    SCL_LOW(SCL_port, SCL_pin)     GPIO_setOutputLowOnPin((SCL_port), (SCL_pin))

#define    SDA_IN(SDA_port, SDA_pin)      GPIO_setAsInputPin((SDA_port), (SDA_pin))
#define    SDA_OUT(SDA_port, SDA_pin)     GPIO_setAsOutputPin((SDA_port), (SDA_pin))
#define    SDA_HIGH(SDA_port, SDA_pin)    GPIO_setOutputHighOnPin((SDA_port), (SDA_pin))
#define    SDA_LOW(SDA_port, SDA_pin)     GPIO_setOutputLowOnPin((SDA_port), (SDA_pin))
#define    SDA(SDA_port, SDA_pin)         GPIO_getInputPinValue((SDA_port), (SDA_pin))

void I2C_Start(uint16_t SCL_port, uint16_t SCL_pin,
                uint16_t SDA_port, uint16_t SDA_pin);
void I2C_Stop(uint16_t SCL_port, uint16_t SCL_pin,
                uint16_t SDA_port, uint16_t SDA_pin);
void I2C_SendACK(uint16_t SCL_port, uint16_t SCL_pin,
                    uint16_t SDA_port, uint16_t SDA_pin, uint8_t ack);
uint8_t I2C_RecvACK(uint16_t SCL_port, uint16_t SCL_pin,
                        uint16_t SDA_port, uint16_t SDA_pin);
void I2C_SendByte(uint16_t SCL_port, uint16_t SCL_pin,
                    uint16_t SDA_port, uint16_t SDA_pin, uint8_t byte);
uint8_t I2C_ReadByte(uint16_t SCL_port, uint16_t SCL_pin,
                        uint16_t SDA_port, uint16_t SDA_pin);

#endif /* I2C_Sim_H_ */
