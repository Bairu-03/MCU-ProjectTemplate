/*
 * Car.h
 *
 *  Created on: 2024年7月22日
 *      Author: Bairu
 */

#ifndef HARDWARE_CAR_CAR_H_
#define HARDWARE_CAR_CAR_H_

#define Car_F 0
#define Car_B 1
#define Car_Stop 2

void AScar_Init(void);
void AScar_Status(uint8_t Status, float leftMotorDuty, float rightMotorDuty, float ServoDuty);

#endif /* HARDWARE_CAR_CAR_H_ */
