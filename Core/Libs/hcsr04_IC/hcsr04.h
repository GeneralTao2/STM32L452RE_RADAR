/*
 * hcsr04.h
 *
 *  Created on: Jun 20, 2020
 *      Author: ar725
 */

#ifndef LIBS_HCSR04_HCSR04_H_
#define LIBS_HCSR04_HCSR04_H_

/*
 * To use HC-SR04 need to do some things in code-generator:
 * 1. Set Output GPIO pin for Trigg pin;
 * 2. Use high frequency (> 70 MHz) in corresponding periphery;
 * 3. Select timer and set some channel to Input Capture direct mode;
 * 4. In Parameter Settings set Prescaler to [frequency] / 1 M - 1;
 * 5. Set counter period to max (in case of 16-bit - 65534);
 * 6. Polarity Selection should be in Rising Mode;
 * 7. In NVIC check Capture Compare or Global Interrupt;
 * 8. Define HCSR04_HandleTypeDef structure and configure needed (!) fields;
 * 9. Describe somewhere HAL_TIM_IC_CaptureCallback function and contain
 * 	HCSR04_CaptureCallback(htim, [hcsr]) into him.
 * Don't forget to call HCSR04_Init somewhere before main loop.
 * To read distance you need firstly call HCSR04_ReadDistance somewhere in main loop.
 * Take in mind, that HCSR04_ReadDistance is sync function.
 * VCC should be connected to 5V supply pin.
 */

/* Same microcontroller settings */
#include "../tools/tools.h"

/* Allows to use 1 us ((micro second) delays
 * https://github.com/keatis/dwt_delay
 */
#include "../dwt_delay/dwt_delay.h"

/* HCSR04 pin settings */
typedef struct HCSR04_HandleTypeDef {
	/*! Trigger pin configs */
	GPIO_PinConfigs trig;

	/*! Input Capture (IC) timer */
	TIM_HandleTypeDef *htim;

	/*! Input Capture (IC) channel [TIM_CHANNEL_x] */
	volatile uint8_t channel;

	/* Contains distance in cm (to an obstacle) */
	volatile uint32_t distance;

	/* Need to indicate begin and end of input pulse */
	volatile uint8_t isFirstCaptured;

	/* Capture value in pulse beginning */
	volatile uint32_t IC_firstValue;
} HCSR04_HandleTypeDef;

void HCSR04_Init(HCSR04_HandleTypeDef *hcsr);

void HCSR04_CaptureCallback(TIM_HandleTypeDef *htim, HCSR04_HandleTypeDef *hcsr, void (*callback)(void));

void HCSR04_ReadDistance(HCSR04_HandleTypeDef *hcsr);

uint32_t HCSR04_GetDistance(HCSR04_HandleTypeDef *hcsr04);

#endif /* LIBS_HCSR04_HCSR04_H_ */
