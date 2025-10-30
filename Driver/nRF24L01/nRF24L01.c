/*
 * nRF24L01.c
 *
 *  Created on: 2024年7月20日
 *      Author: Bairu
 */

#include "driverlib.h"
#include "nRF24L01.h"
#include "Sys_Clock.h"
#include "math.h"

#define TX_ADR_WIDTH 5    // 5字节地址宽度
#define RX_ADR_WIDTH 5    // 5字节地址宽度
#define TX_PLOAD_WIDTH 32 // 32字节有效数据宽度
#define RX_PLOAD_WIDTH 32 // 32字节有效数据宽度

const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/**
 * @brief  初始化SPI接口（软件模拟）
 * @param  无
 * @retval 无
 */
void SPI_Sim_Init(void)
{
    GPIO_setAsInputPinWithPullUpResistor(IRQ_Port, IRQ_Pin);
    GPIO_setAsOutputPin(MOSI_Port, MOSI_Pin);
    GPIO_setAsOutputPin(CSN_Port, CSN_Pin);
    GPIO_setAsInputPinWithPullUpResistor(MISO_Port, MISO_Pin);
    GPIO_setAsOutputPin(SCK_Port, SCK_Pin);
    GPIO_setAsOutputPin(CE_Port, CE_Pin);
}

/**
 * @brief  SPI交换字节数据
 * @param  Byte 待发送的字节数据
 * @retval 读取到的字节数据
 */
uint8_t SPI_SwapByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        if ((uint8_t)(Byte & 0x80) == 0x80)
        {
            MOSI_1;
        }
        else
        {
            MOSI_0;
        }
        Byte = (Byte << 1);
        delay_us(5);
        SCK_1;
        Byte |= MISO_Read;
        SCK_0;
    }
    return Byte;
}

/**
 * @brief  向nRF24L01寄存器写数据
 * @param  Reg 寄存器地址
 * @param  Value 待发送数据
 * @retval 执行状态
 */
uint8_t NRF24L01_Write_Reg(uint8_t Reg, uint8_t Value)
{
    uint8_t Status;

    CSN_0;
    Status = SPI_SwapByte(Reg);
    SPI_SwapByte(Value);
    CSN_1;

    return Status;
}

/**
 * @brief  从nRF24L01寄存器读数据
 * @param  Reg 寄存器地址
 * @retval 读取到的数据
 */
uint8_t NRF24L01_Read_Reg(uint8_t Reg)
{
    uint8_t Value;

    CSN_0;
    SPI_SwapByte(Reg);
    Value = SPI_SwapByte(NOP);
    CSN_1;

    return Value;
}

/**
 * @brief  从nRF24L01读数据到缓冲数组
 * @param  Reg 寄存器地址
 * @param  Buf 数组地址，保存从寄存器读出的数据
 * @param  Len 要读出的字节个数
 * @retval 执行状态
 */
uint8_t NRF24L01_Read_Buf(uint8_t Reg, uint8_t *Buf, uint8_t Len)
{
    uint8_t Status, i;
    CSN_0;
    Status = SPI_SwapByte(Reg);
    for (i = 0; i < Len; i++)
    {
        Buf[i] = SPI_SwapByte(NOP);
    }
    CSN_1;
    return Status;
}

/**
 * @brief  向nRF24L01发送一组数据
 * @param  Reg 寄存器地址
 * @param  Buf 数组地址，存放要写入寄存器的数据
 * @param  Len 要发送的字节个数
 * @retval 执行状态
 */
uint8_t NRF24L01_Write_Buf(uint8_t Reg, uint8_t *Buf, uint8_t Len)
{
    uint8_t Status, i;
    CSN_0;
    Status = SPI_SwapByte(Reg);
    for (i = 0; i < Len; i++)
    {
        SPI_SwapByte(*Buf++);
    }
    CSN_1;
    return Status;
}

/**
 * @brief  读出接收到的多字节数据
 * @param  Buf 数组地址，保存接收到的数据
 * @retval 状态值
 */
uint8_t NRF24L01_GetRxBuf(uint8_t *Buf)
{
    uint8_t State;
    State = NRF24L01_Read_Reg(STATUS);
    NRF24L01_Write_Reg(nRF_WRITE_REG + STATUS, State);
    if (State & RX_OK)
    {
        CE_1;
        NRF24L01_Read_Buf(RD_RX_PLOAD, Buf, RX_PLOAD_WIDTH);
        NRF24L01_Write_Reg(FLUSH_RX, NOP);
        CE_1;
        delay_us(150);
        return 0;
    }
    return 1;
}

/**
 * @brief  发送多字节数据
 * @param  Buf 数组地址，存放待发送的数据
 * @retval 状态值
 */
