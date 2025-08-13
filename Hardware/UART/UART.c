#include "UART.h"
#include "stdarg.h"
#include "stdio.h"

/**
 * 串口接收状态标志。
 * bit15，接收到0x0a置1，接收完成；
 * bit14，接收到0x0d置1；
 * bit13~bit0，接收到的有效字节数。
 */
uint16_t USART_RX_STA = 0;

/**
 * PA9-TXD | PA10-RXD
 * 接收缓冲数组，最大USART_REC_LEN个字节。
 * 在取完串口数据后，需要用 Reset_UART_RecStatus() 初始化串口接收标志
 */
uint8_t USART_RX_BUF[USART_REC_LEN];

/**
 * @brief  初始化串口，PA9-TXD | PA10-RXD。
 * @param  bound 串口波特率。
 * @retval 无
 */
void UART_Init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); // 使能USART1，GPIOA时钟

    // USART1_TX   PA9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART1_RX   PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        // 响应优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;                                     // 串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     // 8位数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          // 一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // 收发模式

    USART_Init(USART1, &USART_InitStructure);      // 初始化串口1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 开启串口接受中断
    USART_Cmd(USART1, ENABLE);                     // 使能串口1
}

/**
 * @brief  串口发送数据。
 * @param  data 要发送的数据。
 * @retval 无
 */
void UART_SendData(uint16_t data)
{
    USART_SendData(USART1, data);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ; // 等待发送完成
}

/**
 * @brief  UART格式化打印信息。
 * @param  format 格式化字符串
 * @retval 无
 */
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
        UART_SendData(TxBuffer[i]);
        // 等待发送完成
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
    }
    // 等待数据帧发送完成
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;
}

/**
 * @brief  判断串口接收是否完成（接收到0x0D 0x0A）。
 * @param  无
 * @retval 状态值
 *      - \b 1 : 接收完成
 *      - \b 0 : 接收未完成
 */
uint8_t UART_GetRecStatus(void)
{
    // 若USART_RX_STA最高位为1，接收完成
    return ((USART_RX_STA & 0x8000) ? 1 : 0);
}

/**
 * @brief  获取串口接收到的数据的长度。
 * @param  无
 * @retval 数组长度值(uint16_t)
 */
uint16_t UART_GetRecLength(void)
{
    return (uint16_t)(USART_RX_STA & 0x3FFF);
}

/**
 * @brief  重置串口接收状态标志，准备下次接收。
 * @param  无
 * @retval 无
 */
void UART_ResetRecStatus(void)
{
    USART_RX_STA = 0;
}

void USART1_IRQHandler(void)
{
    uint8_t Res;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res = USART_ReceiveData(USART1); // 读取接收到的数据

        if ((USART_RX_STA & 0x8000) == 0) // 接收未完成
        {
            if (USART_RX_STA & 0x4000) // 接收到了0x0d
            {
                if (Res != 0x0a)
                    USART_RX_STA = 0; // 接收错误,重新开始
                else
                    USART_RX_STA |= 0x8000; // 接收完成
                USART_RX_BUF[USART_RX_STA & 0X3FFF] = 0; // 补字符串结束符
            }
            else // 还没收到0X0d
            {
                if (Res == 0x0d)
                    USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
                    USART_RX_STA++;
                    if (USART_RX_STA > (USART_REC_LEN - 1))
                        USART_RX_STA = 0; // 接收数据错误,重新开始接收
                }
            }
        }
    }
}
