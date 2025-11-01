#include "test.h"
#include "stdbool.h"
#include "SysClk.h"
#include "UART.h"
#include "KEY.h"
#include "Timer.h"
#include "LED.h"
#include "EEPROM.h"
#include "ADC.h"

#define TIM2_PERIOD 2499
#define TIM4_PERIOD 249
#define TIM1_PRESCALER 15
#define TIM1_PERIOD 249

// WWDG计数器值 = 超时时间 * (SYSCLK / 12288) + 63
//     超时时间 = (127 - 63) / (16MHz / 12288) = 49.16ms
#define COUNTER_INIT 0x7F
#define WINDOW_VALUE 0x7F

// 读写EEPROM
void Read_Write_EEPROM(void)
{
    uint8_t Data = 0x00;
    uint32_t OffsetAddress = 0x05;

    UART_printf("Task1: Read and write EEPROM.\n");
    UART_printf("Read data from 0x%04lX, ", OffsetAddress);
    Read_EEPROM(OffsetAddress, &Data);
    UART_printf("Original data: 0x%02X\n", Data);
    if (Data != 0xA5)
    {
        UART_printf("Write 0xA5 to 0x%04lX\n", OffsetAddress);
        Write_EEPROM(OffsetAddress, 0xA5);
    }
    else
    {
        UART_printf("Write 0x5A to 0x%04lX\n", OffsetAddress);
        Write_EEPROM(OffsetAddress, 0x5A);
    }
    UART_printf("Read data from 0x%04lX, ", OffsetAddress);
    Read_EEPROM(OffsetAddress, &Data);
    UART_printf("Modified data: 0x%02X\n\n", Data);
}

// ADC转换
void Get_ADC_Value(void)
{
    UART_printf("Task2: Get ADC Value.\n");
    UART_printf("ADC-CH1(PA5) Voltage = %.2fV\n", ADC_GetCH1Voltage());
    UART_printf("ADC-CH2(PA4) Voltage = %.2fV\n\n", ADC_GetCH2Voltage());
}

// 输出PWM
void Generate_PWM(void)
{
    TIM1_Cmd(ENABLE);
    TIM1_CtrlPWMOutputs(ENABLE);
    UART_printf("Task3: Generate PWM.\n");
    UART_printf("f = 4kHz\nTIM1-OC1(PD2): 20%%\n"
                "TIM1-OC2(PD4): 50%%\nTIM1-OC3(PD5): 80%%\n\n");
}

// 关闭PWM输出
void Stop_PWM_Output(void)
{
    TIM1_CtrlPWMOutputs(DISABLE);
    TIM1_Cmd(DISABLE);
    TIM1_SetCounter(0);
}

// 进入HALT模式
void Enter_Halt_Mode(void)
{
    Stop_PWM_Output();
    LED_Off(LED3);
    UART_printf("Task4: Enter Halt Mode.\n\n");

    halt();
}

// 通过UART打印输入指令的执行结果
void UART_Send_Message(void)
{
    UART_ErrorFlag_TypeDef tmp_UART_Flag = UART_No_Msg;
    if (g_UART_Flag != UART_No_Msg)
    {
        disableInterrupts();
        tmp_UART_Flag = g_UART_Flag;
        g_UART_Flag = UART_No_Msg;
        enableInterrupts();
        if (tmp_UART_Flag == UART_Out_Of_Range)
        {
            UART_printf("\nThe input data is out of range!\n"
                        "It can only be an integer between 0 and 10.\n\n");
        }
        else if (tmp_UART_Flag == UART_Invalid_Value)
        {
            UART_printf("\nThe input data is incorrect!\n"
                        "It can only be an integer between 0 and 10.\n\n");
        }
        else
        {
            UART_printf("\nThe LED flash interval is set to: %ds\n\n", (g_LED_Blink_Interval / 100));
        }
    }
}

void run_test(void)
{
    uint8_t Key_Count = 0;
    Key_PressStatus_TypeDef tmp_Key_Status = Key_NotPress;
    bool Clear_WDG_Flag = true;

    disableInterrupts();

    // 系统时钟频率 = HSI / 1 = 16MHz
    SYSCLK_Config(CLK_SYSCLKSource_HSI, CLK_SYSCLKDiv_1);

    UART_Config(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);

    LED_Config(LED3);

    // 10ms定时, TIM2_Period = (0.01s * (16MHz / 64) - 1) = 2499
    TIM2_Config(TIM2_Prescaler_64, TIM2_PERIOD);

    // 1ms定时, TIM4_Period = (0.001s * (16MHz / 64) - 1) = 249
    TIM4_Config(TIM4_Prescaler_64, TIM4_PERIOD);

    Key_Config();

    ADC_Config();

    // fPWM = 16MHz / (15 + 1) / (249 + 1) = 4kHz
    TIM1_PWMConfig(TIM1_PRESCALER, TIM1_PERIOD);
    TIM1_SetPWMDuty(TIM1_PERIOD, TIM1_PWM_OC1, 20); // PD2-20%
    TIM1_SetPWMDuty(TIM1_PERIOD, TIM1_PWM_OC2, 50); // PD4-50%
    TIM1_SetPWMDuty(TIM1_PERIOD, TIM1_PWM_OC3, 80); // PD5-80%

    WWDG_Init(COUNTER_INIT, WINDOW_VALUE);

    enableInterrupts();

    // 检查是否发生窗口看门狗复位
    if (RST_GetFlagStatus(RST_FLAG_WWDGF))
    {
        UART_printf("WWDG Reset\n");
        RST_ClearFlag(RST_FLAG_WWDGF);
    }
    else
    {
        UART_printf("Start\n");
    }

    UART_printf("Tip:\n"
                "    1.Short press the blue button to switch between 4 tasks, "
                "and long press the blue button to stop clearing WWDG\n");
    UART_printf("    2.Enter an integer between 0-10 in non-Halt mode "
                "to control the LED flash interval.\n\n");

    while (1)
    {
        if (g_KeyPressStatus != Key_NotPress)
        {
            disableInterrupts();
            tmp_Key_Status = g_KeyPressStatus;
            g_KeyPressStatus = Key_NotPress;
            enableInterrupts();
        }

        if (tmp_Key_Status == Key_ShortPress)
        {
            Key_Count++;
            switch (Key_Count)
            {
            case 1:
                /* 读写EEPROM */
                Read_Write_EEPROM();
                break;
            case 2:
                /* ADC转换 */
                Get_ADC_Value();
                break;
            case 3:
                /* 生成PWM */
                Generate_PWM();
                break;
            case 4:
                /* 进入HALT模式 */
                Key_Count = 0;
                Enter_Halt_Mode();
                break;
            default:
                break;
            }
            tmp_Key_Status = Key_NotPress;
        }
        else if (tmp_Key_Status == Key_LongPress)
        {
            UART_printf("Stop clear WWDG\n");
            /* 停止清狗 */
            Clear_WDG_Flag = false;
            tmp_Key_Status = Key_NotPress;
        }

        UART_Send_Message();

        if (Clear_WDG_Flag)
        {
            WWDG_SetCounter(COUNTER_INIT);
        }
    }
}
