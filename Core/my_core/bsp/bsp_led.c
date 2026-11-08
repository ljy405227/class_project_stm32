#include "bsp_led.h"
#include "config.h"

#if CFG_LED_EN

static GPIO_TypeDef *const led_port[8] = {
    LED_F0_GPIO_Port, LED_F1_GPIO_Port, LED_F2_GPIO_Port, LED_F3_GPIO_Port,
    LED_F4_GPIO_Port, LED_F5_GPIO_Port, LED_F6_GPIO_Port, LED_F7_GPIO_Port
};

static const uint16_t led_pin[8] = {
    LED_F0_Pin, LED_F1_Pin, LED_F2_Pin, LED_F3_Pin,
    LED_F4_Pin, LED_F5_Pin, LED_F6_Pin, LED_F7_Pin
};

void bsp_led_init(void)
{
    bsp_led_set_all(0x00U);
}

void bsp_led_on(uint8_t idx)
{
    if (idx < 8U)
    {
        HAL_GPIO_WritePin(led_port[idx], led_pin[idx], GPIO_PIN_RESET);
    }
}

void bsp_led_off(uint8_t idx)
{
    if (idx < 8U)
    {
        HAL_GPIO_WritePin(led_port[idx], led_pin[idx], GPIO_PIN_SET);
    }
}

void bsp_led_toggle(uint8_t idx)
{
    if (idx < 8U)
    {
        HAL_GPIO_TogglePin(led_port[idx], led_pin[idx]);
    }
}

void bsp_led_set_all(uint8_t mask)
{
    uint8_t i;
    for (i = 0U; i < 8U; i++)
    {
        HAL_GPIO_WritePin(led_port[i], led_pin[i], (mask & (1U << i)) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
}

uint8_t bsp_led_get(uint8_t idx)
{
    if (idx >= 8U)
    {
        return 0U;
    }
    return (HAL_GPIO_ReadPin(led_port[idx], led_pin[idx]) == GPIO_PIN_RESET) ? 1U : 0U;
}

#endif /* CFG_LED_EN */
