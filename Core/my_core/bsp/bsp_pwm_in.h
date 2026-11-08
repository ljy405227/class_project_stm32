#ifndef __BSP_PWM_IN_H
#define __BSP_PWM_IN_H

#include "main.h"
#include <stdint.h>

void bsp_pwm_in_init(void);
int bsp_pwm_in_get(uint32_t *freq_hz, float *duty_percent);

#endif /* __BSP_PWM_IN_H */
