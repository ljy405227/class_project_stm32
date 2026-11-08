#include "drv_dht11.h"
#include "my_tool.h"
#include "FreeRTOS.h"
#include "task.h"

static int dht_wait_while(drv_dht11_t *dev, GPIO_PinState level, uint32_t timeout_us)
{
    uint32_t t = 0U;
    while (HAL_GPIO_ReadPin(dev->port, dev->pin) == level)
    {
        if (t++ >= timeout_us)
        {
            return -1;
        }
        my_delay_us(1U);
    }
    return 0;
}

void drv_dht11_init(drv_dht11_t *dev, GPIO_TypeDef *port, uint16_t pin)
{
    if (dev == NULL)
    {
        return;
    }
    dev->port = port;
    dev->pin = pin;
    dev->last_read = 0U;
    HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_SET);
}

int drv_dht11_read(drv_dht11_t *dev, uint8_t *temp, uint8_t *hum)
{
    uint8_t data[5] = {0U, 0U, 0U, 0U, 0U};
    uint8_t i;
    int ret = 0;

    if ((dev == NULL) || (temp == NULL) || (hum == NULL))
    {
        return -1;
    }
    if ((dev->last_read != 0U) && ((HAL_GetTick() - dev->last_read) < 1000U))
    {
        return -3;
    }

    HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_RESET);
    my_delay_ms(18U);
    HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_SET);
    my_delay_us(30U);

    taskENTER_CRITICAL();
    do
    {
        if (dht_wait_while(dev, GPIO_PIN_SET, 100U) != 0)   { ret = -1; break; }
        if (dht_wait_while(dev, GPIO_PIN_RESET, 100U) != 0) { ret = -1; break; }
        if (dht_wait_while(dev, GPIO_PIN_SET, 100U) != 0)   { ret = -1; break; }

        for (i = 0U; i < 40U; i++)
        {
            if (dht_wait_while(dev, GPIO_PIN_RESET, 100U) != 0) { ret = -1; break; }
            my_delay_us(40U);
            data[i / 8U] <<= 1;
            if (HAL_GPIO_ReadPin(dev->port, dev->pin) == GPIO_PIN_SET)
            {
                data[i / 8U] |= 1U;
            }
            if (dht_wait_while(dev, GPIO_PIN_SET, 100U) != 0) { ret = -1; break; }
        }
    } while (0);
    taskEXIT_CRITICAL();

    HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_SET);

    if (ret != 0)
    {
        return ret;
    }
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
    {
        return -2;
    }

    dev->last_read = HAL_GetTick();
    *hum = data[0];
    *temp = data[2];
    return 0;
}
