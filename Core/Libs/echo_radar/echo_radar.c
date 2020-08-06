/*
 * encho_radar.c
 *
 *  Created on: Jul 16, 2020
 *      Author: ar725
 */

#include "echo_radar.h"

/* Updates stepAngle and mapLength relative to current coverage spectrum */
void EchoRadar_UpdateViewing(void) {
	echoRadar.stepAngle = EncoderGetTublerState(&encoder) * ECHO_MOTOR_STPES_IN_ONE_DEGREE;
	echoRadar.mapLength = ECHO_MOTOR_VIEWING_ANGLE * 2 / echoRadar.stepAngle;
}

/* Determines motion via comparing values in run-time map and standard map */
void EchoRadar_Compare(void) {
	if(echoRadar.mapIndex > 2 && echoRadar.mapIndex < ECHO_MOTOR_VIEWING_ANGLE*2 - 4) {
		uint8_t localErrorStrength = 0;
		for(uint16_t i=echoRadar.mapIndex-2; i<=echoRadar.mapIndex+2; i++) {
			if(abs(echoRadar.standardMap[echoRadar.mapIndex] - echoRadar.map[echoRadar.mapIndex]) > 10) {
				localErrorStrength++;
			}
		}
		if(localErrorStrength>4) {
			echoRadar.globalErrorStrength++;
			if(echoRadar.globalErrorStrength>3) {
			 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
			}
		} else {
			 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
			echoRadar.globalErrorStrength = 0;
		}
	}
}

/* Enables motor rotation */
void EchoRadar_EnableMotor(void) {
	HAL_TIM_Base_Start(&htim6);
	HAL_TIM_Base_Start_IT(&htim6);
	if(echoRadar.standardMapReadingFlag) {
		echoRadar.standardMap[echoRadar.mapIndex] = HCSR04_GetDistance(&hcsr);
	} else {
		echoRadar.map[echoRadar.mapIndex] = HCSR04_GetDistance(&hcsr);
		EchoRadar_Compare();
	}
	if(motor.direction) {
		echoRadar.mapIndex++;
		if(motor.position + echoRadar.stepAngle  >= ECHO_MOTOR_VIEWING_ANGLE) {
				motor.direction = 0;
		}
	} else {
		echoRadar.mapIndex--;
		if(motor.position - echoRadar.stepAngle <= -ECHO_MOTOR_VIEWING_ANGLE) {
			echoRadar.standardMapReadingFlag = 0;
		  motor.direction = 1;
		}
	}
	echoRadar.HCSR_State = 0;
}

/* Disables motor rotation */
void EchoRadar_DisableMotor(void) {
	HAL_TIM_Base_Stop(&htim6);
	HAL_TIM_Base_Stop_IT(&htim6);
	echoRadar.HCSR_State = 1;
}

/* Handler, that called in timer interrupt
 * Performs stepper motor rotation
 */
void EchoRadar_MotorTimerHandler(void) {
	if(echoRadar.counter >= echoRadar.stepAngle) {
		EchoRadar_DisableMotor();
		echoRadar.counter = 0;
	} else {
		StepMotorRotate(&motor);
		echoRadar.counter++;
	}
}

/* Initialize global variables of this system */
void EchoRadar_Init(void) {
	EchoRadar_UpdateViewing();
	motor.position = -ECHO_MOTOR_VIEWING_ANGLE + 1;
	echoRadar.counter = 0;
	echoRadar.HCSR_State = 1;
	echoRadar.mapIndex = 0;
	echoRadar.standardMapReadingFlag = 1;
	echoRadar.messageCounter = 0;
	echoRadar.lastDirection = motor.direction;
	echoRadar.buttonNotPressed = 1;
	echoRadar.lastButtonState = EncoderGetButtonState(&encoder);
}

/* Processes interface working
 * Performs motor rotation and echo scanning
 */
void EchoRadar_Processing(void) {
	uint8_t currentButtonState = EncoderGetButtonState(&encoder);

	/* Interface processing */
	if(echoRadar.buttonNotPressed) {
		OledMenu_PrintEchoRadarInfo(0, EncoderGetTublerState(&encoder));
	}
	if(echoRadar.buttonNotPressed && currentButtonState != echoRadar.lastButtonState) {
		OledMenu_PrintEchoRadarInfo(currentButtonState, EncoderGetTublerState(&encoder));
	}
	echoRadar.lastButtonState = currentButtonState;

	/* Motor rotation and echo scanning performing */
	if(currentButtonState) {
    if(echoRadar.buttonNotPressed) {
    	EchoRadar_UpdateViewing();
    }
    echoRadar.buttonNotPressed = 0;
		if(echoRadar.HCSR_State) {
			HCSR04_ReadDistance(&hcsr);
		}
	} else {
		StepMotorWrite(&motor, 0, 0, 0, 0);
	}
}

/* Sends message, that contain real-time map and its id via USART
 * Model: {array=<text>}{id=<text>}
 */
void EchoRadar_SendMessage(void) {
	char localBufer[32];
	strcpy((char*)DMA_USART_GetBuffer(), "{array=");
	if(motor.direction != echoRadar.lastDirection) {
		for(uint16_t i=0; i<echoRadar.mapLength; i++) {
			sprintf(localBufer, "%d ", echoRadar.map[i]);
			strcat((char*)DMA_USART_GetBuffer(), localBufer);
		}
		sprintf(localBufer, "}{id=%d}", echoRadar.messageCounter);
		strcat((char*)DMA_USART_GetBuffer(), localBufer);
		DMA_USART_Print();
		echoRadar.messageCounter++;
	}
	echoRadar.lastDirection = motor.direction;
}
