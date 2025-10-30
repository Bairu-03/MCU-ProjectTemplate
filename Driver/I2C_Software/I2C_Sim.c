/*
 * I2C_Sim.c
 *
 *  Created on: 2024年7月19日
 *      Author: Bairu
 */

#include "I2C_Sim.h"

/**
 * @brief  IIC起始信号。
 * @param  SCL_port 产生I2C SCL信号的端口号
 *      @arg 有效取值:
 *        - \b GPIO_PORT_P1
 *        - \b GPIO_PORT_P2
 *        - \b GPIO_PORT_P3
 *        - \b GPIO_PORT_P4
 *        - \b GPIO_PORT_P5
 *        - \b GPIO_PORT_P6
 *        - \b GPIO_PORT_P7
 *        - \b GPIO_PORT_P8
 *        - \b GPIO_PORT_P9
 *        - \b GPIO_PORT_P10
 *        - \b GPIO_PORT_P11
 *        - \b GPIO_PORT_PA
 *        - \b GPIO_PORT_PB
 *        - \b GPIO_PORT_PC
 *        - \b GPIO_PORT_PD
 *        - \b GPIO_PORT_PE
 *        - \b GPIO_PORT_PF
 *        - \b GPIO_PORT_PJ
 * @param  SCL_pin 产生I2C SCL信号的引脚号
 *      @arg 有效取值:
 *        - \b GPIO_PIN0
 *        - \b GPIO_PIN1
 *        - \b GPIO_PIN2
 *        - \b GPIO_PIN3
 *        - \b GPIO_PIN4
 *        - \b GPIO_PIN5
 *        - \b GPIO_PIN6
 *        - \b GPIO_PIN7
 *        - \b GPIO_PIN8
 *        - \b GPIO_PIN9
 *        - \b GPIO_PIN10
 *        - \b GPIO_PIN11
 *        - \b GPIO_PIN12
 *        - \b GPIO_PIN13
 *        - \b GPIO_PIN14
 *        - \b GPIO_PIN15
 *        - \b GPIO_PIN_ALL8
 *        - \b GPIO_PIN_ALL16
 * @param  SDA_port 产生I2C SDA信号的端口号
 *      @arg 有效取值: 同SCL_port
 * @param  SDA_pin 产生I2C SDA信号的引脚号
 *      @arg 有效取值: 同SCL_pin
 * @retval 无
 */
void I2C_Start(uint16_t SCL_port, uint16_t SCL_pin,
                uint16_t SDA_port, uint16_t SDA_pin)
{
    SDA_OUT(SDA_port, SDA_pin);
    SCL_OUT(SCL_port, SCL_pin);
    SCL_HIGH(SCL_port, SCL_pin);
    SDA_HIGH(SDA_port, SDA_pin);
    I2C_delay();
    SDA_LOW(SDA_port, SDA_pin);
    I2C_delay();
    SCL_LOW(SCL_port, SCL_pin);
}

/**
 * @brief  IIC停止信号。
 * @param  SCL_port 产生I2C SCL信号的端口号
 *      @arg 有效取值:
 *        - \b GPIO_PORT_P1
 *        - \b GPIO_PORT_P2
 *        - \b GPIO_PORT_P3
 *        - \b GPIO_PORT_P4
 *        - \b GPIO_PORT_P5
 *        - \b GPIO_PORT_P6
 *        - \b GPIO_PORT_P7
 *        - \b GPIO_PORT_P8
 *        - \b GPIO_PORT_P9
 *        - \b GPIO_PORT_P10
 *        - \b GPIO_PORT_P11
 *        - \b GPIO_PORT_PA
 *        - \b GPIO_PORT_PB
 *        - \b GPIO_PORT_PC
 *        - \b GPIO_PORT_PD
 *        - \b GPIO_PORT_PE
 *        - \b GPIO_PORT_PF
 *        - \b GPIO_PORT_PJ
 * @param  SCL_pin 产生I2C SCL信号的引脚号
 *      @arg 有效取值:
 *        - \b GPIO_PIN0
 *        - \b GPIO_PIN1
 *        - \b GPIO_PIN2
 *        - \b GPIO_PIN3
 *        - \b GPIO_PIN4
 *        - \b GPIO_PIN5
 *        - \b GPIO_PIN6
 *        - \b GPIO_PIN7
 *        - \b GPIO_PIN8
 *        - \b GPIO_PIN9
 *        - \b GPIO_PIN10
 *        - \b GPIO_PIN11
 *        - \b GPIO_PIN12
 *        - \b GPIO_PIN13
 *        - \b GPIO_PIN14
 *        - \b GPIO_PIN15
 *        - \b GPIO_PIN_ALL8
 *        - \b GPIO_PIN_ALL16
 * @param  SDA_port 产生I2C SDA信号的端口号
 *      @arg 有效取值: 同SCL_port
 * @param  SDA_pin 产生I2C SDA信号的引脚号
 *      @arg 有效取值: 同SCL_pin
 * @retval 无
 */
