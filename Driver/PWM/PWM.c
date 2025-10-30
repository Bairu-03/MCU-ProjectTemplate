/*
 * PWM.c
 *
 *  Created on: 2024年7月15日
 *      Author: Bairu
 */

#include "driverlib.h"
#include "PWM.h"

static uint16_t TA0_arr;
static uint16_t TA1_arr;
static uint16_t TA2_arr;

/**
 * @brief  TA0定时器PWM初始化，PWM频率 = 时钟源频率 / 分频系数 / (arr+1)
 * @param  PWMnum PWM输出通道数量。
 *     @arg 有效取值:
 *      - \b 1 : P1.2 PWM1
 *      - \b 2 : P1.2 PWM1，P1.3 PWM2
 *      - \b 3 : P1.2 PWM1，P1.3 PWM2，P1.4 PWM3
 *      - \b 4 : P1.2 PWM1，P1.3 PWM2，P1.4 PWM3，P1.5 PWM4
 * @param  clockSource 时钟源。
 *     @arg 有效取值:
 *      - \b TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK
 *      - \b TIMER_A_CLOCKSOURCE_ACLK
 *      - \b TIMER_A_CLOCKSOURCE_SMCLK
 *      - \b TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
 * @param  psc 时钟分频系数。
 *     @arg 有效取值:
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_1
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_2
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_3
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_4
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_5
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_6
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_7
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_8
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_10
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_12
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_14
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_16
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_20
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_24
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_28
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_32
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_40
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_48
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_56
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_64
 * @param  arr 目标自动装载值 - 1。
 *     @arg 取值: 0 - 65535
 * @retval 无
 */
void TA0_PWM_Init(uint8_t PWMnum, uint16_t clockSource, uint16_t psc, uint16_t arr)
{
    TA0_arr = arr;

    Timer_A_initUpModeParam TAup = {0};
    TAup.clockSource = clockSource;
    TAup.clockSourceDivider = psc;
    TAup.timerPeriod = arr;
    TAup.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    TAup.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
    TAup.timerClear = TIMER_A_DO_CLEAR;
    TAup.startTimer = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &TAup);

    Timer_A_initCompareModeParam TAPWM = {0};
    TAPWM.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    TAPWM.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;

    uint8_t i, regAddressIncrement = 0, pinShift = 0;
    for(i = PWMnum; i > 0; i--)
    {
        // 初始化捕获比较寄存器
        TAPWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1 + regAddressIncrement;
        TAPWM.compareValue = (int)(arr * 0);
        Timer_A_initCompareMode(TIMER_A0_BASE, &TAPWM);
        regAddressIncrement += 2;

        // GPIO复用输出
        GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2 << pinShift);
        pinShift++;
    }
}

/**
 * @brief  控制TA0各通道PWM占空比
 * @param  CHx 选择PWM输出通道。
 *     @arg 取值:
 *      - \b 0 : P1.2
 *      - \b 1 : P1.3
 *      - \b 2 : P1.4
 *      - \b 3 : P1.5
 * @param  Duty PWM占空比。
 *     @arg 取值: 0 - 100.0
 * @retval 无
 */
void TA0_PWM_Duty(uint8_t CHx, float Duty)
{
    uint16_t TA_CCRx[4] = {
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_CAPTURECOMPARE_REGISTER_2,
        TIMER_A_CAPTURECOMPARE_REGISTER_3,
        TIMER_A_CAPTURECOMPARE_REGISTER_4
    };

    Timer_A_setCompareValue(TIMER_A0_BASE,
                            TA_CCRx[CHx],
                            (uint16_t)(((TA0_arr + 1) * Duty) / 100.0));
}

/**
 * @brief  TA1定时器PWM初始化，PWM频率 = 时钟源频率 / 分频系数 / (arr+1)
 * @param  PWMnum PWM输出通道数量。
 *     @arg 有效取值:
 *      - \b 1 : P2.0 PWM1
 * @param  clockSource 时钟源。
 *     @arg 有效取值:
 *      - \b TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK
 *      - \b TIMER_A_CLOCKSOURCE_ACLK
 *      - \b TIMER_A_CLOCKSOURCE_SMCLK
 *      - \b TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
 * @param  psc 时钟分频系数。
 *     @arg 有效取值:
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_1
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_2
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_3
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_4
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_5
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_6
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_7
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_8
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_10
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_12
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_14
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_16
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_20
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_24
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_28
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_32
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_40
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_48
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_56
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_64
 * @param  arr 目标自动装载值 - 1。
 *     @arg 取值: 0 - 65535
 * @retval 无
 */
