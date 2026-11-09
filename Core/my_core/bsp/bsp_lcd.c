#include "bsp_lcd.h"
#include "config.h"
#include "drv_ili9341.h"

#if CFG_LCD_EN

void bsp_lcd_init(void)
{
    drv_ili9341_init();
}

void bsp_lcd_clear(uint16_t color)
{
    drv_ili9341_clear(color);
}

void bsp_lcd_text(uint16_t x, uint16_t y, const char *text, uint16_t fg, uint16_t bg, uint8_t scale)
{
    drv_ili9341_draw_string(x, y, text, fg, bg, scale);
}

#endif /* CFG_LCD_EN */
