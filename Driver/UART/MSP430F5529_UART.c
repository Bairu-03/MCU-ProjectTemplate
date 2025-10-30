/**
 * @file    MSP430F5529_UART.c
 * @version v1.1
 * @author  Bairu
 * @date    2024年7月16日
 * @brief   MSP430F5529串口驱动
 */

#include "driverlib.h"
#include "MSP430F5529_UART.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/**
 * 串口0、串口1接收状态标志。
 * bit15，接收到0x0a置1，接收完成；
 * bit14，接收到0x0d置1；
 * bit13~bit0，接收到的有效字节数。
 */
uint16_t UART0_RX_STA = 0;
uint16_t UART1_RX_STA = 0;

/**
 * P3.3-TXD | P3.4-RXD
 * 串口0接收缓冲数组，最大USART_REC_LEN个字节，末字节为换行符
 * 在取完串口数据后，需要用Reset_Uart_RecStatus(USCI_A0_BASE)初始化串口接收标志
 */
uint8_t UART0_RX_BUF[UART_REC_LEN];    // P3.3-TXD | P3.4-RXD

/**
 * P4.4-TXD | P4.5-RXD
 * 串口1接收缓冲数组，最大USART_REC_LEN个字节，末字节为换行符
 * 在取完串口数据后，需要用Reset_Uart_RecStatus(USCI_A1_BASE)初始化串口接收标志
 */
uint8_t UART1_RX_BUF[UART_REC_LEN];    // P4.4-TXD | P4.5-RXD

/**
 * @brief  用类似printf的方式从串口输出格式化字符串。
 *      注意：若要打印浮点数，则需到Project - Properties - CCS Build - P430 Compiler - Advanced Options - Language Options中
 *      将Level of printf/scanf support required (--printf _support)项设为full
 * @param  baseAddress 串口基址
 *      @arg 有效取值:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @param  format 格式化字符串
 * @retval 无
 */
void UART_printf(uint16_t baseAddress, const char *format,...)
{
    uint32_t length;
    va_list args;
    uint32_t i;
    char TxBuffer[128] = {0};

    va_start(args, format);
    length = vsnprintf((char*)TxBuffer, sizeof(TxBuffer)+1, (char*)format, args);
    va_end(args);

    for(i = 0; i < length; i++)
        USCI_A_UART_transmitData(baseAddress, TxBuffer[i]);
}

/**
 * @brief  初始化串口
 * @param  baseAddress 串口基址
 *      @arg 有效取值:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @param  Baudrate 串口波特率
 * @retval 初始化执行状态。
 *      - \b STATUS_SUCCESS : 初始化成功
 *      - \b STATUS_FAIL : 初始化失败
 */
bool UART_Init(uint16_t baseAddress, uint32_t Baudrate)
{
    float UART_Temp = 0;
    USCI_A_UART_initParam huart = {0};

    if(baseAddress == USCI_A0_BASE)         // USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
    {
        GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN3);
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN4);
    }
    else if(baseAddress == USCI_A1_BASE)    // USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
    {
        GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4, GPIO_PIN4);
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN5);
    }

    if(Baudrate <= 9600)
    {
        huart.selectClockSource = USCI_A_UART_CLOCKSOURCE_ACLK;
        UART_Temp = (float)UCS_getACLK()/Baudrate;
    }
    else
    {
        huart.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
        UART_Temp = (float)UCS_getSMCLK()/Baudrate;
    }

    if(UART_Temp < 16)
        huart.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;
    else
    {
        huart.overSampling = USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;
        UART_Temp /= 16;
    }

    huart.clockPrescalar = (int)UART_Temp;

    if(huart.overSampling == USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION)
    {
        huart.secondModReg = (int)((UART_Temp - huart.clockPrescalar) * 8);
    }
    else
    {
        huart.firstModReg = (int)((UART_Temp - huart.clockPrescalar) * 16);
    }

    huart.parity = USCI_A_UART_NO_PARITY;
    huart.msborLsbFirst = USCI_A_UART_LSB_FIRST;
    huart.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
    huart.uartMode = USCI_A_UART_MODE;

    if (STATUS_FAIL == USCI_A_UART_init(baseAddress, &huart))
    {
        return STATUS_FAIL;
    }

    // 使能UART模块
    USCI_A_UART_enable(baseAddress);

    // 开启串口接收中断
    USCI_A_UART_clearInterrupt(baseAddress, USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(baseAddress, USCI_A_UART_RECEIVE_INTERRUPT);

    return STATUS_SUCCESS;
}

