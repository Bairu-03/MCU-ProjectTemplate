/*
 * MSP430F5529_UART.h
 *
 *  Created on: 2024年7月16日
 *      Author: Bairu
 */

#ifndef MSP430F5529_UART_H_
#define MSP430F5529_UART_H_

#define UART_REC_LEN 200 // 定义最大接收字节数 200

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
 *                            串口通信例程                              *
 ************************************************************************
    #include "driverlib/MSP430F5xx_6xx/driverlib.h"
    #include "System/Sys_Clock.h"
    #include "Hardware/OLED/OLED.h"
    #include "Hardware/UART/MSP430F5529_UART.h"

    void main(void)
    {
        WDT_A_hold(WDT_A_BASE);
        SystemClock_Init();

        __bis_SR_register(GIE);             // 使能总中断

        OLED_Init();                        // OLED初始化

        UART_Init(USCI_A0_BASE, 115200);    // 串口0初始化
        UART_Init(USCI_A1_BASE, 115200);    // 串口1初始化

        OLED_ShowString(1, 1, "UART0:", 8);
        OLED_ShowString(5, 1, "UART1:", 8);

        while(1)
        {
            // 若串口0接收完成
            if(get_Uart_RecStatus(USCI_A0_BASE))
            {
                OLED_ShowString(3, 1, "                ", 8);
                uint8_t i;
                // 获取UART0_RX_BUF的长度，遍历
                for(i = 0; i < get_Uart_RecLength(USCI_A0_BASE); i++)
                {
                    // 读取UART0_RX_BUF中的内容，串口回传，OLED显示
                    UART_SendData(USCI_A0_BASE, UART0_RX_BUF[i]);
                    OLED_ShowChar(3, (8 * i + 1), UART0_RX_BUF[i], 8);
                }
                // 重置串口接收状态标志，准备下次接收
                Reset_Uart_RecStatus(USCI_A0_BASE);
            }

            // 若串口1接收完成
            if(get_Uart_RecStatus(USCI_A1_BASE))
            {
                OLED_ShowString(7, 1, "                ", 8);
                uint8_t i;
                // 获取UART1_RX_BUF的长度，遍历
                for(i = 0; i < get_Uart_RecLength(USCI_A1_BASE); i++)
                {
                    // 读取UART1_RX_BUF中的内容，串口回传，OLED显示
                    UART_SendData(USCI_A1_BASE, UART1_RX_BUF[i]);
                    OLED_ShowChar(7, (8 * i + 1), UART1_RX_BUF[i], 8);
                }
                // 重置串口接收状态标志，准备下次接收
                Reset_Uart_RecStatus(USCI_A1_BASE);
            }
        }
    }
 ************************************************************************/
