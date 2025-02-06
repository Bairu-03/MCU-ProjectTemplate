#include "ti_msp_dl_config.h"
#include "Delay.h"

int main(void)
{
    SYSCFG_DL_init();

    while(1)
    {
        // PA14 LED闪烁
        DL_GPIO_clearPins(GPIO_LED_PORT, GPIO_LED_LED_PIN_PIN);
        delay_ms(100);
        DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_LED_PIN_PIN);
        delay_ms(200);
    }
}

