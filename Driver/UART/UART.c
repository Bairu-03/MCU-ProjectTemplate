#include "UART.h"
#include "stdarg.h"
#include "stdio.h"

volatile UART_ErrorFlag_TypeDef g_UART_Flag = UART_No_Msg;

/**
 * @brief  UART配置 TxD - PC3 | RxD - PC2
 * @param  BaudRate : 波特率
 * @param  USART_WordLength : 数据位长度
 * @param  USART_StopBits : 停止位长度
 * @param  USART_Parity : 校验位
 * @retval 无
 */
void UART_Config(uint32_t BaudRate, USART_WordLength_TypeDef USART_WordLength,
                 USART_StopBits_TypeDef USART_StopBits, USART_Parity_TypeDef USART_Parity)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);

    GPIO_Init(TX_GPIO_PORT, TX_GPIO_PIN, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(RX_GPIO_PORT, RX_GPIO_PIN, GPIO_Mode_In_PU_No_IT);

    USART_Init(USART1, BaudRate, USART_WordLength_8b,
               USART_StopBits_1, USART_Parity_No,
               (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1, ENABLE);
}

// 格式化打印字符串到串口
void UART_printf(const char *format, ...)
{
    uint32_t length;
    va_list args;
    uint32_t i;
    char TxBuffer[UART_SEND_LEN] = {0};

    va_start(args, format);
    length = vsprintf((char *)TxBuffer, (char *)format, args);
    va_end(args);
    for (i = 0; i < length; i++)
    {
        USART_SendData8(USART1, TxBuffer[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
    }
    // 等待数据帧发送完成
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;
}
