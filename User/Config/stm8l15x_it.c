/**
 ******************************************************************************
 * @file    GPIO/GPIO_Toggle/stm8l15x_it.c
 * @author  MCD Application Team
 * @version V1.3.0
 * @date    07/14/2010
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all peripherals interrupt service routine.
 ******************************************************************************
 * @copy
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
 */

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_it.h"
#include "stdbool.h"
#include "KEY.h"
#include "LED.h"
#include "UART.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
 * @{
 */

/** @addtogroup GPIO_Toggle
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEBOUNCE_TIME_MS 20
#define LONG_PRESS_TIME_MS 1000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint16_t s_LED_Timer = 0;

static uint8_t s_Key_Debounce_Count = 0;
static uint16_t s_Key_Press_Timer = 0;
static bool s_Key_Pressed = false;
static bool s_Key_LongPress_Flag = false;

// 接收缓冲数组
static uint8_t s_UART_RxBuf[UART_REC_LEN] = {0};

// 接收缓冲数组索引
static uint8_t s_UART_BufIndex = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
 * @brief  Dummy interrupt routine
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(NonHandledInterrupt, 0)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#endif

/**
 * @brief  TRAP interrupt routine
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
 * @brief  FLASH Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(FLASH_IRQHandler, 1)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
 * @brief  DMA1 channel0 and channel1 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
 * @brief  DMA1 channel2 and channel3 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler, 3)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
 * @brief  RTC Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(RTC_IRQHandler, 4)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
 * @brief  External IT PORTE/F and PVD Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler, 5)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  External IT PORTB Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTIB_IRQHandler, 6)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  External IT PORTD Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTID_IRQHandler, 7)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  External IT PIN0 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTI0_IRQHandler, 8)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  External IT PIN1 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTI1_IRQHandler, 9)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    // 按键中断
    if (EXTI_GetITStatus(EXTI_IT_Pin1) != RESET)
    {
        // 开启TIM4中断, 响应按键
        TIM4_ITConfig(TIM4_IT_Update, ENABLE);
        EXTI_ClearITPendingBit(EXTI_IT_Pin1);
    }
}

/**
 * @brief  External IT PIN2 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTI2_IRQHandler, 10)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  External IT PIN3 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTI3_IRQHandler, 11)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  External IT PIN4 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTI4_IRQHandler, 12)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  External IT PIN5 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTI5_IRQHandler, 13)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  External IT PIN6 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTI6_IRQHandler, 14)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  External IT PIN7 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(EXTI7_IRQHandler, 15)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
 * @brief  LCD start of new frame Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(LCD_IRQHandler, 16)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
 * @brief  CLK switch/CSS/TIM1 break Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler, 17)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  ADC1/Comparator Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler, 18)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  TIM2 Update/Overflow/Trigger/Break Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_IRQHandler, 19)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    // 控制LED闪烁, 10ms执行一次
    if (TIM2_GetITStatus(TIM2_IT_Update) != RESET)
    {
        if (s_LED_Timer < g_LED_Blink_Interval)
        {
            s_LED_Timer++;
        }
        else
        {
            LED_Toggle(LED3);
            s_LED_Timer = 0;
        }
        TIM2_ClearITPendingBit(TIM2_IT_Update);
    }
}

/**
 * @brief  Timer2 Capture/Compare Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(TIM2_CAP_IRQHandler, 20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  Timer3 Update/Overflow/Trigger/Break Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_IRQHandler, 21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
 * @brief  Timer3 Capture/Compare Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(TIM3_CAP_IRQHandler, 22)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
 * @brief  TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler, 23)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
 * @brief  TIM1 Capture/Compare Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(TIM1_CAP_IRQHandler, 24)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  TIM4 Update/Overflow/Trigger Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler, 25)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    // 按键轮询, 1ms执行一次
    if (TIM4_GetITStatus(TIM4_IT_Update) != RESET)
    {
        // 按键被按下
        if (IS_KEY2_PRESSED())
        {
            // 过20ms再判定按键状态, 消抖
            if (s_Key_Debounce_Count <= DEBOUNCE_TIME_MS)
            {
                s_Key_Debounce_Count++;
            }
            else
            {
                // 按键动作有效
                s_Key_Pressed = true;

                // 还未触发长按, 累加按键时间
                if (!s_Key_LongPress_Flag)
                {
                    s_Key_Press_Timer++;
                }
            }
        }
        // 按键未按下
        else
        {
            s_Key_Debounce_Count = 0;
            // 若按键动作有效
            if (s_Key_Pressed)
            {
                // 未达到长按标准
                if (!s_Key_LongPress_Flag)
                {
                    g_KeyPressStatus = Key_ShortPress;  // 状态标志设为短按
                    TIM4_ITConfig(TIM4_IT_Update, DISABLE);
                }
                s_Key_Pressed = false;
                s_Key_LongPress_Flag = false;
                s_Key_Debounce_Count = 0;
                s_Key_Press_Timer = 0;
                
            }
        }

        // 按键持续时间达到长按标准
        if (s_Key_Press_Timer > LONG_PRESS_TIME_MS)
        {
            g_KeyPressStatus = Key_LongPress;  // 状态标志设为长按

            TIM4_ITConfig(TIM4_IT_Update, DISABLE);
            s_Key_LongPress_Flag = true;
            s_Key_Debounce_Count = 0;
            s_Key_Press_Timer = 0;
        }

        TIM4_ClearITPendingBit(TIM4_IT_Update);
    }
}
/**
 * @brief  SPI1 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(SPI1_IRQHandler, 26)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  USART1 TX Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(USART1_TX_IRQHandler, 27)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @brief  USART1 RX Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(USART1_RX_IRQHandler, 28)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    // UART接收数据
    uint8_t ReceiveData;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        // 读取DR中的数据
        ReceiveData = USART_ReceiveData8(USART1);

        // 接收到结束符: 回车符 \r
        if (ReceiveData == '\r')
        {
            // 在\r后面补字符串结束符
            s_UART_RxBuf[s_UART_BufIndex] = '\0';

            bool Is_Error_Data = false;
            uint8_t Input_Data = 0;
            // 遍历接收数组, 解析
            for (uint8_t i = 0; i < s_UART_BufIndex; i++)
            {
                // 检查字符是否有效
                if (s_UART_RxBuf[i] < '0' || s_UART_RxBuf[i] > '9')
                {
                    Is_Error_Data = true;
                    break;
                }
                // 把数字字符转成对应的整形数字
                Input_Data = Input_Data * 10 + (s_UART_RxBuf[i] - '0');
            }
            if (!Is_Error_Data)
            {
                if (Input_Data <= 10)
                {
                    // 注：控制LED的定时器中断间隔: 10ms
                    g_LED_Blink_Interval = Input_Data * 100;
                    g_UART_Flag = UART_Correct_Command;
                }
                else
                {
                    g_UART_Flag = UART_Out_Of_Range;
                }
            }
            else
            {
                g_UART_Flag = UART_Invalid_Value;
            }
            s_UART_BufIndex = 0;
        }
        else
        {
            // 没有接收到结束符, 继续接收, 索引号自增
            s_UART_RxBuf[s_UART_BufIndex++] = ReceiveData;
            // 缓冲区溢出, 重置索引
            if (s_UART_BufIndex >= UART_REC_LEN)
            {
                s_UART_BufIndex = 0;
            }
        }
    }
}

/**
 * @brief  I2C1 Interrupt routine.
 * @param  None
 * @retval None
 */
INTERRUPT_HANDLER(I2C1_IRQHandler, 29)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
