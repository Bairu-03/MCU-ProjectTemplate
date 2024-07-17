/*
 * MSP430F5529_UART.h
 *
 *  Created on: 2024��7��16��
 *      Author: Bairu
 */

#ifndef MSP430F5529_UART_H_
#define MSP430F5529_UART_H_

#define UART_REC_LEN 200 // �����������ֽ��� 200

extern uint8_t UART0_RX_BUF[UART_REC_LEN];    // P3.3-TXD | P3.4-RXD
extern uint8_t UART1_RX_BUF[UART_REC_LEN];    // P4.4-TXD | P4.5-RXD

void UART_printf(uint16_t baseAddress, const char *format,...);
bool UART_Init(uint16_t baseAddress, uint32_t Baudrate);
uint8_t get_Uart_RecStatus(uint16_t baseAddress);
uint16_t get_Uart_RecLength(uint16_t baseAddress);
void Reset_Uart_RecStatus(uint16_t baseAddress);
void UART_SendData(uint16_t baseAddress, uint8_t transmitData);


#endif /* MSP430F5529_UART_H_ */

/************************************************************************
 *                            ����ͨ������                              *
 ************************************************************************
    #include "driverlib/MSP430F5xx_6xx/driverlib.h"
    #include "System/Sys_Clock.h"
    #include "Hardware/OLED/OLED.h"
    #include "Hardware/UART/MSP430F5529_UART.h"

    void main(void)
    {
        WDT_A_hold(WDT_A_BASE);
        SystemClock_Init();

        __bis_SR_register(GIE);             // ʹ�����ж�

        OLED_Init();                        // OLED��ʼ��

        UART_Init(USCI_A0_BASE, 115200);    // ����0��ʼ��
        UART_Init(USCI_A1_BASE, 115200);    // ����1��ʼ��

        OLED_ShowString(1, 1, "UART0:", 8);
        OLED_ShowString(5, 1, "UART1:", 8);

        while(1)
        {
            // ������0�������
            if(get_Uart_RecStatus(USCI_A0_BASE))
            {
                OLED_ShowString(3, 1, "                ", 8);
                uint8_t i;
                // ��ȡUART0_RX_BUF�ĳ��ȣ�����
                for(i = 0; i < get_Uart_RecLength(USCI_A0_BASE); i++)
                {
                    // ��ȡUART0_RX_BUF�е����ݣ����ڻش���OLED��ʾ
                    UART_SendData(USCI_A0_BASE, UART0_RX_BUF[i]);
                    OLED_ShowChar(3, (8 * i + 1), UART0_RX_BUF[i], 8);
                }
                // ���ô��ڽ���״̬��־��׼���´ν���
                Reset_Uart_RecStatus(USCI_A0_BASE);
            }

            // ������1�������
            if(get_Uart_RecStatus(USCI_A1_BASE))
            {
                OLED_ShowString(7, 1, "                ", 8);
                uint8_t i;
                // ��ȡUART1_RX_BUF�ĳ��ȣ�����
                for(i = 0; i < get_Uart_RecLength(USCI_A1_BASE); i++)
                {
                    // ��ȡUART1_RX_BUF�е����ݣ����ڻش���OLED��ʾ
                    UART_SendData(USCI_A1_BASE, UART1_RX_BUF[i]);
                    OLED_ShowChar(7, (8 * i + 1), UART1_RX_BUF[i], 8);
                }
                // ���ô��ڽ���״̬��־��׼���´ν���
                Reset_Uart_RecStatus(USCI_A1_BASE);
            }
        }
    }
 ************************************************************************/
