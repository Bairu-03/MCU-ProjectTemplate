#ifndef __UART_H
#define __UART_H

#include "stm8l15x.h"

#define TX_GPIO_PORT GPIOC
#define RX_GPIO_PORT GPIOC

#define TX_GPIO_PIN GPIO_Pin_3
#define RX_GPIO_PIN GPIO_Pin_2

// UART接收缓冲区大小
#define UART_REC_LEN 200

// UART发送缓冲区大小
#define UART_SEND_LEN 200

typedef enum
{
    UART_No_Msg,
    UART_Correct_Command,
    UART_Out_Of_Range,
    UART_Invalid_Value
} UART_ErrorFlag_TypeDef;

extern volatile UART_ErrorFlag_TypeDef g_UART_Flag;

void UART_Config(uint32_t BaudRate, USART_WordLength_TypeDef USART_WordLength,
                 USART_StopBits_TypeDef USART_StopBits, USART_Parity_TypeDef USART_Parity);
void UART_printf(const char *format, ...);

#endif /* __UART_H */
