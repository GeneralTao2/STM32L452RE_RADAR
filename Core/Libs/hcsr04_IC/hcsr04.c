/*
 * hcsr04.c
 *
 *  Created on: Jun 20, 2020
 *      Author: ar725
 */
#include "hcsr04.h"

/* Returns current saved distance */
uint32_t HCSR04_GetDistance(HCSR04_HandleTypeDef *hcsr04) {
	return hcsr04->distance;
}

/* Initializes HCSR04_HandleTypeDef structure fields */
void HCSR04_Init(HCSR04_HandleTypeDef *hcsr) {
	hcsr->isFirstCaptured = 0;
	hcsr->distance = 0;
	DWT_Init();
}

/* Begins distance reading routine */
void HCSR04_ReadDistance(HCSR04_HandleTypeDef *hcsr) {
	HAL_GPIO_WritePin(hcsr->trig.GPIO, hcsr->trig.PIN, GPIO_PIN_SET);
	DWT_Delay(10);
	HAL_GPIO_WritePin(hcsr->trig.GPIO, hcsr->trig.PIN, GPIO_PIN_RESET);

	HAL_TIM_IC_Start_IT(hcsr->htim, hcsr->channel);
}

/* Calculates and saves in hcsr->distance distance in cm */
void HCSR04_CaptureCallback(TIM_HandleTypeDef *htim, HCSR04_HandleTypeDef *hcsr, void (*callback)(void)) {
	if(htim != hcsr->htim) {
		return;
	}
	/* if the interrupt source is channel_x;
	 * conversion to HAL_TIM_ActiveChannel;
	 * channel_x = channel/4 + 1
	 */
	if (htim->Channel == hcsr->channel/4 + 1) {

			/* if the first value is not captured */
			if (hcsr->isFirstCaptured == 0) {
				/* read the first value */
				hcsr->IC_firstValue = HAL_TIM_ReadCapturedValue(htim, hcsr->channel);
				/* set the first captured as true */
				hcsr->isFirstCaptured = 1;
				/* Now change the polarity to falling edge */
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, hcsr->channel, TIM_INPUTCHANNELPOLARITY_FALLING);
			} else

				/* if the first is already captured */
				if (hcsr->isFirstCaptured == 1) {
				/* read second value */
				uint32_t IC_secandValue = HAL_TIM_ReadCapturedValue(htim, hcsr->channel);
				/* reset the counter */
				__HAL_TIM_SET_COUNTER(htim, 0);

				if (IC_secandValue > hcsr->IC_firstValue) {
					hcsr->distance = IC_secandValue - hcsr->IC_firstValue;
				} else

				if (hcsr->IC_firstValue > IC_secandValue) {
					hcsr->distance = (0xffff - hcsr->IC_firstValue) + IC_secandValue;
				}

				/* turns value from us to cm */
				hcsr->distance *= .034/2;
				/* set it back to false */
				hcsr->isFirstCaptured = 0;
				/* set polarity to rising edge */
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, hcsr->channel, TIM_INPUTCHANNELPOLARITY_RISING);

				HAL_TIM_IC_Stop_IT(hcsr->htim, hcsr->channel);
				callback();
			}
		}
}

