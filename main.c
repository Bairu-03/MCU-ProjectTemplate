#include "driverlib/MSP430F5xx_6xx/driverlib.h"
#include "System/Sys_Clock.h"
#include "Hardware/OLED/OLED.h"
#include "Hardware/UART/MSP430F5529_UART.h"
#include "Hardware/Encoder/Encoder.h"
#include "Hardware/PWM/PWM.h"
#include "PID/PID.h"

void main(void)
{
    WDT_A_hold(WDT_A_BASE);
    SystemClock_Init();

    __bis_SR_register(GIE);

    OLED_Init();

    UART_Init(USCI_A1_BASE, 115200);

    TA0_PWM_Init(TIMER_A_CLOCKSOURCE_SMCLK, TIMER_A_CLOCKSOURCE_DIVIDER_40, 624);
    Encoder_Init();

    PID pid_struct;    // ����PID�����ṹ�����

    // ��ʼ��PID����
    float Kp = 0.24, Ki = 0.035, Kd = 0.3;
    float minIntegral = 0, maxIntegral = 100;
    float minOutput = 0, maxOutput = 100;
    float target = 100.5;
    float realvalue = 0;
    float pid_output = 0;
    PID_Init(&pid_struct, Kp, Ki, Kd, target,
             minIntegral, maxIntegral,
             minOutput, maxOutput);

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);

    OLED_ShowString(1, 1, "Target:", 8);
    OLED_ShowString(3, 1, "Current:", 8);
    OLED_ShowString(5, 1, "pid_out:", 8);

    while(1)
    {
        // �����޸�Ŀ��ֵ
        if(!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1))
        {
             target += 30;
             while(!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1));
        }
        if(!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN1))
        {
            target -= 15;
            while(!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN1));
        }
        PID_ResetTarget(&pid_struct, target);
        OLED_ShowFloat(1, 57, target, 3, 2, 8);

        // ��ȡ�����ǰת��
        realvalue = getRotatingSpeed(P20);
        OLED_ShowFloat(3, 65, realvalue, 3, 2, 8);

        // ���ڴ�ӡת�����ݣ����vofa+�����ת�ٱ仯����
        UART_printf(USCI_A1_BASE, "%.2f\n", realvalue);

        // PID������� - PWM
        pid_output = PID_Compute(&pid_struct, realvalue);
        OLED_ShowFloat(5, 65, pid_output, 3, 2, 8);

        // ����PID�������P1.2��PWMռ�ձ�
        TA0_PWM_Duty(0, pid_output);
    }
}