void TA1_PWM_Init(uint8_t PWMnum, uint16_t clockSource, uint16_t psc, uint16_t arr)
{
    TA1_arr = arr;

    Timer_A_initUpModeParam TAup = {0};
    TAup.clockSource = clockSource;
    TAup.clockSourceDivider = psc;
    TAup.timerPeriod = arr;
    TAup.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    TAup.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
    TAup.timerClear = TIMER_A_DO_CLEAR;
    TAup.startTimer = true;
    Timer_A_initUpMode(TIMER_A1_BASE, &TAup);

    Timer_A_initCompareModeParam TAPWM = {0};
    TAPWM.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    TAPWM.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;

    uint8_t i, regAddressIncrement = 0, pinShift = 0;
    for(i = PWMnum; i > 0; i--)
    {
        // 初始化捕获比较寄存器
        TAPWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1 + regAddressIncrement;
        TAPWM.compareValue = (int)(arr * 0);
        Timer_A_initCompareMode(TIMER_A1_BASE, &TAPWM);
        regAddressIncrement += 2;

        // GPIO复用输出
        GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN0 << pinShift);
        pinShift++;
    }
}

/**
 * @brief  控制TA1各通道PWM占空比
 * @param  CHx 选择PWM输出通道。
 *     @arg 取值:
 *      - \b 0 : P2.0
 * @param  Duty PWM占空比。
 *     @arg 取值: 0 - 100.0
 * @retval 无
 */
void TA1_PWM_Duty(uint8_t CHx, float Duty)
{
    uint16_t TA_CCRx[1] = {
        TIMER_A_CAPTURECOMPARE_REGISTER_1
    };

    Timer_A_setCompareValue(TIMER_A1_BASE,
                            TA_CCRx[CHx],
                            (uint16_t)(((TA1_arr + 1) * Duty) / 100.0));
}

/**
 * @brief  TA2定时器PWM初始化，PWM频率 = 时钟源频率 / 分频系数 / (arr+1)
 * @param  PWMnum PWM输出通道数量。
 *     @arg 有效取值:
 *      - \b 1 : P2.4 PWM1
 *      - \b 2 : P2.4 PWM1，P2.5 PWM2
 * @param  clockSource 时钟源。
 *     @arg 有效取值:
 *      - \b TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK
 *      - \b TIMER_A_CLOCKSOURCE_ACLK
 *      - \b TIMER_A_CLOCKSOURCE_SMCLK
 *      - \b TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
 * @param  psc 时钟分频系数。
 *     @arg 有效取值:
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_1
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_2
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_3
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_4
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_5
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_6
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_7
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_8
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_10
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_12
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_14
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_16
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_20
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_24
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_28
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_32
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_40
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_48
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_56
 *      - \b TIMER_A_CLOCKSOURCE_DIVIDER_64
 * @param  arr 目标自动装载值 - 1。
 *     @arg 取值: 0 - 65535
 * @retval 无
 */
void TA2_PWM_Init(uint8_t PWMnum, uint16_t clockSource, uint16_t psc, uint16_t arr)
{
    TA2_arr = arr;

    Timer_A_initUpModeParam TAup = {0};
    TAup.clockSource = clockSource;
    TAup.clockSourceDivider = psc;
    TAup.timerPeriod = arr;
    TAup.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    TAup.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE;
    TAup.timerClear = TIMER_A_DO_CLEAR;
    TAup.startTimer = true;
    Timer_A_initUpMode(TIMER_A2_BASE, &TAup);

    Timer_A_initCompareModeParam TAPWM = {0};
    TAPWM.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE;
    TAPWM.compareOutputMode = TIMER_A_OUTPUTMODE_RESET_SET;

    uint8_t i, regAddressIncrement = 0, pinShift = 0;
    for(i = PWMnum; i > 0; i--)
    {
        // 初始化捕获比较寄存器
        TAPWM.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1 + regAddressIncrement;
        TAPWM.compareValue = (int)(arr * 0);
        Timer_A_initCompareMode(TIMER_A2_BASE, &TAPWM);
        regAddressIncrement += 2;

        // GPIO复用输出
        GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN4 << pinShift);
        pinShift++;
    }
}

/**
 * @brief  控制TA2各通道PWM占空比
 * @param  CHx 选择PWM输出通道。
 *     @arg 取值:
 *      - \b 0 : P2.4
 *      - \b 1 : P2.5
 * @param  Duty PWM占空比。
 *     @arg 取值: 0 - 100.0
 * @retval 无
 */
void TA2_PWM_Duty(uint8_t CHx, float Duty)
{
    uint16_t TA_CCRx[2] = {
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_CAPTURECOMPARE_REGISTER_2
    };

    Timer_A_setCompareValue(TIMER_A2_BASE,
                            TA_CCRx[CHx],
                            (uint16_t)(((TA2_arr + 1) * Duty) / 100.0));
}
