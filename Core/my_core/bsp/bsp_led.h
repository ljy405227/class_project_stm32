#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "main.h"
#include <stdint.h>

void bsp_led_init(void);
void bsp_led_on(uint8_t idx);
void bsp_led_off(uint8_t idx);
void bsp_led_toggle(uint8_t idx);
void bsp_led_set_all(uint8_t mask);
uint8_t bsp_led_get(uint8_t idx);

#endif /* __BSP_LED_H */
