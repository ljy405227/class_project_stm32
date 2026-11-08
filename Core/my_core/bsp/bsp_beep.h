#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include "main.h"
#include <stdint.h>

void bsp_beep_init(void);
void bsp_beep_on(void);
void bsp_beep_off(void);
void bsp_beep_toggle(void);
void bsp_beep_ms(uint32_t ms);

#endif /* __BSP_BEEP_H */
