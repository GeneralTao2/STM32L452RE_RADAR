/*
 * oled_menu.h
 *
 *  Created on: Aug 6, 2020
 *      Author: ar725
 */
#ifndef LIBS_OLED_MENU_OLED_MENU_H_
#define LIBS_OLED_MENU_OLED_MENU_H_

#include "../tools/tools.h"
#include "../dma_usart/dma_usart.h"
#include "../ssd1306/ssd1306.h"
#include "../ssd1306/ssd1306_tests.h"

extern I2C_HandleTypeDef hi2c1;

void OledMenu_Init(void);

void OledMenu_I2C_Scan(void);

void OledMenu_Printf(uint16_t x, uint16_t y, FontDef Font, SSD1306_COLOR color,
		const char * format, ... );

void OledMenu_PrintEchoRadarInfo(uint8_t state, uint16_t val);

#endif /* LIBS_OLED_MENU_OLED_MENU_H_ */
