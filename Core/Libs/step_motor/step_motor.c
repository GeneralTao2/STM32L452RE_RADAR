/*
 * step_motor.c
 *
 *  Created on: Jun 6, 2020
 *      Author: ar725
 */
#include "step_motor.h"

void StepMotorInit(StepMotor_HandleTypeDef *motor) {
	motor->direction = 1;
	motor->phase = 1;
	motor->position = 0;
}

/* Sets the state of the step motor by ruling on 4 сoils */
void StepMotorWrite(StepMotor_HandleTypeDef *motor, GPIO_PinState ps1,
												GPIO_PinState ps2,
												GPIO_PinState ps3,
												GPIO_PinState ps4) {
	HAL_GPIO_WritePin(motor->p1.GPIO, motor->p1.PIN, ps1);
	HAL_GPIO_WritePin(motor->p2.GPIO, motor->p2.PIN, ps2);
	HAL_GPIO_WritePin(motor->p3.GPIO, motor->p3.PIN, ps3);
	HAL_GPIO_WritePin(motor->p4.GPIO, motor->p4.PIN, ps4);
}

/* Alternates between 4 states of the step motor:
 * 1: 1 0 0 0
 * 2: 0 1 0 0
 * 3: 0 0 1 0
 * 4: 0 0 0 1
 */
void StepMotorRotate(StepMotor_HandleTypeDef *motor) {
	if(motor->position == -1000) {
		motor->position = motor->position;
	}
	switch(motor->phase) {
	case 1:
		StepMotorWrite(motor, 1, 0, 0, 0);
	break;
	case 2:
		StepMotorWrite(motor, 0, 1, 0, 0);
	break;
	case 3:
		StepMotorWrite(motor, 0, 0, 1, 0);
	break;
	case 4:
		StepMotorWrite(motor, 0, 0, 0, 1);
	break;
	default:
		StepMotorWrite(motor, 0, 0, 0, 0);
	}

	if(motor->direction == 1) {
		motor->phase++;
		motor->position++;
	} else {
		motor->phase--;
		motor->position--;
	}
	if(motor->phase >= 5) {
		motor->phase = 1;
	}
	if(motor->phase <= 0) {
		motor->phase = 4;
	}
}

