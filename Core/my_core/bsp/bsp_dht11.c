#include "bsp_dht11.h"
#include "config.h"
#include "drv_dht11.h"

#if CFG_DHT11_EN

static drv_dht11_t s_dht11;

void bsp_dht11_init(void)
{
    drv_dht11_init(&s_dht11, DHT11_DATA_GPIO_Port, DHT11_DATA_Pin);
}

int bsp_dht11_read(uint8_t *temp, uint8_t *hum)
{
    return drv_dht11_read(&s_dht11, temp, hum);
}

#endif /* CFG_DHT11_EN */
