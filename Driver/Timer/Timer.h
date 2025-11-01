#ifndef __TIMER_H
#define __TIMER_H

#include "stm8l15x.h"
#include "stdbool.h"

extern volatile uint32_t g_BlockDelayCount;
extern volatile uint32_t g_NonBlockDelayCount;

typedef enum
{
    TIM1_PWM_OC1,   // PD2
    TIM1_PWM_OC2,   // PD4
    TIM1_PWM_OC3    // PD5
}TIM1_PWMChannel_TypeDef;

void TIM1_PWMConfig(uint16_t TIM1_Prescaler, uint16_t TIM1_Period);
void TIM1_SetPWMDuty(uint16_t TIM1_Period, TIM1_PWMChannel_TypeDef PWM_Channel, uint8_t Duty);
void TIM2_Config(TIM2_Prescaler_TypeDef TIM2_Prescaler, uint16_t TIM2_Period);
void TIM4_Config(TIM4_Prescaler_TypeDef TIM4_Prescaler, uint8_t TIM4_Period);

#endif /* __TIMER_H */
