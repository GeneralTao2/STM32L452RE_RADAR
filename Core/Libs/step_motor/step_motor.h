/*
 * step_motor.h
 *
 *  Created on: Jun 6, 2020
 *      Author: ar725
 */

#ifndef STEP_MOTOR_H_
#define STEP_MOTOR_H_

#include "../tools/tools.h"

typedef struct StepMotor_HandleTypeDef {
	GPIO_PinConfigs p1;
	GPIO_PinConfigs p2;
	GPIO_PinConfigs p3;
	GPIO_PinConfigs p4;

	/* Direction of rotation 0/1 */
	uint8_t direction;

	/* Working state (off/on) 0/1 */
	uint8_t state;

	/* Current phase [1,4] */
	uint8_t phase;

	/* Delay between steps */
	uint32_t delay;

	/* Counter for creating of delay */
	uint32_t progTime;

	/* Current position (in passes) */
	int16_t position;
} StepMotor_HandleTypeDef;

void StepMotorInit(StepMotor_HandleTypeDef *motor);

void StepMotorWrite(StepMotor_HandleTypeDef *motor,
												GPIO_PinState ps1,
												GPIO_PinState ps2,
												GPIO_PinState ps3,
												GPIO_PinState ps4);

void StepMotorRotate(StepMotor_HandleTypeDef *motor);

#endif /* STEP_MOTOR_H_ */
