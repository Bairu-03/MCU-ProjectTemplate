/*
 * Car.c
 *
 *  Created on: 2024年7月22日
 *      Author: Bairu
 */

#include "driverlib.h"
#include "Car.h"
#include "Sys_Clock.h"
#include "PWM.h"

/**
 * @brief  初始化阿克曼转向小车相关外设。
 * @param  无
 * @retval 无
 */
void AScar_Init(void)
{
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN3 | GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN2);

    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3 | GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P8, GPIO_PIN2);

    // 驱动轮电机PWM，频率5kHz（P1.2，P1.3）
    TA0_PWM_Init(2, TIMER_A_CLOCKSOURCE_SMCLK, TIMER_A_CLOCKSOURCE_DIVIDER_40, 124);

    // 转向舵机PWM，频率50Hz（P2.4）
    TA2_PWM_Init(1, TIMER_A_CLOCKSOURCE_SMCLK, TIMER_A_CLOCKSOURCE_DIVIDER_40, 12499);
}

/**
 * @brief  控制阿克曼转向小车驱动轮电机状态及转速。
 * @param  Status 小车状态。
 *      @arg 有效取值:
 *       - \b Car_F    : 向前
 *       - \b Car_B    : 向后
 *       - \b Car_Stop : 停止（此时传入函数的PWM占空比值将被忽略，建议设为0）
 * @param  leftWheelDuty P1.3左电机PWM占空比。
 *      @arg 取值: 0 - 100.0
 * @param  rightMotorDuty P1.2右电机PWM占空比。
 *      @arg 取值: 0 - 100.0
 * @param  ServoDuty P2.4转向舵机PWM占空比。
 *      @arg 取值: 4.5 - (7.6) - 9.5
 * @retval 无
 */
void AScar_Status(uint8_t Status, float leftMotorDuty, float rightMotorDuty, float ServoDuty)
{
    // 转向舵机角度限幅
    if(ServoDuty > 9.5)
        ServoDuty = 9.5;
    if(ServoDuty < 4.5)
        ServoDuty = 4.5;

    TA2_PWM_Duty(0, ServoDuty);

    if(Status == Car_F)
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3 | GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);
        GPIO_setOutputLowOnPin(GPIO_PORT_P8, GPIO_PIN2);

        delay_us(5);

        // 右轮反转
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN3);

        // 左轮反转
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7);
    }

    if(Status == Car_B)
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3 | GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);
        GPIO_setOutputLowOnPin(GPIO_PORT_P8, GPIO_PIN2);

        delay_us(5);

        // 右轮正转
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN0);

        // 左轮正转
        GPIO_setOutputHighOnPin(GPIO_PORT_P8, GPIO_PIN2);
    }

    if(Status == Car_Stop)
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN3 | GPIO_PIN0);
        GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN7);
        GPIO_setOutputLowOnPin(GPIO_PORT_P8, GPIO_PIN2);

        leftMotorDuty = 0;
        rightMotorDuty = 0;
    }

    // 左电机PWM占空比
    TA0_PWM_Duty(1, leftMotorDuty);

    // 右电机PWM占空比
    TA0_PWM_Duty(0, rightMotorDuty);
}
