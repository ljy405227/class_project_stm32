#include "my_tool.h"
#include "usart.h"
#include "cmsis_os.h"
#include <stdio.h>

float my_constrain(float val, float min, float max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

void my_dwt_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void my_delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000U);
    while ((DWT->CYCCNT - start) < ticks)
    {
    }
}

void my_delay_ms(uint32_t ms)
{
    if (osKernelGetState() == osKernelRunning)
    {
        osDelay(ms);
    }
    else
    {
        HAL_Delay(ms);
    }
}

#if defined(__CC_ARM)
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
};

FILE __stdout;

void _ttywrch(int ch)
{
    (void)ch;
}

void _sys_exit(int x)
{
    (void)x;
}
#endif

int fputc(int ch, FILE *f)
{
    uint8_t c = (uint8_t)ch;
    (void)f;
    HAL_UART_Transmit(&huart1, &c, 1, 100);
    return ch;
}
