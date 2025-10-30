/*
 * nRF24L01.h
 *
 *  Created on: 2024年7月20日
 *      Author: Bairu
 */

#ifndef NRF24L01_H_
#define NRF24L01_H_

/**********  NRF24L01引脚定义  ***********/
// IRQ  - P2.7 |  MISO - P7.0
// MOSI - P4.2 |  SCK  - P3.6
// CSN  - P4.1 |  CE   - P3.5
// VCC  - VCC  |  GND  - GND

#define  IRQ_Port   GPIO_PORT_P2
#define  MOSI_Port  GPIO_PORT_P4
#define  CSN_Port   GPIO_PORT_P4
#define  MISO_Port  GPIO_PORT_P7
#define  SCK_Port   GPIO_PORT_P3
#define  CE_Port    GPIO_PORT_P3

#define  IRQ_Pin    GPIO_PIN7
#define  MOSI_Pin   GPIO_PIN2
#define  CSN_Pin    GPIO_PIN1
#define  MISO_Pin   GPIO_PIN0
#define  SCK_Pin    GPIO_PIN6
#define  CE_Pin     GPIO_PIN5

/**********************  SPI通信引脚动作  ***********************/
#define  IRQ_Read   (GPIO_getInputPinValue(IRQ_Port, IRQ_Pin))

#define  MOSI_1     GPIO_setOutputHighOnPin(MOSI_Port, MOSI_Pin)
#define  MOSI_0     GPIO_setOutputLowOnPin(MOSI_Port, MOSI_Pin)

#define  CSN_1      GPIO_setOutputHighOnPin(CSN_Port, CSN_Pin)
#define  CSN_0      GPIO_setOutputLowOnPin(CSN_Port, CSN_Pin)

#define  MISO_Read  (GPIO_getInputPinValue(MISO_Port, MISO_Pin))

#define  SCK_1      GPIO_setOutputHighOnPin(SCK_Port, SCK_Pin)
#define  SCK_0      GPIO_setOutputLowOnPin(SCK_Port, SCK_Pin)

#define  CE_1       GPIO_setOutputHighOnPin(CE_Port, CE_Pin)
#define  CE_0       GPIO_setOutputLowOnPin(CE_Port, CE_Pin)

/**********  NRF24L01寄存器操作命令  ***********/
#define  nRF_READ_REG     0x00    // 定义寄存器的读命令
#define  nRF_WRITE_REG    0x20    // 定义寄存器的写命令
#define  RD_RX_PLOAD      0x61    // 定义 RX 负载寄存器地址
#define  WR_TX_PLOAD      0xA0    // 定义TX有效负载寄存器地址
#define  FLUSH_TX         0xE1    // 定义刷新TX寄存器命令
#define  FLUSH_RX         0xE2    // 定义刷新 RX 寄存器命令
#define  REUSE_TX_PL      0xE3    // 定义重用 TX 有效负载寄存器命令
#define  NOP              0xFF    // 定义空操作，可用于读取状态寄存器

/**********  NRF24L01寄存器地址  *************/
#define  nRF24L01_CONFIG    0x00    // "配置"寄存器地址
#define  EN_AA              0x01    // "启用自动确认”寄存器地址
#define  EN_RXADDR          0x02    // "启用 RX 地址”寄存器地址
#define  SETUP_AW           0x03    // "设置地址宽度"寄存器地址
#define  SETUP_RETR         0x04    // "设置自动重传"寄存器地址
#define  RF_CH              0x05    // "RF通道"寄存器地址
#define  RF_SETUP           0x06    // "RF设置"寄存器地址
#define  STATUS             0x07    // "状态"寄存器地址
#define  OBSERVE_TX         0x08    // "监听TX"寄存器地址
#define  CD                 0x09    // "载波检测"寄存器地址
#define  RX_ADDR_P0         0x0A    // "RX地址管道0"寄存器地址
#define  RX_ADDR_P1         0x0B    // "RX地址管道1"寄存器地址
#define  RX_ADDR_P2         0x0C    // "RX地址管道2"寄存器地址
#define  RX_ADDR_P3         0x0D    // "RX地址管道3"寄存器地址
#define  RX_ADDR_P4         0x0E    // "RX地址管道4"寄存器地址
#define  RX_ADDR_P5         0x0F    // "RX地址管道5"寄存器地址
#define  TX_ADDR            0x10    // "TX地址"寄存器地址
#define  RX_PW_P0           0x11    // "RX有效负载宽度，管道0"寄存器地址
#define  RX_PW_P1           0x12    // "RX有效负载宽度，管道1"寄存器地址
#define  RX_PW_P2           0x13    // "RX有效负载宽度，管道2"寄存器地址
#define  RX_PW_P3           0x14    // "RX有效负载宽度，管道3"寄存器地址
#define  RX_PW_P4           0x15    // "RX有效负载宽度，管道4"寄存器地址
#define  RX_PW_P5           0x16    // "RX有效负载宽度，管道5"寄存器地址
#define  FIFO_STATUS        0x17    // "FIFO状态寄存器"寄存器地址

/**********  STATUS寄存器bit位定义  ***********/
#define  MAX_TX  0x10 // 达到最大发送次数中断
#define  TX_OK   0x20  // TX发送完成中断
#define  RX_OK   0x40  // 接收到数据中断


void SPI_Sim_Init(void);
uint8_t SPI_SwapByte(uint8_t Byte);
uint8_t NRF24L01_Write_Reg(uint8_t Reg, uint8_t Value);
uint8_t NRF24L01_Read_Reg(uint8_t Reg);
uint8_t NRF24L01_Read_Buf(uint8_t Reg, uint8_t *Buf, uint8_t Len);
uint8_t NRF24L01_Write_Buf(uint8_t Reg, uint8_t *Buf, uint8_t Len);
uint8_t NRF24L01_GetRxBuf(uint8_t *Buf);
uint8_t NRF24L01_SendTxBuf(uint8_t *Buf);
uint8_t NRF24L01_Check(void);
void NRF24L01_RT_Init(void);
void NRF24L01_Init(void);
void NRF24L01_SendBuf(uint8_t *Buf);
uint8_t NRF24L01_Get_Value_Flag(void);
void NRF24L01_SendNum(uint32_t Num);
uint32_t NRF24L01_GetNum(void);

#endif /* NRF24L01_H_ */
