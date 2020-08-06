/*
 * encho_radar.h
 *
 *  Created on: Jul 16, 2020
 *      Author: ar725
 */

#ifndef LIBS_ECHO_RADAR_ECHO_RADAR_H_
#define LIBS_ECHO_RADAR_ECHO_RADAR_H_

/* Including needed libs */
#include "../Libs/encoder/encoder.h"
#include "../Libs/step_motor/step_motor.h"
#include "../Libs/hcsr04_IC/hcsr04.h"
#include <stdlib.h>
#include "../dma_usart/dma_usart.h"
#include "../oled_menu/oled_menu.h"

/* Stepper motor coverage spectrum in motor steps */
#define ECHO_MOTOR_VIEWING_ANGLE 525

/* Stepper motor step quantity in one degree */
#define ECHO_MOTOR_STPES_IN_ONE_DEGREE 5.833333333

/* Maximum difference between standard and run-time distance */
#define ECHO_MOTOR_MAX_DIFF 10

/* Including already created and initialized structs from main.c */
extern HCSR04_HandleTypeDef hcsr;
extern StepMotor_HandleTypeDef motor;
extern TIM_HandleTypeDef htim6;
extern Encoder_HandleTypeDef encoder;

/* System struct for containing global variables */
static struct EchoRadar {

	/* Stepper motor steps between scanning processing */
	int16_t stepAngle;

	/* Counter for counting steps between scanning processing */
	uint16_t counter;

	/* Flag for enabling/disabling scanning processing */
	uint8_t  HCSR_State;

	/* Map, that contains standard scanning values
	 * Fills up after MC starting once
	 * Needs for comparison standard scanning values and real-time scanning values
	 */
	uint16_t standardMap[ECHO_MOTOR_VIEWING_ANGLE * 2];

	/* Flag for enabling/disabling standard map filling up */
	uint8_t standardMapReadingFlag;

	/* Counter for determining deviations quantity in comparison processing */
	uint8_t globalErrorStrength;

	/* Map, that contains real-time scanning values */
	uint16_t map[ECHO_MOTOR_VIEWING_ANGLE * 2];

	/* Quantity of scanning in corresponding coverage spectrum */
	uint16_t mapLength;

	/* Index of current writable cell in current map */
	uint16_t mapIndex;

	/* Counts sent messages. Needs for message identification */
	uint16_t messageCounter;

	/* Stepper motor direction in previous step */
  uint8_t lastDirection;

  /* Flag, that is
   *  1, if button never pressed
   *  0, if button pressed at least once
   */
  uint8_t buttonNotPressed;

  /* Button state in previous loop execution */
  uint8_t lastButtonState;
} echoRadar;

void EchoRadar_Init(void);

void EchoRadar_Processing(void);

void EchoRadar_SendMessage(void);

void EchoRadar_UpdateViewing(void);

void EchoRadar_EnableMotor(void);

void EchoRadar_MotorTimerHandler(void);



#endif /* LIBS_ECHO_RADAR_ECHO_RADAR_H_ */
