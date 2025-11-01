#include "LED.h"

volatile uint16_t g_LED_Blink_Interval = 100;

// 初始化 LED IO 口
void LED_Config(LED_Num_TypeDef LED_Num)
{
    if (LED_Num == LED3)
    {
        GPIO_Init(LED3_PORT, LED3_PIN, GPIO_Mode_Out_PP_High_Slow);
    }
    else
    {
        GPIO_Init(LED4_PORT, LED4_PIN, GPIO_Mode_Out_PP_High_Slow);
    }
}

// 反转LED状态
void LED_Toggle(LED_Num_TypeDef LED_Num)
{
    if (LED_Num == LED3)
    {
        GPIO_ToggleBits(LED3_PORT, LED3_PIN);
    }
    else
    {
        GPIO_ToggleBits(LED4_PORT, LED4_PIN);
    }
}

// 点亮LED
void LED_On(LED_Num_TypeDef LED_Num)
{
    if (LED_Num == LED3)
    {
        GPIO_SetBits(LED3_PORT, LED3_PIN);
    }
    else
    {
        GPIO_SetBits(LED4_PORT, LED4_PIN);
    }
}

// 熄灭LED
void LED_Off(LED_Num_TypeDef LED_Num)
{
    if (LED_Num == LED3)
    {
        GPIO_ResetBits(LED3_PORT, LED3_PIN);
    }
    else
    {
        GPIO_ResetBits(LED4_PORT, LED4_PIN);
    }
}
