/*
 * Encoder.c
 *
 *  Created on: 2024年7月15日
 *      Author: Bairu
 */

#include "driverlib.h"
#include "Encoder.h"

uint16_t P20_Count = 0;
uint16_t P22_Count = 0;
uint16_t P20PulseNumber = 0;
uint16_t P22PulseNumber = 0;

/**
 * @brief  初始化P2.0, P2.2脉冲计数功能
 * @param  无
 * @retval 无
 */
void Encoder_Init(void)
{
    // 初始化TA1为增计数模式，每50ms触发一次中断，读取脉冲数
    Timer_A_initUpModeParam htim = {0};
    htim.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;    // 25MHz
    htim.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_40;
    htim.timerPeriod = 31250 - 1;    // (25M / 40 / 31250) = 20Hz
    htim.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    htim.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    htim.timerClear = TIMER_A_DO_CLEAR;
    htim.startTimer = true;
    Timer_A_initUpMode(TIMER_A1_BASE, &htim);

    // 初始化捕获编码器脉冲的IO口: P2.0, P2.2
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN2);
    GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN2, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN2);
    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN2);
}

/**
 * @brief  获取P2.0引脚编码器脉冲数
 * @param  无
 * @retval 50ms内的脉冲数
 */
uint16_t getP20PulseNum(void)
{
    return P20PulseNumber;
}

/**
 * @brief  获取P2.2引脚编码器脉冲数
 * @param  无
 * @retval 50ms内的脉冲数
 */
uint16_t getP22PulseNum(void)
{
    return P22PulseNumber;
}

// TA1定时中断，每50ms执行一次，取脉冲数并清零
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
    P20PulseNumber = P20_Count;
    P22PulseNumber = P22_Count;
    P20_Count = 0;
    P22_Count = 0;
}

// P2.0，P2.2中断，每次中断计数+1
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
    if(GPIO_getInterruptStatus(GPIO_PORT_P2, GPIO_PIN0))
    {
        P20_Count++;
        GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN0);
    }
    if(GPIO_getInterruptStatus(GPIO_PORT_P2, GPIO_PIN2))
    {
        P22_Count++;
        GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN2);
    }
}

