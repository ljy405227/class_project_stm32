#ifndef __BSP_SEG_H
#define __BSP_SEG_H

#include "main.h"
#include <stdint.h>

void bsp_seg_init(void);
void bsp_seg_clear(void);
void bsp_seg_show_digit(uint8_t pos, uint8_t digit);

#endif /* __BSP_SEG_H */
