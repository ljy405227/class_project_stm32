#include "bsp_pwm_in.h"
#include "config.h"
#include "tim.h"

#if CFG_PWM_IN_EN

static volatile uint32_t s_period;
static volatile uint32_t s_high;
static volatile uint32_t s_freq;
static volatile float s_duty;
static volatile uint8_t s_valid;

void bsp_pwm_in_init(void)
{
    s_period = 0U;
    s_high = 0U;
    s_freq = 0U;
    s_duty = 0.0f;
    s_valid = 0U;

    (void)HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
    (void)HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance != TIM3)
    {
        return;
    }

    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        uint32_t period = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
        if (period > 0U)
        {
            s_period = period;
            s_freq = 1000000U / period;
            if (s_high <= period)
            {
                s_duty = (float)s_high * 100.0f / (float)period;
            }
            s_valid = 1U;
        }
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
        s_high = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
    }
}

int bsp_pwm_in_get(uint32_t *freq_hz, float *duty_percent)
{
    if (s_valid == 0U)
    {
        return -1;
    }
    if (freq_hz != NULL)
    {
        *freq_hz = s_freq;
    }
    if (duty_percent != NULL)
    {
        *duty_percent = s_duty;
    }
    return 0;
}

#endif /* CFG_PWM_IN_EN */
