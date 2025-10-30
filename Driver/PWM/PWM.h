/*
 * PWM.h
 *
 *  Created on: 2024年7月15日
 *      Author: Bairu
 */

#ifndef HARDWARE_PWM_PWM_H_
#define HARDWARE_PWM_PWM_H_

void TA0_PWM_Init(uint8_t PWMnum, uint16_t clockSource, uint16_t psc, uint16_t arr);
void TA0_PWM_Duty(uint8_t CHx, float Duty);
void TA1_PWM_Init(uint8_t PWMnum, uint16_t clockSource, uint16_t psc, uint16_t arr);
void TA1_PWM_Duty(uint8_t CHx, float Duty);
void TA2_PWM_Init(uint8_t PWMnum, uint16_t clockSource, uint16_t psc, uint16_t arr);
void TA2_PWM_Duty(uint8_t CHx, float Duty);

#endif /* HARDWARE_PWM_PWM_H_ */
