/*
 * encoder.h
 *
 *  Created on: 13 июн. 2020 г.
 *      Author: ar725
 */

#ifndef ENCODER_ENCODER_H_
#define ENCODER_ENCODER_H_

/*
 * To use encoder you need to do some things:
 * *) Define Encoder_HandleTypeDef structure and extern it to stm32xxxx_it.c file.
 * *) Configure tumbler interrupt handler:
 *		a) If you want to use External Interrupts
 * 			1) Set CLK and DT pins to External Interrupt Mode. Pins should be in some interrupt section;
 * 			2) Enable NVIC interrupts for current section;
 * 			3) Set CLK and DT to Rising/Falling edge trigger direction;
 * 			4) Set EncoderTumblerExternInterrupt in corresponding external interrupt function;
 * 		b) If you want to use Encoder Timer;
 * 			1) Select some timer and set Combined Channels to Encoder Mode (be careful for pins, what can be moved);
 * 			2) Set Counter Period to max (in 16-bit case - 65534);
 * 			3) Encoder mode set to TI1;
 * 			4) In GPIO settings set Pull-up to corresponding pins;
 * 			5) Activate another timer and Trigger Event Section to Update Event;
 * 			6) Configure this timer Counter Settings to ~ 1 kHz frequency calling;
 * 				Dont'f forget to call HAL_TIM_Base_Start and HAL_TIM_Base_Start_IT
 * 				Also activate corresponding NVIC
 * 			7) Set EncoderTumblerTimerInterrupt in this timer interrupt function.
 * *) Set SW pin to External Interrupt Mode;
 * *) Enable NVIC interrupts for current section;
 * *) Set CLK and DT to Rising edge trigger direction;
 * *) Set EncoderButtonExternInterrupt in corresponding external interrupt function;
 * Don't forget to call EncoderInit somewhere in the beginning.
 */

#include "../tools/tools.h"

#define ENCODER_BUTTON_PROGTIME_DELAY 1000
#define ENCODER_MAX_VAL 15
#define ENCODER_MIN_VAL 1
typedef struct Encoder_HandleTypeDef {
	/*! CLK pin configs, if using EXT */
	GPIO_PinConfigs CLK;
	/*! DT pin configs, if using EXT */
	GPIO_PinConfigs DT;
	/*! SW pin configs */
	GPIO_PinConfigs SW;

	/*! Timer, configured to Encoder Mode, if using Encoder Timer */
	TIM_HandleTypeDef *tumblerEncoder_TIM;

	/* Button state 0/1 */
	volatile uint8_t buttonState;

	/* Tumbler state */
	volatile int16_t tumblerState;

	/* Counter for prevent contact of contacts of button */
	volatile uint32_t buttonProgTime;

	/* Contains last tumbler pins state, if is used
	 * EncoderTumblerExternInterrupt. [0,3]
	 */
	volatile uint8_t tumbler_CLC_DT_last;

	/* Contains last encoder timer value, if is used
	 * EncoderTumblerTimerInterrupt
	 */
	volatile uint16_t tumbler_TIM_CounterLast;

	/* Counter for counting encoder timer pulses. [0,2] */
	volatile uint8_t tumblerStepCounter;
} Encoder_HandleTypeDef;

void EncoderInit(Encoder_HandleTypeDef *encoder);

void EncoderTumblerExternInterrupt(Encoder_HandleTypeDef *encoder);

void EncoderButtonExternInterrupt(Encoder_HandleTypeDef *encoder);

void EncoderTumblerTimerInterrupt(Encoder_HandleTypeDef *encoder);


uint8_t EncoderGetButtonState(Encoder_HandleTypeDef *encoder);

int16_t EncoderGetTublerState(Encoder_HandleTypeDef *encoder);

void EncoderTougleButton(Encoder_HandleTypeDef *encoder);

void EncoderTomblerInc(Encoder_HandleTypeDef *encoder);

void EncoderTomblerDec(Encoder_HandleTypeDef *encoder);

#endif /* ENCODER_ENCODER_H_ */
