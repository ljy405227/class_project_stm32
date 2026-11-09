#ifndef __BSP_LCD_H
#define __BSP_LCD_H

#include "main.h"
#include <stdint.h>

#define BSP_LCD_BLACK   0x0000U
#define BSP_LCD_WHITE   0xFFFFU
#define BSP_LCD_RED     0xF800U
#define BSP_LCD_GREEN   0x07E0U
#define BSP_LCD_BLUE    0x001FU
#define BSP_LCD_YELLOW  0xFFE0U
#define BSP_LCD_CYAN    0x07FFU

void bsp_lcd_init(void);
void bsp_lcd_clear(uint16_t color);
void bsp_lcd_text(uint16_t x, uint16_t y, const char *text, uint16_t fg, uint16_t bg, uint8_t scale);

#endif /* __BSP_LCD_H */