void I2C_Stop(uint16_t SCL_port, uint16_t SCL_pin,
                uint16_t SDA_port, uint16_t SDA_pin)
{
    SDA_OUT(SDA_port, SDA_pin);
    SCL_OUT(SCL_port, SCL_pin);
    SCL_LOW(SCL_port, SCL_pin);
    SDA_LOW(SDA_port, SDA_pin);
    SCL_HIGH(SCL_port, SCL_pin);
    I2C_delay();
    SDA_HIGH(SDA_port, SDA_pin);
    I2C_delay();
}

/**
 * @brief  模拟IIC主机发送应答信号。
 * @param  SCL_port 产生I2C SCL信号的端口号
 *      @arg 有效取值:
 *        - \b GPIO_PORT_P1
 *        - \b GPIO_PORT_P2
 *        - \b GPIO_PORT_P3
 *        - \b GPIO_PORT_P4
 *        - \b GPIO_PORT_P5
 *        - \b GPIO_PORT_P6
 *        - \b GPIO_PORT_P7
 *        - \b GPIO_PORT_P8
 *        - \b GPIO_PORT_P9
 *        - \b GPIO_PORT_P10
 *        - \b GPIO_PORT_P11
 *        - \b GPIO_PORT_PA
 *        - \b GPIO_PORT_PB
 *        - \b GPIO_PORT_PC
 *        - \b GPIO_PORT_PD
 *        - \b GPIO_PORT_PE
 *        - \b GPIO_PORT_PF
 *        - \b GPIO_PORT_PJ
 * @param  SCL_pin 产生I2C SCL信号的引脚号
 *      @arg 有效取值:
 *        - \b GPIO_PIN0
 *        - \b GPIO_PIN1
 *        - \b GPIO_PIN2
 *        - \b GPIO_PIN3
 *        - \b GPIO_PIN4
 *        - \b GPIO_PIN5
 *        - \b GPIO_PIN6
 *        - \b GPIO_PIN7
 *        - \b GPIO_PIN8
 *        - \b GPIO_PIN9
 *        - \b GPIO_PIN10
 *        - \b GPIO_PIN11
 *        - \b GPIO_PIN12
 *        - \b GPIO_PIN13
 *        - \b GPIO_PIN14
 *        - \b GPIO_PIN15
 *        - \b GPIO_PIN_ALL8
 *        - \b GPIO_PIN_ALL16
 * @param  SDA_port 产生I2C SDA信号的端口号
 *      @arg 有效取值: 同SCL_port
 * @param  SDA_pin 产生I2C SDA信号的引脚号
 *      @arg 有效取值: 同SCL_pin
 * @param  ack 决定主机是否发送应答信号。
 *     @arg 有效取值:
 *      - \b I2C_ACK : 发送应答信号（拉低SDA）
 *      - \b I2C_NO_ACK: 不发送应答信号
 * @retval 无
 */
void I2C_SendACK(uint16_t SCL_port, uint16_t SCL_pin,
                    uint16_t SDA_port, uint16_t SDA_pin, uint8_t ack)
{
    SDA_OUT(SDA_port, SDA_pin);
    SCL_OUT(SCL_port, SCL_pin);
    if (ack == I2C_ACK)
        SDA_LOW(SDA_port, SDA_pin);
    else
        SDA_HIGH(SDA_port, SDA_pin);
    SCL_HIGH(SCL_port, SCL_pin);
    I2C_delay();
    SCL_LOW(SCL_port, SCL_pin);
    I2C_delay();
}

