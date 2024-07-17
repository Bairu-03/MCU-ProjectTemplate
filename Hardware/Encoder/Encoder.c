/*
 * Encoder.c
 *
 *  Created on: 2024��7��15��
 *      Author: Bairu
 */

#include "driverlib/MSP430F5xx_6xx/driverlib.h"
#include "Encoder.h"

uint16_t P20_Count = 0;
uint16_t P22_Count = 0;
uint8_t CaptureSwitch = 0;
float P20RotatingSpeed = 0;
float P22RotatingSpeed = 0;

/**
 * @brief  ��ʼ��TA1Ϊ������ģʽ��ÿ125ms����һ���жϣ���������Ƶ��
 * @param  ��
 * @retval ��
 */
void Encoder_TA1_Init(void)
{
    Timer_A_initUpModeParam htim = {0};
    htim.clockSource = TIMER_A_CLOCKSOURCE_ACLK;    // 32768Hz
    htim.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64;
    htim.timerPeriod = 64 - 1;    // (32768 / 64 / 64) = 8Hz
    htim.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    htim.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    htim.timerClear = TIMER_A_DO_CLEAR;
    htim.startTimer = true;
    Timer_A_initUpMode(TIMER_A1_BASE, &htim);
}

/**
 * @brief  ��ʼ��������������������IO��: P2.0, P2.2
 * @param  ��
 * @retval ��
 */
void Encoder_IO_Init(void)
{
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN2);
    GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN2, GPIO_LOW_TO_HIGH_TRANSITION); //�����ز����ж�
    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN2);
}

/**
 * @brief  ��ʼ��������ת�ٶ�ȡ���ܣ�IO��: P2.0, P2.2
 * @param  ��
 * @retval ��
 */
void Encoder_Init(void)
{
    Encoder_TA1_Init();
    Encoder_IO_Init();
}

/**
 * @brief  ��ȡ����������
 * @param  PortX  ��������������˿�
 *     @arg ��Чȡֵ:
 *      - \b P20 : P2.0��
 *      - \b P22 : P2.2��
 * @retval 125ms�ڵ�������
 */
float getRotatingSpeed(uint8_t PortX)
{
    if(PortX == P20) {
        return P20RotatingSpeed;
    } else if(PortX == P22) {
        return P22RotatingSpeed;
    } else {
        return 0;
    }
}

// TA1�жϷ�����������ת��
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR (void)
{
    static uint16_t P20CountBegin = 0, P20CountEnd = 0;
    static uint16_t P22CountBegin = 0, P22CountEnd = 0;
    if(!CaptureSwitch)
    {
        GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN2);
        P20CountBegin = P20_Count;
        P22CountBegin = P22_Count;
    }
    else
    {
        GPIO_disableInterrupt(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN2);
        P20CountEnd = P20_Count;
        P22CountEnd = P22_Count;

        // // ת��(rps)��ƥ��JGB37-520�����������ٱ�1:30��11��
        // P20RotatingSpeed = 8.0 * (float)(P20CountEnd - P20CountBegin) / 330.0;
        // P22RotatingSpeed = 8.0 * (float)(P22CountEnd - P22CountBegin) / 330.0;

        // ����������/125ms
        P20RotatingSpeed = P20CountEnd - P20CountBegin;
        P22RotatingSpeed = P22CountEnd - P22CountBegin;

        P20_Count = 0;
        P22_Count = 0;
    }
    CaptureSwitch = !CaptureSwitch;
}

// P2.0��P2.2�ⲿ�жϷ�������ÿ���жϼ���+1
// ע��: ��Ŀ�����뱾ģ�����Ҫ������λ��ʹ��PORT2�жϣ�
//      ����Ҫ�����жϺ���ע�͵��������˴��ĺ����帴�Ƶ���PORT2�жϺ����ڣ�����������ظ���������
#pragma vector=PORT2_VECTOR
__interrupt void Port2_interrupt(void)
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