/**
 * @brief  判断串口接收是否完成（接收到0x0D 0x0A）
 * @param  baseAddress 串口基址
 *      @arg 有效取值:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @retval
 *      - \b 1 : 接收完成
 *      - \b 0 : 接收未完成
 */
uint8_t get_Uart_RecStatus(uint16_t baseAddress)
{
    uint16_t UART_RX_STA = 0;

    if(baseAddress == USCI_A0_BASE)    // USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
    {
        UART_RX_STA = UART0_RX_STA;
    }

    if(baseAddress == USCI_A1_BASE)    // USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
    {
        UART_RX_STA = UART1_RX_STA;
    }

    // 若UART_RX_STA最高位为1，接收完成
    return (UART_RX_STA & 0x8000) ? 1 : 0;
}

/**
 * @brief  获取串口接收到的数据的长度
 * @param  baseAddress 串口基址
 *      @arg 有效取值:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @retval 数组长度值(uint16_t)
 */
uint16_t get_Uart_RecLength(uint16_t baseAddress)
{
    uint16_t UART_RX_STA;
    if(baseAddress == USCI_A0_BASE)         // USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
    {
        UART_RX_STA = UART0_RX_STA;
    }
    else if(baseAddress == USCI_A1_BASE)    // USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
    {
        UART_RX_STA = UART1_RX_STA;
    }
    return (uint16_t)(UART_RX_STA & 0x3FFF);
}

/**
 * @brief  重置串口接收状态标志，准备下次接收
 * @param  baseAddress 串口基址
 *      @arg 有效取值:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @retval 无
 */
void Reset_Uart_RecStatus(uint16_t baseAddress)
{
    if(baseAddress == USCI_A0_BASE)    // USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
    {
        UART0_RX_STA = 0;
    }

    if(baseAddress == USCI_A1_BASE)    // USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
    {
        UART1_RX_STA = 0;
    }
}

/**
 * @brief  串口发送数据
 * @param  baseAddress 串口基址
 *      @arg 有效取值:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @param  transmitData 要发送的数据
 * @retval 无
 */
void UART_SendData(uint16_t baseAddress, uint8_t transmitData)
{
    USCI_A_UART_transmitData(baseAddress,transmitData);
}

/*****************************
 * USCI_A0中断向量服务程序
 ****************************/
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR (void)
{
    uint8_t receivedData = 0;
    switch (__even_in_range(UCA0IV,4))
    {
        // Vector 2 - RXIFG
        // 串口接收中断(接收到的数据必须是0x0D 0x0A结尾)
        case 2:
            receivedData = USCI_A_UART_receiveData(USCI_A0_BASE);
            if ((UART0_RX_STA & 0x8000) == 0) // 接收未完成
            {
                if (UART0_RX_STA & 0x4000) // 接收到了0x0D
                {
                    if (receivedData != 0x0A)
                        UART0_RX_STA = 0; // 接收错误,重新开始
                    else
                        UART0_RX_STA |= 0x8000; // 接收完成
                }
                else // 还没收到0x0D
                {
                    if (receivedData == 0x0D)
                        UART0_RX_STA |= 0x4000;
                    else
                    {
                        UART0_RX_BUF[UART0_RX_STA & 0x3FFF] = receivedData;
                        UART0_RX_STA++;
                        if (UART0_RX_STA > (UART_REC_LEN - 1))
                            UART0_RX_STA = 0; // 接收数据错误,重新开始接收
                    }
                }
            }
            break;
        default:
            break;
    }
}

/*****************************
 * USCI_A1中断向量服务程序
 ****************************/
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR (void)
{
    uint8_t receivedData = 0;
    switch (__even_in_range(UCA1IV,4))
    {
        //Vector 2 - RXIFG
        case 2:
            receivedData = USCI_A_UART_receiveData(USCI_A1_BASE);
            if ((UART1_RX_STA & 0x8000) == 0) // 接收未完成
            {
                if (UART1_RX_STA & 0x4000) // 接收到了0x0D
                {
                    if (receivedData != 0x0A)
                        UART1_RX_STA = 0; // 接收错误,重新开始
                    else
                        UART1_RX_STA |= 0x8000; // 接收完成
                }
                else // 还没收到0x0D
                {
                    if (receivedData == 0x0D)
                        UART1_RX_STA |= 0x4000;
                    else
                    {
                        UART1_RX_BUF[UART1_RX_STA & 0x3FFF] = receivedData;
                        UART1_RX_STA++;
                        if (UART1_RX_STA > (UART_REC_LEN - 1))
                            UART1_RX_STA = 0; // 接收数据错误,重新开始接收
                    }
                }
            }
            break;
        default:
            break;
    }
}
