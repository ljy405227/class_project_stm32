#include "bsp_pwm_out.h"
#include "config.h"
#include "tim.h"
#include "debug.h"

#if CFG_PWM_OUT_EN

#define PWM_OUT_PSC         83U
#define PWM_OUT_TIMER_HZ    1000000U

void bsp_pwm_out_init(void)
{
    __HAL_TIM_SET_PRESCALER(&htim14, PWM_OUT_PSC);
    if (HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1) != HAL_OK)
    {
        DEBUG_ERROR("TIM14 PWM start failed\r\n");
    }
}

void bsp_pwm_out_set(uint32_t freq_hz, float duty_percent)
{
    uint32_t arr;
    uint32_t ccr;

    if (freq_hz < 1U)
    {
        freq_hz = 1U;
    }
    arr = PWM_OUT_TIMER_HZ / freq_hz;
    if (arr == 0U)
    {
        arr = 1U;
    }
    arr -= 1U;
    if (arr > 0xFFFFU)
    {
        arr = 0xFFFFU;
    }

    if (duty_percent < 0.0f)
    {
        duty_percent = 0.0f;
    }
    if (duty_percent > 100.0f)
    {
        duty_percent = 100.0f;
    }
    ccr = (uint32_t)(((float)(arr + 1U) * duty_percent) / 100.0f + 0.5f);
    if (ccr > (arr + 1U))
    {
        ccr = arr + 1U;
    }

    __HAL_TIM_SET_AUTORELOAD(&htim14, arr);
    __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, ccr);
}

void bsp_pwm_out_set_duty(float duty_percent)
{
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim14);
    uint32_t ccr;

    if (duty_percent < 0.0f)
    {
        duty_percent = 0.0f;
    }
    if (duty_percent > 100.0f)
    {
        duty_percent = 100.0f;
    }
    ccr = (uint32_t)(((float)(arr + 1U) * duty_percent) / 100.0f + 0.5f);
    if (ccr > (arr + 1U))
    {
        ccr = arr + 1U;
    }
    __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, ccr);
}

void bsp_pwm_out_stop(void)
{
    (void)HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);
}

#endif /* CFG_PWM_OUT_EN */
