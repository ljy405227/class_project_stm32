#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "main.h"
#include <stdint.h>

#define BSP_ADC_CHANNELS 5U

void bsp_adc_init(void);
uint16_t bsp_adc_get(uint8_t ch);
float bsp_adc_get_voltage(uint8_t ch);

#endif /* __BSP_ADC_H */
