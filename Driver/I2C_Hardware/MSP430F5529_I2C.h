#ifndef MSP430F5529_I2C_H_
#define MSP430F5529_I2C_H_

#include <stdint.h>

void I2C_init(uint8_t slaveAddress);
void writeByte(uint8_t byte);
void writeWord(uint16_t word);
void readByte(uint8_t RegAddr, uint8_t* b);
void readBytes(uint8_t RegAddr, uint8_t length, uint8_t* data);

#endif /* MSP430F5529_I2C_H_ */
