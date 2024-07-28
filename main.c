#include "driverlib.h"
#include <stdio.h>
#include "System/Sys_Clock.h"
#include "Hardware/OLED/OLED.h"
#include "Hardware/Car/Car.h"
#include "Hardware/PWM/PWM.h"
#include "Hardware/Encoder/Encoder.h"
#include "Hardware/UART/MSP430F5529_UART.h"
#include "PID/PID.h"

void main(void)
{
    WDT_A_hold(WDT_A_BASE);
    SystemClock_Init();
    __bis_SR_register(GIE);

    OLED_Init();
    AScar_Init();
    Encoder_Init();
    UART_Init(USCI_A0_BASE, 115200);
    UART_Init(USCI_A1_BASE, 115200);

    // 驱动电机转速PID
    PID MotorPID;
    float Motor_Kp = 0.015, Motor_Ki = 0.014, Motor_Kd = 0.001;
    float Motor_target = 370;
    PID_Init(&MotorPID, Motor_Kp, Motor_Ki, Motor_Kd, Motor_target, -1850, 1850, 0, 100);
    float Motor_now = 0;
    float Motor_pidout = 0;

    // 转向舵机PID
    IncPID ServoPID;
    float Servo_Kp = 0.05, Servo_Ki = 0, Servo_Kd = 0.01;
    float Servo_target = 70;
    IncPID_Init(&ServoPID, Servo_Kp, Servo_Ki, Servo_Kd, Servo_target, 4.5, 9.5);
    float Servo_now = 0;
    float Servo_pidout = 0;

    OLED_ShowString(1, 1, "Snow:", 8);
    OLED_ShowString(3, 1, "Spidout:", 8);
    OLED_ShowString(5, 1, "Mnow:", 8);
    OLED_ShowString(7, 1, "Mpidout:", 8);

    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);

    uint8_t runFlag = 0;
    while(1)
    {
        // 串口1接收pid参数
        if(get_Uart_RecStatus(USCI_A1_BASE))
        {
            uint8_t i;
            char temp_pid[7];
            float uart_p, uart_i, uart_d;
            // 串口调节pid，格式：p1.234* 或 i1.234* 或 d1.234*
            if(UART1_RX_BUF[0] == 'p' || UART1_RX_BUF[0] == 'i' || UART1_RX_BUF[0] == 'd')
            {
                // 读取串口传入的pid值
                for(i = 0; i < get_Uart_RecLength(USCI_A1_BASE) - 1; i++)
                {
                    temp_pid[i] = UART1_RX_BUF[i + 1];
                }
                if(UART1_RX_BUF[0] == 'p')
                {
                    sscanf(temp_pid, "%f", &uart_p);
                    PID_Reset_pid(&MotorPID, 1, uart_p);
                }
                if(UART1_RX_BUF[0] == 'i')
                {
                    sscanf(temp_pid, "%f", &uart_i);
                    PID_Reset_pid(&MotorPID, 2, uart_i);
                }
                if(UART1_RX_BUF[0] == 'd')
                {
                    sscanf(temp_pid, "%f", &uart_d);
                    PID_Reset_pid(&MotorPID, 3, uart_d);
                }
                UART_printf(USCI_A1_BASE, "OK\n");
            }
            else
            {
                UART_printf(USCI_A1_BASE, "ERROR\n");
            }
            Reset_Uart_RecStatus(USCI_A1_BASE);
        }

        // 串口0接收openMV数据（寻迹偏移量）
        if(get_Uart_RecStatus(USCI_A0_BASE))
        {
            uint8_t i;
            for(i = 0; i < get_Uart_RecLength(USCI_A0_BASE); i++)
            {
                Servo_now = UART0_RX_BUF[i];
            }
            Reset_Uart_RecStatus(USCI_A0_BASE);
        }
        OLED_ShowNum(1, 41, Servo_now, 3, 8);

        // 驱动轮电机转速
        Motor_now = getP20PulseNum();
        OLED_ShowNum(5, 41, Motor_now, 4, 8);

        // 转向环PID
        Servo_pidout = IncPID_Compute(&ServoPID, Servo_now);
        // UART_printf(USCI_A1_BASE, "%.2f,%.2f\n", (Servo_now / 10.0), Servo_pidout);
        OLED_ShowFloat(3, 65, Servo_pidout, 3, 2, 8);

        // 速度环PID
        Motor_pidout = PID_Compute(&MotorPID, Motor_now);
        UART_printf(USCI_A1_BASE, "%.2f\n", Motor_now);
        OLED_ShowFloat(7, 65, Motor_pidout, 3, 2, 8);

        // 按键控制小车启停
        if(!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN1))
        {
            delay_ms(10);
            while(!GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN1));
            runFlag = 1;
        }
        if(!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1))
        {
            delay_ms(10);
            while(!GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1));
            runFlag = 0;
        }

        if(runFlag)
        {
            PID_ResetTarget(&MotorPID, Motor_target);
            AScar_Status(Car_F, 0, Motor_pidout, Servo_pidout);
        } else {
            PID_ResetTarget(&MotorPID, 0);
            AScar_Status(Car_Stop, 0, 0, 7.8);
        }
    }
}
