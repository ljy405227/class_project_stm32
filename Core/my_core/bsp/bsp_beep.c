#include "bsp_beep.h"
#include "config.h"
#include "my_tool.h"

#if CFG_BEEP_EN

#if BEEP_ACTIVE_LOW
#define BEEP_ON_STATE   GPIO_PIN_RESET
#define BEEP_OFF_STATE  GPIO_PIN_SET
#else
#define BEEP_ON_STATE   GPIO_PIN_SET
#define BEEP_OFF_STATE  GPIO_PIN_RESET
#endif

void bsp_beep_init(void)
{
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, BEEP_OFF_STATE);
}

void bsp_beep_on(void)
{
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, BEEP_ON_STATE);
}

void bsp_beep_off(void)
{
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, BEEP_OFF_STATE);
}

void bsp_beep_toggle(void)
{
    HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin);
}

void bsp_beep_ms(uint32_t ms)
{
    bsp_beep_on();
    my_delay_ms(ms);
    bsp_beep_off();
}

#endif /* CFG_BEEP_EN */
