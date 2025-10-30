/*
 * Encoder.h
 *
 *  Created on: 2024年7月15日
 *      Author: Bairu
 */

#ifndef HARDWARE_ENCODER_ENCODER_H_
#define HARDWARE_ENCODER_ENCODER_H_

extern uint16_t P20_Count;
extern uint16_t P22_Count;

void Encoder_Init(void);
uint16_t getP20PulseNum(void);
uint16_t getP22PulseNum(void);

#endif /* HARDWARE_ENCODER_ENCODER_H_ */
