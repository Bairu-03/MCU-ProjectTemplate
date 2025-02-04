#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

void TIM2_PWM_Init(uint16_t psc, uint16_t arr);
void TIM2_PWM_Duty(uint8_t CHx, float Duty);

#endif
