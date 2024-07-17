/*
 * PWM.h
 *
 *  Created on: 2024��7��15��
 *      Author: Bairu
 */

#ifndef HARDWARE_PWM_PWM_H_
#define HARDWARE_PWM_PWM_H_

void TA0_PWM_Init(uint16_t clockSource, uint16_t psc, uint16_t arr);
void TA0_PWM_Duty(uint8_t CHx, float Duty);

#endif /* HARDWARE_PWM_PWM_H_ */
