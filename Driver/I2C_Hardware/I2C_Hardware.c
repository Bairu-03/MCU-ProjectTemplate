#include "I2C_Hardware.h"

/**
 * @brief  硬件I2C初始化。
 * @note   SCL: PB10
 * @note   SDA: PB11
 * @param  无
 * @retval 无
 */
void I2C_HW_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_ClockSpeed = 400000; // 400kHz
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C2, &I2C_InitStructure);
    I2C_Cmd(I2C2, ENABLE);
}

/**
 * @brief  I2C起始信号。
 * @param  无
 * @retval 无
 */
void I2C_HW_Start(void)
{
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
        ;
    I2C_GenerateSTART(I2C2, ENABLE);
}

/**
 * @brief  I2C发送地址以选择从机。
 * @param  addr  从机地址。
 * @retval 无
 */
void I2C_HW_SendAddr(uint8_t addr)
{
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
        ;
    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Transmitter);
}

/**
 * @brief  I2C发送一个字节数据。
 * @param  Byte  要发送的一个字节数据。
 * @retval 无
 */
void I2C_HW_SendByte(uint8_t data)
{
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        ;
    I2C_SendData(I2C2, data);
}

/**
 * @brief  I2C停止信号。
 * @param  无
 * @retval 无
 */
void I2C_HW_Stop(void)
{
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;
    I2C_GenerateSTOP(I2C2, ENABLE);
}
