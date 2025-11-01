#include "Timer.h"

/**
 * @brief  配置TIM1输出3路PWM.
 *         PWM频率 = SYSCLK / (TIM1_Prescaler + 1) / (TIM1_Period + 1)
 * @param  TIM1_Prescaler : 指定预分频器值
 * @param  TIM1_Period : 指定装载值
 *             16位定时器, 计数范围0 - 65535.
 * @retval 无
 */
void TIM1_PWMConfig(uint16_t TIM1_Prescaler, uint16_t TIM1_Period)
{
    GPIO_Init(GPIOD, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5, GPIO_Mode_Out_PP_Low_Fast);

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);

    TIM1_TimeBaseInit(TIM1_Prescaler, TIM1_CounterMode_Up, TIM1_Period, 0);

    // 初始化输出通道
    TIM1_OC1Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
                 0, TIM1_OCPolarity_High, TIM1_OCNPolarity_High,
                 TIM1_OCIdleState_Set, TIM1_OCNIdleState_Set);

    TIM1_OC2Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
                 0, TIM1_OCPolarity_High, TIM1_OCNPolarity_High,
                 TIM1_OCIdleState_Set, TIM1_OCNIdleState_Set);

    TIM1_OC3Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
                 0, TIM1_OCPolarity_High, TIM1_OCNPolarity_High,
                 TIM1_OCIdleState_Set, TIM1_OCNIdleState_Set);

    // 启用预装载寄存器
    TIM1_ARRPreloadConfig(ENABLE);
    TIM1_OC1PreloadConfig(ENABLE);
    TIM1_OC2PreloadConfig(ENABLE);
    TIM1_OC3PreloadConfig(ENABLE);

    TIM1_CtrlPWMOutputs(DISABLE);

    TIM1_Cmd(DISABLE);
}

/**
 * @brief  指定TIM1各路PWM的占空比.
 * @param  TIM1_Period : 配置TIM1时设置的装载值
 * @param  PWM_Channel : 指定输出PWM的通道, 取值参考TIM1_PWMChannel_TypeDef枚举类型
 * @param  Duty : 指定占空比, 取值范围 0 - 100
 * @retval 无
 */
void TIM1_SetPWMDuty(uint16_t TIM1_Period, TIM1_PWMChannel_TypeDef PWM_Channel, uint8_t Duty)
{
    if (PWM_Channel == TIM1_PWM_OC1)
    {
        TIM1_SetCompare1((TIM1_Period + 1) * Duty / 100);
    }
    else if (PWM_Channel == TIM1_PWM_OC2)
    {
        TIM1_SetCompare2((TIM1_Period + 1) * Duty / 100);
    }
    else
    {
        TIM1_SetCompare3((TIM1_Period + 1) * Duty / 100);
    }
}

/**
 * @brief  TIM2配置.
 * @param  TIM2_Prescaler : 指定预分频器值
 * @param  TIM2_Period : 指定装载值
 *             16位定时器, 计数范围0 - 65535.
 * @retval 无
 */
void TIM2_Config(TIM2_Prescaler_TypeDef TIM2_Prescaler, uint16_t TIM2_Period)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);

    TIM2_TimeBaseInit(TIM2_Prescaler, TIM2_CounterMode_Up, TIM2_Period);

    TIM2_ClearFlag(TIM2_FLAG_Update);
    TIM2_ITConfig(TIM2_IT_Update, ENABLE);

    TIM2_Cmd(ENABLE);
}

/**
 * @brief  TIM4配置.
 * @param  TIM4_Prescaler : 指定预分频器值
 * @param  TIM4_Period : 指定装载值
 *             8位定时器, 计数范围0 - 255.
 * @retval 无
 */
void TIM4_Config(TIM4_Prescaler_TypeDef TIM4_Prescaler, uint8_t TIM4_Period)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

    TIM4_TimeBaseInit(TIM4_Prescaler, TIM4_Period);

    TIM4_ClearFlag(TIM4_FLAG_Update);
    TIM4_ITConfig(TIM4_IT_Update, DISABLE);

    TIM4_Cmd(ENABLE);
}
