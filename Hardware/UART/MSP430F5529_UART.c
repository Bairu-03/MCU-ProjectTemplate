/*
 * MSP430F5529_UART.c
 *
 *  Created on: 2024��7��16��
 *      Author: Bairu
 */
#include "driverlib.h"
#include "MSP430F5529_UART.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/**
 * ����״̬��־
 * bit15��������ɱ�־��0x0a��
 * bit14�����յ�0x0d
 * bit13~bit0�����յ�����Ч�ֽ���
 */
uint16_t UART0_RX_STA = 0;
uint16_t UART1_RX_STA = 0;

/**
 * ����0���ջ������飬���USART_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з�
 * ��ȡ�괮�����ݺ���Ҫ��Reset_Uart_RecStatus(USCI_A0_BASE)��ʼ�����ڽ��ձ�־
 */
uint8_t UART0_RX_BUF[UART_REC_LEN];    // P3.3-TXD | P3.4-RXD

/**
 * ����1���ջ������飬���USART_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з�
 * ��ȡ�괮�����ݺ���Ҫ��Reset_Uart_RecStatus(USCI_A1_BASE)��ʼ�����ڽ��ձ�־
 */
uint8_t UART1_RX_BUF[UART_REC_LEN];    // P4.4-TXD | P4.5-RXD

/**
 * @brief  ������printf�ķ�ʽ�Ӵ��������ʽ���ַ�����
 *      ע�⣺��Ҫ��ӡ�����������赽Project - Properties - CCS Build - P430 Compiler - Advanced Options - Language Options��
 *      ��Level of printf/scanf support required (--printf _support)����Ϊfull
 * @param  baseAddress ���ڻ�ַ
 *      @arg ��Чȡֵ:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @param  format ��ʽ���ַ���
 * @retval ��
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
 * @brief  ��ʼ������
 * @param  baseAddress ���ڻ�ַ
 *      @arg ��Чȡֵ:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @param  Baudrate ���ڲ�����
 * @retval ��ʼ��ִ��״̬��
 *      - \b STATUS_SUCCESS : ��ʼ���ɹ�
 *      - \b STATUS_FAIL : ��ʼ��ʧ��
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

    // ʹ��UARTģ��
    USCI_A_UART_enable(baseAddress);

    // �������ڽ����ж�
    USCI_A_UART_clearInterrupt(baseAddress, USCI_A_UART_RECEIVE_INTERRUPT);
    USCI_A_UART_enableInterrupt(baseAddress, USCI_A_UART_RECEIVE_INTERRUPT);

    return STATUS_SUCCESS;
}

/**
 * @brief  �жϴ��ڽ����Ƿ���ɣ����յ�0x0D 0x0A��
 * @param  baseAddress ���ڻ�ַ
 *      @arg ��Чȡֵ:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @retval
 *      - \b 1 : �������
 *      - \b 0 : ����δ���
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

    // ��UART_RX_STA���λΪ1���������
    return (UART_RX_STA & 0x8000) ? 1 : 0;
}

/**
 * @brief  ��ȡ���ڽ�������UART_RX_BUF�ĳ���
 * @param  baseAddress ���ڻ�ַ
 *      @arg ��Чȡֵ:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @retval ���鳤��ֵ(uint16_t)
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
 * @brief  ���ô��ڽ���״̬��־��׼���´ν���
 * @param  baseAddress ���ڻ�ַ
 *      @arg ��Чȡֵ:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @retval ��
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
 * @brief  ���ڷ�������
 * @param  baseAddress ���ڻ�ַ
 *      @arg ��Чȡֵ:
 *          - \b USCI_A0_BASE -> P3.3-TXD | P3.4-RXD
 *          - \b USCI_A1_BASE -> P4.4-TXD | P4.5-RXD
 * @param  transmitData Ҫ���͵�����
 * @retval ��
 */
void UART_SendData(uint16_t baseAddress, uint8_t transmitData)
{
    USCI_A_UART_transmitData(baseAddress,transmitData);
}

/*****************************
 * USCI_A0�ж������������
 ****************************/
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR (void)
{
    uint8_t receivedData = 0;
    switch (__even_in_range(UCA0IV,4))
    {
        // Vector 2 - RXIFG
        // ���ڽ����ж�(���յ������ݱ�����0x0D 0x0A��β)
        case 2:
            receivedData = USCI_A_UART_receiveData(USCI_A0_BASE);
            if ((UART0_RX_STA & 0x8000) == 0) // ����δ���
            {
                if (UART0_RX_STA & 0x4000) // ���յ���0x0D
                {
                    if (receivedData != 0x0A)
                        UART0_RX_STA = 0; // ���մ���,���¿�ʼ
                    else
                        UART0_RX_STA |= 0x8000; // �������
                }
                else // ��û�յ�0x0D
                {
                    if (receivedData == 0x0D)
                        UART0_RX_STA |= 0x4000;
                    else
                    {
                        UART0_RX_BUF[UART0_RX_STA & 0x3FFF] = receivedData;
                        UART0_RX_STA++;
                        if (UART0_RX_STA > (UART_REC_LEN - 1))
                            UART0_RX_STA = 0; // �������ݴ���,���¿�ʼ����
                    }
                }
            }
            break;
        default:
            break;
    }
}

/*****************************
 * USCI_A1�ж������������
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
            if ((UART1_RX_STA & 0x8000) == 0) // ����δ���
            {
                if (UART1_RX_STA & 0x4000) // ���յ���0x0D
                {
                    if (receivedData != 0x0A)
                        UART1_RX_STA = 0; // ���մ���,���¿�ʼ
                    else
                        UART1_RX_STA |= 0x8000; // �������
                }
                else // ��û�յ�0x0D
                {
                    if (receivedData == 0x0D)
                        UART1_RX_STA |= 0x4000;
                    else
                    {
                        UART1_RX_BUF[UART1_RX_STA & 0x3FFF] = receivedData;
                        UART1_RX_STA++;
                        if (UART1_RX_STA > (UART_REC_LEN - 1))
                            UART1_RX_STA = 0; // �������ݴ���,���¿�ʼ����
                    }
                }
            }
            break;
        default:
            break;
    }
}
