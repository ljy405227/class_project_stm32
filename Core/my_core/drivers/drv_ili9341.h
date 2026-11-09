#ifndef __DRV_ILI9341_H
#define __DRV_ILI9341_H

#include <stdint.h>

#define DRV_LCD_WIDTH   320U
#define DRV_LCD_HEIGHT  240U

void drv_ili9341_init(void);
void drv_ili9341_clear(uint16_t color);
void drv_ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void drv_ili9341_draw_string(uint16_t x, uint16_t y, const char *text, uint16_t fg, uint16_t bg, uint8_t scale);

#endif /* __DRV_ILI9341_H */
