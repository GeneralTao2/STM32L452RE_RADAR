/*
 * oled_menu.c
 *
 *  Created on: Aug 6, 2020
 *      Author: ar725
 */
#include "oled_menu.h"

void OledMenu_Printf(uint16_t x, uint16_t y, FontDef Font, SSD1306_COLOR color,
		const char * format, ... ) {
	char mes[100];
	va_list args;
	va_start(args, format);
	vsprintf(mes, format, args);
	va_end(args);

	ssd1306_SetCursor(x, y);
	ssd1306_WriteString(mes, Font, color);
}

void OledMenu_PrintEchoRadarInfo(uint8_t state, uint16_t val) {
	OledMenu_Printf(0, 0, Font_11x18, White, "ECHO RADAR");
	OledMenu_Printf(0, 20, Font_7x10, White, "State: %s", state ? "on " : "off");
	OledMenu_Printf(0, 32, Font_7x10, White, "Angle: %d deg  ", val);
	OledMenu_Printf(0, 46, Font_6x8, White, "Angle is configurable");
	OledMenu_Printf(0, 56, Font_6x8, White, "only before scanning");
	ssd1306_UpdateScreen();
}

void OledMenu_I2C_Scan(void) {
    char LCD_Buffer[] = "Scanning I2C bus...\r\n";
    DMA_USART_Printf(LCD_Buffer);
    HAL_StatusTypeDef res;
    for(uint16_t i = 0; i < 128; i++) {
        res = HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 1, 10);
        if(res == HAL_OK) {
            char msg[64];
            snprintf(msg, sizeof(msg), "0x%02X", i);
            DMA_USART_Printf(msg);
        } else {
        	DMA_USART_Printf(".");
        }
    }
    DMA_USART_Printf("\r\n");
}

void OledMenu_Init(void) {
	OledMenu_I2C_Scan();
	ssd1306_Init();
}

