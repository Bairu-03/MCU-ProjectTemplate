#include "driverlib.h"
#include "MSP430F5529_I2C.h"
#include <stdint.h>
#include <stdint.h>

#define I2C_USCI_BASE USCI_B0_BASE
#define I2C_USCI_VECTOR USCI_B0_VECTOR
#define I2C_USCI_IV UCB0IV

#define I2C_BUF_LENGTH 32
static char i2c_buf[I2C_BUF_LENGTH];
static uint8_t i2c_buf_len = 0;
static uint8_t i2c_buf_cur = 0;

static uint8_t *i2c_rx_buf = 0;
static uint8_t i2c_rx_buf_len = 0;

/**
 * @brief  初始化I2C。
 *       P3.0 - SDA | P3.1 - SCL
 * @param  slaveAddress 从机地址
 * @retval 无
 */
void I2C_init(uint8_t slaveAddress)
{
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN0 | GPIO_PIN1);

    /* I2C 主配置参数 */
    USCI_B_I2C_initMasterParam i2cConfig =
    {
    USCI_B_I2C_CLOCKSOURCE_SMCLK,
    UCS_getSMCLK(),
    USCI_B_I2C_SET_DATA_RATE_400KBPS
    };
    /* 初始化USCI_B0和I2C 主机与从设备通信*/
    USCI_B_I2C_initMaster(I2C_USCI_BASE, &i2cConfig);

    /* 使能I2C模块开始操作 */
    USCI_B_I2C_enable(I2C_USCI_BASE);

    // 指定从机地址
    USCI_B_I2C_setSlaveAddress(I2C_USCI_BASE, slaveAddress);
    USCI_B_I2C_clearInterrupt(I2C_USCI_BASE, USCI_B_I2C_RECEIVE_INTERRUPT);
    USCI_B_I2C_enableInterrupt(I2C_USCI_BASE, USCI_B_I2C_RECEIVE_INTERRUPT);
    USCI_B_I2C_clearInterrupt(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
    USCI_B_I2C_enableInterrupt(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);

    return;
}

/* 向特定寄存器写入一个字节，不能在中断上下文中调用 */
void writeByte(uint8_t byte)
{
    while (USCI_B_I2C_isBusBusy(I2C_USCI_BASE));

    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_MODE);

    // 启动并发送第一个字符
    i2c_buf[0] = byte;
    i2c_buf_cur = 1;
    i2c_buf_len = 1;
    USCI_B_I2C_masterSendMultiByteStart(I2C_USCI_BASE, i2c_buf[0]);
}

/* 向特定寄存器写入一个字，不能在中断上下文中调用 */
void writeWord(uint16_t word)
{
    while (USCI_B_I2C_isBusBusy(I2C_USCI_BASE));

    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_MODE);

    // 启动并发送第一个字符
    i2c_buf[0] = word >> 8;
    i2c_buf[1] = (uint8_t)word;
    i2c_buf_cur = 1;
    i2c_buf_len = 2;
    USCI_B_I2C_masterSendMultiByteStart(I2C_USCI_BASE, i2c_buf[0]);
}

///* 设置/清除特定寄存器的某些位，不能在中断上下文中调用 */
//void writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data)
//{
//    uint8_t b = 0;
//    readByte(regAddr, &b);
//    delay_ms(2);
//    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
//    writeByte(regAddr, b);
//}

/* 从特定寄存器读取一些字节，无法在中断上下文中调用 */
void readByte(uint8_t RegAddr, uint8_t* b)
{
    while (USCI_B_I2C_isBusBusy(I2C_USCI_BASE));

    // 发送地址
    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_MODE);
    i2c_buf_cur = 1;
    i2c_buf_len = 1;
    USCI_B_I2C_masterSendSingleByte(I2C_USCI_BASE, RegAddr);

    // 接收
    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_RECEIVE_MODE);
    i2c_rx_buf = b;
    i2c_rx_buf_len = 1;
    USCI_B_I2C_masterReceiveSingleStart(I2C_USCI_BASE);
}

void readBytes(uint8_t RegAddr, uint8_t length, uint8_t* data)
{
    while (USCI_B_I2C_isBusBusy(I2C_USCI_BASE));
    // 发送地址
    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_TRANSMIT_MODE);
    i2c_buf_cur = 1;
    i2c_buf_len = 1;
    USCI_B_I2C_masterSendSingleByte(I2C_USCI_BASE, RegAddr);

    // 接收
    USCI_B_I2C_setMode(I2C_USCI_BASE, USCI_B_I2C_RECEIVE_MODE);
    i2c_rx_buf = data;
    i2c_rx_buf_len = length;
    USCI_B_I2C_masterReceiveMultiByteStart(I2C_USCI_BASE);
}

#pragma vector = I2C_USCI_VECTOR
__interrupt void USCI_B0_ISR(void)
{
    switch (__even_in_range(I2C_USCI_IV, 12))
    {
        case USCI_I2C_UCTXIFG:
            if (i2c_buf_cur < i2c_buf_len)
            {
                USCI_B_I2C_masterSendMultiByteNext( I2C_USCI_BASE, i2c_buf[i2c_buf_cur]);
                i2c_buf_cur++;
            }
            else
            {
                USCI_B_I2C_masterSendMultiByteStop(I2C_USCI_BASE);
                // 清除主中断状态
                USCI_B_I2C_clearInterrupt(I2C_USCI_BASE,
                                          USCI_B_I2C_TRANSMIT_INTERRUPT);
            }
            break;
        case USCI_I2C_UCRXIFG:
            i2c_rx_buf_len--;
            if(i2c_rx_buf_len)
            {
                if(i2c_rx_buf_len== 1)
                {
                    // 启动接收结束 -> 接收带有 NAK 的字节
                    *i2c_rx_buf++ = USCI_B_I2C_masterReceiveMultiByteFinish( I2C_USCI_BASE);
                }
                else
                {
                    // 保持一次接收一个字节
                    *i2c_rx_buf++ = USCI_B_I2C_masterReceiveMultiByteNext( I2C_USCI_BASE);
                }
            }
            else
            {
                // 接收最后一个字节
                *i2c_rx_buf= USCI_B_I2C_masterReceiveMultiByteNext(I2C_USCI_BASE);
            }
            break;
    }
}
