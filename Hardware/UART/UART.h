#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"

#define UART_REC_LEN 255 // 定义最大接收字节数 200

#define UART_SEND_LEN 255 // UART发送缓冲区大小

extern uint8_t UART_RX_BUF[UART_REC_LEN];

void UART_Init(uint32_t baud);
void UART_SendData(uint16_t data);
void UART_printf(const char *format, ...);
uint8_t UART_GetRecStatus(void);
uint16_t UART_GetRecLength(void);
void UART_ResetRecStatus(void);

#endif

/**
  ***************************************************
  * @example 串口收发数据例程
  * @brief   实现串口接收数据并原样返回
  ***************************************************
    UART_Init(115200);

    // 1.串口接收数据并原样返回
    if (UART_GetRecStatus())
    {
        uint8_t t;
        for (t = 0; t < UART_GetRecLength(); t++)
        {
            UART_SendData(UART_RX_BUF[t]);
        }
        UART_ResetRecStatus();
    }

    // 2.串口接收数据并原样返回，同时显示在OLED上
    if (UART_GetRecStatus())
    {
        uint8_t t;
        OLED_ClearLine(1, 2);
        for (t = 0; t < UART_GetRecLength(); t++)
        {
            UART_SendData(UART_RX_BUF[t]);
            OLED_ShowChar(1, (t * 8 + 1), UART_RX_BUF[t], 8);
        }
        UART_ResetRecStatus();
    }
  ***************************************************
  */
