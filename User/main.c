#include "driverlib.h"
#include "Sys_Clock.h"

void main(void)
{
    WDT_A_hold(WDT_A_BASE);    // 关闭看门狗
    SystemClock_Init();        // 初始化系统时钟

    __bis_SR_register(GIE);    // 使能总中断

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);   //定义输出端口 1.0-红灯
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN7);   //定义输出端口 4.7-绿灯

    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);    //红灯灭
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);    //绿灯灭
    while(1)
    {
        //红、绿灯闪
        GPIO_toggleOutputOnPin(GPIO_PORT_P4, GPIO_PIN7);
        delay_ms(100);
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        delay_ms(100);
    }
}
