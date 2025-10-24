#include "stm32f10x.h"
#include "delay.h"
#include "I2C_Software.h"
#include "OLED.h"
#include "UART.h"

int main(void)
{
    I2C_Sim_Init();
    OLED_I2C_t OLED_I2C = {
        .Start = I2C_Sim_Start,
        .SendByte = I2C_Sim_SendByte,
        .Stop = I2C_Sim_Stop,
    };

    OLED_Init(&OLED_I2C);

    UART_Init(115200);
    UART_printf("\nUART initialization complete! Baud rate: 115200.\n");
    UART_printf("Please send a string to display on the OLED screen.\n");

    OLED_ShowString(1, 1, " UART OLED TEST ", 8);
    OLED_ShowString(3, 1, "================", 8);

    Delay_ms(1000);

    /* 设置OLED反显 */
    OLED_SetDisplayMode(NEGATIVE_MODE);

    /* 绘制进度条框 */
    OLED_SetCursor(4, 8);
    OLED_WriteData(0xFF);
    OLED_SetCursor(5, 8);
    OLED_WriteData(0xFF);
    OLED_SetCursor(4, 120);
    OLED_WriteData(0xFF);
    OLED_SetCursor(5, 120);
    OLED_WriteData(0xFF);
    for (int j = 0; j < 111; j++)
    {
        OLED_SetCursor(4, 9 + j);
        OLED_WriteData(0x01);
        OLED_SetCursor(5, 9 + j);
        OLED_WriteData(0x80);
    }

    /* 第一行文字向右上方滚动 */
    OLED_Scroll_VH(1, 16, ScrH_ON, ScrVR, 1, 2, 1, 128, 1, OLED_ScrSpeed5);

    Delay_ms(500);

    /* 模拟进度条加载 */
    int i;
    for (i = 2; i < 111; i++)
    {
        OLED_SetCursor(4, 8 + i);
        OLED_WriteData(0xFD);
        OLED_SetCursor(5, 8 + i);
        OLED_WriteData(0xBF);
        Delay_ms(i - i / 2 + 1);
    }
    OLED_ShowString(5, 1, "     FINISH     ", 8);

    Delay_ms(1000);

    OLED_Stop_Scroll();
    OLED_ClearLine(1, 2);
    OLED_ShowString(1, 1, " UART OLED TEST ", 8);

    OLED_ClearLine(5, 6);

    while (1)
    {
        // 串口收发测试
        if (UART_GetRecStatus())
        {
            uint8_t t;
            OLED_ClearLine(5, 6);
            for (t = 0; t < UART_GetRecLength(); t++)
            {
                UART_SendData(UART_RX_BUF[t]);
                OLED_ShowChar(5, (t * 8 + 1), UART_RX_BUF[t], 8);
            }
            UART_ResetRecStatus();
        }
    }
}
