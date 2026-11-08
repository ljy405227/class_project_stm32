#ifndef __BSP_PWM_OUT_H
#define __BSP_PWM_OUT_H

#include "main.h"
#include <stdint.h>

void bsp_pwm_out_init(void);
void bsp_pwm_out_set(uint32_t freq_hz, float duty_percent);
void bsp_pwm_out_set_duty(float duty_percent);
void bsp_pwm_out_stop(void);

#endif /* __BSP_PWM_OUT_H */
