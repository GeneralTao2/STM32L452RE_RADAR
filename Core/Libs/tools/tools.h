/*
 * tools.h
 *
 *  Created on: 13 июн. 2020 г.
 *      Author: ar725
 */

#ifndef SRC_TOOLS_H_
#define SRC_TOOLS_H_

#include "stm32l4xx_hal.h"

typedef struct GPIO_PinConfigs {
	GPIO_TypeDef *GPIO;
	uint16_t PIN;
} GPIO_PinConfigs;

#endif /* SRC_TOOLS_H_ */
