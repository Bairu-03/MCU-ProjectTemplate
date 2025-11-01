#ifndef __LED_H
#define __LED_H

#include "stm8l15x.h"

#define LED3_PORT GPIOE
#define LED3_PIN GPIO_Pin_7

#define LED4_PORT GPIOC
#define LED4_PIN GPIO_Pin_7

typedef enum
{
    LED3,
    LED4
} LED_Num_TypeDef;

extern volatile uint16_t g_LED_Blink_Interval;

void LED_Config(LED_Num_TypeDef LED_Num);
void LED_Toggle(LED_Num_TypeDef LED_Num);
void LED_On(LED_Num_TypeDef LED_Num);
void LED_Off(LED_Num_TypeDef LED_Num);

#endif /* __LED_H */
