/*
 * encoder.c
 *
 *  Created on: 13 июн. 2020 г.
 *      Author: ar725
 */

#include "encoder.h"


/* Initialize encoder */
void EncoderInit(Encoder_HandleTypeDef *encoder) {
	encoder->buttonState = 0;
	encoder->tumblerState = 1;
	encoder->tumbler_CLC_DT_last = 0;
	encoder->tumblerStepCounter = 0;
	encoder->buttonProgTime = HAL_GetTick();
	encoder->tumbler_CLC_DT_last = 0;
	encoder->tumbler_CLC_DT_last |=
			(HAL_GPIO_ReadPin(encoder->CLK.GPIO, encoder->CLK.PIN) << 1);
	encoder->tumbler_CLC_DT_last |=
			(HAL_GPIO_ReadPin(encoder->DT.GPIO, encoder->CLK.PIN) << 0);
	encoder->tumbler_TIM_CounterLast = 30000;
	if(encoder->tumblerEncoder_TIM->Instance) {
		encoder->tumblerEncoder_TIM->Instance->CNT = 30000;
		HAL_TIM_Encoder_Start(encoder->tumblerEncoder_TIM, TIM_CHANNEL_ALL);
	}
}

/* Reads encoder tumbler pin states and compares
 * with previous iteration state. If differ, then defines
 * rotation direction and refreshes encoder->tumblerState.
 * Should be called in external interrupt.
 */
void EncoderTumblerExternInterrupt(Encoder_HandleTypeDef *encoder) {
	uint8_t CLK_value = 0;
	CLK_value =  HAL_GPIO_ReadPin(encoder->CLK.GPIO, encoder->CLK.PIN);
	uint8_t DT_value = HAL_GPIO_ReadPin(encoder->DT.GPIO, encoder->DT.PIN);
	uint8_t CLC_DT = (CLK_value << 1) | (DT_value << 0);
	if(encoder->tumbler_CLC_DT_last == 1 && CLC_DT == 0) {
		EncoderTomblerInc(encoder);
	} else if(encoder->tumbler_CLC_DT_last == 2 && CLC_DT == 0) {
		EncoderTomblerDec(encoder);
	}
	encoder->tumbler_CLC_DT_last = CLC_DT;
}

/* Reads encoder tumbler state via timer, configured to encoder
 * mode. Refreshes encoder->tumblerState on every second pulse.
 * Should be called in third party timer with frequency ~ 1 kHz
 */
void EncoderTumblerTimerInterrupt(Encoder_HandleTypeDef *encoder) {
	uint16_t TIM_Counter = encoder->tumblerEncoder_TIM->Instance->CNT;
	//uint16_t TIM_Counter = TIM2->CNT;
	if(TIM_Counter != encoder->tumbler_TIM_CounterLast) {
		if(TIM_Counter > encoder->tumbler_TIM_CounterLast) {
			encoder->tumblerStepCounter += TIM_Counter - encoder->tumbler_TIM_CounterLast;
			/* one pas of encoder rotation creates 2 pulses
			 * so we count 2 pulses to update encoder->tumblerState
			 */
			if(encoder->tumblerStepCounter >= 2) {
				EncoderTomblerInc(encoder);
				encoder->tumblerStepCounter = 0;
			}
		} else {
			encoder->tumblerStepCounter += encoder->tumbler_TIM_CounterLast - TIM_Counter;
			if(encoder->tumblerStepCounter >= 2) {
				EncoderTomblerDec(encoder);
				encoder->tumblerStepCounter = 0;
			}
		}
	}
	encoder->tumbler_TIM_CounterLast = TIM_Counter;
}

/* Reads encoder button state and refreshes encoder->buttonState.
 * Should be called in external interrupt.
 */
void EncoderButtonExternInterrupt(Encoder_HandleTypeDef *encoder) {
	if(HAL_GetTick() - encoder->buttonProgTime > ENCODER_BUTTON_PROGTIME_DELAY) {
		if(HAL_GPIO_ReadPin(encoder->SW.GPIO, encoder->SW.PIN)) {
			EncoderTougleButton(encoder);
		}
		encoder->buttonProgTime = HAL_GetTick();
	}
}

/* Returns tumblerState */
int16_t EncoderGetTublerState(Encoder_HandleTypeDef *encoder) {
	return encoder->tumblerState;
}

/* Returns buttonState */
uint8_t EncoderGetButtonState(Encoder_HandleTypeDef *encoder) {
	return encoder->buttonState;
}

/* Toggles encoder->buttonState */
void EncoderTougleButton(Encoder_HandleTypeDef *encoder) {
	encoder->buttonState = encoder->buttonState ? 0 : 1;
}

/* Increments encoder->tumblerState, if it is lower then ENCODER_MAX_VAL */
void EncoderTomblerInc(Encoder_HandleTypeDef *encoder) {
	if(encoder->tumblerState < ENCODER_MAX_VAL) {
		encoder->tumblerState++;
	}
}

/* Decrements encoder->tumblerState, if it is higher then ENCODER_MIN_VAL */
void EncoderTomblerDec(Encoder_HandleTypeDef *encoder) {
	if(encoder->tumblerState > ENCODER_MIN_VAL) {
			encoder->tumblerState--;
		}
}