/**
 * @brief  主机接收应答信号。
 * @param  SCL_port 产生I2C SCL信号的端口号
 *      @arg 有效取值:
 *        - \b GPIO_PORT_P1
 *        - \b GPIO_PORT_P2
 *        - \b GPIO_PORT_P3
 *        - \b GPIO_PORT_P4
 *        - \b GPIO_PORT_P5
 *        - \b GPIO_PORT_P6
 *        - \b GPIO_PORT_P7
 *        - \b GPIO_PORT_P8
 *        - \b GPIO_PORT_P9
 *        - \b GPIO_PORT_P10
 *        - \b GPIO_PORT_P11
 *        - \b GPIO_PORT_PA
 *        - \b GPIO_PORT_PB
 *        - \b GPIO_PORT_PC
 *        - \b GPIO_PORT_PD
 *        - \b GPIO_PORT_PE
 *        - \b GPIO_PORT_PF
 *        - \b GPIO_PORT_PJ
 * @param  SCL_pin 产生I2C SCL信号的引脚号
 *      @arg 有效取值:
 *        - \b GPIO_PIN0
 *        - \b GPIO_PIN1
 *        - \b GPIO_PIN2
 *        - \b GPIO_PIN3
 *        - \b GPIO_PIN4
 *        - \b GPIO_PIN5
 *        - \b GPIO_PIN6
 *        - \b GPIO_PIN7
 *        - \b GPIO_PIN8
 *        - \b GPIO_PIN9
 *        - \b GPIO_PIN10
 *        - \b GPIO_PIN11
 *        - \b GPIO_PIN12
 *        - \b GPIO_PIN13
 *        - \b GPIO_PIN14
 *        - \b GPIO_PIN15
 *        - \b GPIO_PIN_ALL8
 *        - \b GPIO_PIN_ALL16
 * @param  SDA_port 产生I2C SDA信号的端口号
 *      @arg 有效取值: 同SCL_port
 * @param  SDA_pin 产生I2C SDA信号的引脚号
 *      @arg 有效取值: 同SCL_pin
 * @retval 从机应答状态。
 *      - \b I2C_ACK : 有应答（SDA被拉低）
 *      - \b I2C_NO_ACK : 无应答
 */
uint8_t I2C_RecvACK(uint16_t SCL_port, uint16_t SCL_pin,
                        uint16_t SDA_port, uint16_t SDA_pin)
{
    uint8_t flag;
    SCL_OUT(SCL_port, SCL_pin);
    SDA_IN(SDA_port, SDA_pin);
    SCL_HIGH(SCL_port, SCL_pin);
    I2C_delay();
    if (SDA(SDA_port, SDA_pin) == I2C_ACK)
        flag = I2C_ACK;
    else
        flag = I2C_NO_ACK;
    SCL_LOW(SCL_port, SCL_pin);
    I2C_delay();
    SDA_OUT(SDA_port, SDA_pin);
    return flag;
}

/**
 * @brief  主机向IIC总线发送一个字节数据。
 * @param  SCL_port 产生I2C SCL信号的端口号
 *      @arg 有效取值:
 *        - \b GPIO_PORT_P1
 *        - \b GPIO_PORT_P2
 *        - \b GPIO_PORT_P3
 *        - \b GPIO_PORT_P4
 *        - \b GPIO_PORT_P5
 *        - \b GPIO_PORT_P6
 *        - \b GPIO_PORT_P7
 *        - \b GPIO_PORT_P8
 *        - \b GPIO_PORT_P9
 *        - \b GPIO_PORT_P10
 *        - \b GPIO_PORT_P11
 *        - \b GPIO_PORT_PA
 *        - \b GPIO_PORT_PB
 *        - \b GPIO_PORT_PC
 *        - \b GPIO_PORT_PD
 *        - \b GPIO_PORT_PE
 *        - \b GPIO_PORT_PF
 *        - \b GPIO_PORT_PJ
 * @param  SCL_pin 产生I2C SCL信号的引脚号
 *      @arg 有效取值:
 *        - \b GPIO_PIN0
 *        - \b GPIO_PIN1
 *        - \b GPIO_PIN2
 *        - \b GPIO_PIN3
 *        - \b GPIO_PIN4
 *        - \b GPIO_PIN5
 *        - \b GPIO_PIN6
 *        - \b GPIO_PIN7
 *        - \b GPIO_PIN8
 *        - \b GPIO_PIN9
 *        - \b GPIO_PIN10
 *        - \b GPIO_PIN11
 *        - \b GPIO_PIN12
 *        - \b GPIO_PIN13
 *        - \b GPIO_PIN14
 *        - \b GPIO_PIN15
 *        - \b GPIO_PIN_ALL8
 *        - \b GPIO_PIN_ALL16
 * @param  SDA_port 产生I2C SDA信号的端口号
 *      @arg 有效取值: 同SCL_port
 * @param  SDA_pin 产生I2C SDA信号的引脚号
 *      @arg 有效取值: 同SCL_pin
 * @param  byte 待发送的字节。
 * @retval 无
 */