uint8_t NRF24L01_SendTxBuf(uint8_t *Buf)
{
    uint8_t State;

    CE_0;
    NRF24L01_Write_Buf(WR_TX_PLOAD, Buf, TX_PLOAD_WIDTH);
    CE_1;
    while (IRQ_Read == 1);
    State = NRF24L01_Read_Reg(STATUS);
    NRF24L01_Write_Reg(nRF_WRITE_REG + STATUS, State);
    if (State & MAX_TX)
    {
        NRF24L01_Write_Reg(FLUSH_TX, NOP);
        return MAX_TX;
    }
    if (State & TX_OK)
    {
        return TX_OK;
    }
    return NOP;
}

/**
 * @brief  检测nRF24L01是否存在
 * @param  无
 * @retval 检测结果
        - \b 0 : 存在
        - \b 1 : 不存在
 */
uint8_t NRF24L01_Check(void)
{
    uint8_t check_in_buf[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
    uint8_t check_out_buf[5] = {0x00};

    SCK_0;
    CSN_1;
    CE_0;

    NRF24L01_Write_Buf(nRF_WRITE_REG + TX_ADDR, check_in_buf, 5);

    NRF24L01_Read_Buf(nRF_READ_REG + TX_ADDR, check_out_buf, 5);

    if ((check_out_buf[0] == 0x11)
            && (check_out_buf[1] == 0x22)
            && (check_out_buf[2] == 0x33)
            && (check_out_buf[3] == 0x44)
            && (check_out_buf[4] == 0x55)
        ) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * @brief  nRF24L01收发初始化
 * @param  无
 * @retval 无
 */
void NRF24L01_RT_Init(void)
{
    CE_0;
    NRF24L01_Write_Reg(nRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);
    NRF24L01_Write_Reg(FLUSH_RX, NOP);
    NRF24L01_Write_Buf(nRF_WRITE_REG + TX_ADDR, (uint8_t *)TX_ADDRESS, TX_ADR_WIDTH);
    NRF24L01_Write_Buf(nRF_WRITE_REG + RX_ADDR_P0, (uint8_t *)RX_ADDRESS, RX_ADR_WIDTH);
    NRF24L01_Write_Reg(nRF_WRITE_REG + EN_AA, 0x01);
    NRF24L01_Write_Reg(nRF_WRITE_REG + EN_RXADDR, 0x01);
    NRF24L01_Write_Reg(nRF_WRITE_REG + SETUP_RETR, 0x1A);
    NRF24L01_Write_Reg(nRF_WRITE_REG + RF_CH, 0);
    NRF24L01_Write_Reg(nRF_WRITE_REG + RF_SETUP, 0x0F);
    NRF24L01_Write_Reg(nRF_WRITE_REG + nRF24L01_CONFIG, 0x0F);
    CE_1;
}

/**
 * @brief  nRF24L01初始化（总）
 * @param  无
 * @retval 无
 */
void NRF24L01_Init(void)
{
    SPI_Sim_Init();
    while (NRF24L01_Check())
        ;
    NRF24L01_RT_Init();
}

/**
 * @brief  NRF24L01发送多字节数据
 * @param  Buf 数组地址，保存待发送的数据
 * @retval 无
 */
void NRF24L01_SendBuf(uint8_t *Buf)
{
    CE_0;
    NRF24L01_Write_Reg(nRF_WRITE_REG + nRF24L01_CONFIG, 0x0E);
    CE_1;
    delay_us(150);
    NRF24L01_SendTxBuf(Buf);
    CE_0;
    NRF24L01_Write_Reg(nRF_WRITE_REG + nRF24L01_CONFIG, 0x0F);
    CE_1;
}

/**
 * @brief  获取nRF24L01中断标志
 * @param  无
 * @retval 标志值
        - \b 0 : 接收到数据
        - \b 1 : 未接收到数据
 */
uint8_t NRF24L01_Get_Value_Flag(void)
{
    return IRQ_Read;
}

/**
 * @brief  发送数字
 * @param  Num 待发送数字
 * @retval 无
 */
void NRF24L01_SendNum(uint32_t Num)
{
    uint8_t i, SendBuf[11] = {0};
    SendBuf[0] = 10;
    for (i = 0; i < 10; i ++)
    {
        SendBuf[i + 1] = Num / (uint32_t)pow(10, 9 - i) % 10;
    }

    NRF24L01_SendBuf(SendBuf);
}

/**
 * @brief  接收数字
 * @param  无
 * @retval 接收到的数字
 */
uint32_t NRF24L01_GetNum(void)
{
    uint8_t i, Buf[33] = {0};
    uint32_t Num = 0;
    NRF24L01_GetRxBuf(Buf);
    for (i = 0; i < 10; i ++)
    {
        Num += Buf[i + 1] * pow(10, 9 - i);
    }

    return Num;
}
