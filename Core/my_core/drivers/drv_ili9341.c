#include "drv_ili9341.h"
#include "drv_font8x16.h"
#include "my_tool.h"
#include <stddef.h>

#define LCD_FSMC_BASE   ((uint32_t)0x6C000000U)
#define LCD_CMD         (*(volatile uint16_t *)LCD_FSMC_BASE)
#define LCD_DATA        (*(volatile uint16_t *)(LCD_FSMC_BASE + (1U << 7)))

static void lcd_cmd(uint8_t cmd)
{
    LCD_CMD = (uint16_t)cmd;
}

static void lcd_data8(uint8_t data)
{
    LCD_DATA = (uint16_t)data;
}

static void lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    lcd_cmd(0x2AU);
    lcd_data8((uint8_t)(x0 >> 8));
    lcd_data8((uint8_t)x0);
    lcd_data8((uint8_t)(x1 >> 8));
    lcd_data8((uint8_t)x1);

    lcd_cmd(0x2BU);
    lcd_data8((uint8_t)(y0 >> 8));
    lcd_data8((uint8_t)y0);
    lcd_data8((uint8_t)(y1 >> 8));
    lcd_data8((uint8_t)y1);

    lcd_cmd(0x2CU);
}

void drv_ili9341_init(void)
{
    lcd_cmd(0x01U);          /* software reset */
    my_delay_ms(120U);

    lcd_cmd(0xCFU);
    lcd_data8(0x00U); lcd_data8(0xC1U); lcd_data8(0x30U);

    lcd_cmd(0xEDU);
    lcd_data8(0x64U); lcd_data8(0x03U); lcd_data8(0x12U); lcd_data8(0x81U);

    lcd_cmd(0xE8U);
    lcd_data8(0x85U); lcd_data8(0x00U); lcd_data8(0x78U);

    lcd_cmd(0xCBU);
    lcd_data8(0x39U); lcd_data8(0x2CU); lcd_data8(0x00U); lcd_data8(0x34U); lcd_data8(0x02U);

    lcd_cmd(0xF7U);
    lcd_data8(0x20U);

    lcd_cmd(0xEAU);
    lcd_data8(0x00U); lcd_data8(0x00U);

    lcd_cmd(0xC0U);
    lcd_data8(0x23U);

    lcd_cmd(0xC1U);
    lcd_data8(0x10U);

    lcd_cmd(0xC5U);
    lcd_data8(0x3EU); lcd_data8(0x28U);

    lcd_cmd(0xC7U);
    lcd_data8(0x86U);

    lcd_cmd(0x36U);          /* MX=0, MY=0, MV=1, BGR=1: 320x240 landscape */
    lcd_data8(0x28U);

    lcd_cmd(0x3AU);
    lcd_data8(0x55U);        /* 16-bit RGB565 */

    lcd_cmd(0xB1U);
    lcd_data8(0x00U); lcd_data8(0x18U);

    lcd_cmd(0xB6U);
    lcd_data8(0x08U); lcd_data8(0x82U); lcd_data8(0x27U);

    lcd_cmd(0xF2U);
    lcd_data8(0x00U);

    lcd_cmd(0x26U);
    lcd_data8(0x01U);

    lcd_cmd(0xE0U);
    lcd_data8(0x0FU); lcd_data8(0x31U); lcd_data8(0x2BU); lcd_data8(0x0CU);
    lcd_data8(0x0EU); lcd_data8(0x08U); lcd_data8(0x4EU); lcd_data8(0xF1U);
    lcd_data8(0x37U); lcd_data8(0x07U); lcd_data8(0x10U); lcd_data8(0x03U);
    lcd_data8(0x0EU); lcd_data8(0x09U); lcd_data8(0x00U);

    lcd_cmd(0xE1U);
    lcd_data8(0x00U); lcd_data8(0x0EU); lcd_data8(0x14U); lcd_data8(0x03U);
    lcd_data8(0x11U); lcd_data8(0x07U); lcd_data8(0x31U); lcd_data8(0xC1U);
    lcd_data8(0x48U); lcd_data8(0x08U); lcd_data8(0x0FU); lcd_data8(0x0CU);
    lcd_data8(0x31U); lcd_data8(0x36U); lcd_data8(0x0FU);

    lcd_cmd(0x11U);          /* sleep out */
    my_delay_ms(120U);
    lcd_cmd(0x29U);          /* display on */
    my_delay_ms(20U);
}

void drv_ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint32_t count;

    if ((x >= DRV_LCD_WIDTH) || (y >= DRV_LCD_HEIGHT) || (w == 0U) || (h == 0U))
    {
        return;
    }
    if ((x + w) > DRV_LCD_WIDTH)
    {
        w = (uint16_t)(DRV_LCD_WIDTH - x);
    }
    if ((y + h) > DRV_LCD_HEIGHT)
    {
        h = (uint16_t)(DRV_LCD_HEIGHT - y);
    }

    lcd_set_window(x, y, (uint16_t)(x + w - 1U), (uint16_t)(y + h - 1U));
    count = (uint32_t)w * (uint32_t)h;
    while (count-- != 0U)
    {
        LCD_DATA = color;
    }
}

void drv_ili9341_clear(uint16_t color)
{
    drv_ili9341_fill_rect(0U, 0U, DRV_LCD_WIDTH, DRV_LCD_HEIGHT, color);
}

static void lcd_draw_char(uint16_t x, uint16_t y, char ch, uint16_t fg, uint16_t bg, uint8_t scale)
{
    const uint8_t *glyph;
    uint8_t row;
    uint8_t col;
    uint8_t sy;
    uint8_t sx;
    uint8_t bit;

    if ((ch < (char)DRV_FONT8X16_FIRST) || (ch > (char)DRV_FONT8X16_LAST))
    {
        ch = '?';
    }
    glyph = g_drv_font8x16[(uint8_t)ch - DRV_FONT8X16_FIRST];

    lcd_set_window(x, y,
                   (uint16_t)(x + DRV_FONT8X16_WIDTH * scale - 1U),
                   (uint16_t)(y + DRV_FONT8X16_HEIGHT * scale - 1U));

    for (row = 0U; row < DRV_FONT8X16_HEIGHT; row++)
    {
        for (sy = 0U; sy < scale; sy++)
        {
            for (col = 0U; col < DRV_FONT8X16_WIDTH; col++)
            {
                bit = (uint8_t)((glyph[row] >> (7U - col)) & 0x01U);
                for (sx = 0U; sx < scale; sx++)
                {
                    LCD_DATA = (bit != 0U) ? fg : bg;
                }
            }
        }
    }
}

void drv_ili9341_draw_string(uint16_t x, uint16_t y, const char *text, uint16_t fg, uint16_t bg, uint8_t scale)
{
    if ((text == NULL) || (scale == 0U))
    {
        return;
    }

    while (*text != '\0')
    {
        if ((uint32_t)x + (DRV_FONT8X16_WIDTH * scale) > DRV_LCD_WIDTH)
        {
            break;
        }
        lcd_draw_char(x, y, *text, fg, bg, scale);
        x = (uint16_t)(x + DRV_FONT8X16_WIDTH * scale);
        text++;
    }
}
