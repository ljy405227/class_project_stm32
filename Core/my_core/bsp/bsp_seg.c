#include "bsp_seg.h"
#include "config.h"

#if CFG_SEG_EN

/* FSMC NE3 (Bank1, 0x68000000), 16-bit write:
 * D0..D7  -> 段码 a..dp
 * D8..D13 -> 位选 DS1..DS6 (低有效)
 */
#define SEG_FSMC_BASE   ((uint32_t)0x68000000U)

static const uint8_t s_seg_code[10] = {
    0xC0U, 0xF9U, 0xA4U, 0xB0U, 0x99U,
    0x92U, 0x82U, 0xF8U, 0x80U, 0x90U
};

static const uint8_t s_digit_sel[6] = {
    0xFEU, 0xFDU, 0xFBU, 0xF7U, 0xEFU, 0xDFU
};

static void seg_write(uint16_t value)
{
    *(volatile uint16_t *)SEG_FSMC_BASE = value;
}

void bsp_seg_init(void)
{
    seg_write(0xFFFFU);
}

void bsp_seg_clear(void)
{
    seg_write(0xFFFFU);
}

void bsp_seg_show_digit(uint8_t pos, uint8_t digit)
{
    if ((pos < 6U) && (digit < 10U))
    {
        seg_write((uint16_t)(((uint16_t)s_digit_sel[pos] << 8) | s_seg_code[digit]));
    }
}

#endif /* CFG_SEG_EN */