void I2C_SendByte(uint16_t SCL_port, uint16_t SCL_pin,
                    uint16_t SDA_port, uint16_t SDA_pin, uint8_t byte)
{
    uint8_t i;
    SDA_OUT(SDA_port, SDA_pin);
    SCL_OUT(SCL_port, SCL_pin);
    for (i = 0; i < 8; i++)
    {
        if ((byte << i) & 0x80) {
            SDA_HIGH(SDA_port, SDA_pin);
        } else {
            SDA_LOW(SDA_port, SDA_pin);
        }
        I2C_delay();
        SCL_HIGH(SCL_port, SCL_pin);
        I2C_delay();
        SCL_LOW(SCL_port, SCL_pin);
    }
    I2C_RecvACK(SCL_port, SCL_pin, SDA_port, SDA_pin);
}

/**
 * @brief  主机从IIC总线接收数据。
 * @param  SCL_port 产生I2C SCL信号的端口号
 *      @arg 有效取值:
 *        - \b GPIO_PORT_P1
 *        - \b GPIO_PORT_P2
 *        - \b GPIO_PORT_P3
 *        - \b GPIO_PORT_P4
 *        - \b GPIO_PORT_P5
 *        - \b GPIO_PORT_P6
 *        - \b GPIO_PORT_P7
 *        - \b GPIO_PORT_P8
 *        - \b GPIO_PORT_P9
 *        - \b GPIO_PORT_P10
 *        - \b GPIO_PORT_P11
 *        - \b GPIO_PORT_PA
 *        - \b GPIO_PORT_PB
 *        - \b GPIO_PORT_PC
 *        - \b GPIO_PORT_PD
 *        - \b GPIO_PORT_PE
 *        - \b GPIO_PORT_PF
 *        - \b GPIO_PORT_PJ
 * @param  SCL_pin 产生I2C SCL信号的引脚号
 *      @arg 有效取值:
 *        - \b GPIO_PIN0
 *        - \b GPIO_PIN1
 *        - \b GPIO_PIN2
 *        - \b GPIO_PIN3
 *        - \b GPIO_PIN4
 *        - \b GPIO_PIN5
 *        - \b GPIO_PIN6
 *        - \b GPIO_PIN7
 *        - \b GPIO_PIN8
 *        - \b GPIO_PIN9
 *        - \b GPIO_PIN10
 *        - \b GPIO_PIN11
 *        - \b GPIO_PIN12
 *        - \b GPIO_PIN13
 *        - \b GPIO_PIN14
 *        - \b GPIO_PIN15
 *        - \b GPIO_PIN_ALL8
 *        - \b GPIO_PIN_ALL16
 * @param  SDA_port 产生I2C SDA信号的端口号
 *      @arg 有效取值: 同SCL_port
 * @param  SDA_pin 产生I2C SDA信号的引脚号
 *      @arg 有效取值: 同SCL_pin
 * @retval 接收到的一字节数据。
 */
uint8_t I2C_ReadByte(uint16_t SCL_port, uint16_t SCL_pin,
                        uint16_t SDA_port, uint16_t SDA_pin)
{
    uint8_t i, byte = 0;
    SDA_IN(SDA_port, SDA_pin);
    SCL_OUT(SCL_port, SCL_pin);
    SCL_LOW(SCL_port, SCL_pin);
    I2C_delay();
    for (i = 0; i < 8; i++) {
        SCL_HIGH(SCL_port, SCL_pin);
        byte <<= 1;
        if (SDA(SDA_port, SDA_pin))
            byte |= 1;
        SCL_LOW(SCL_port, SCL_pin);
        I2C_delay();
    }
    SDA_OUT(SDA_port, SDA_pin);
    I2C_delay();
    return byte;
}
