#ifndef __DRV_DHT11_H
#define __DRV_DHT11_H

#include "main.h"
#include <stdint.h>

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t last_read;
} drv_dht11_t;

void drv_dht11_init(drv_dht11_t *dev, GPIO_TypeDef *port, uint16_t pin);
int drv_dht11_read(drv_dht11_t *dev, uint8_t *temp, uint8_t *hum);

#endif /* __DRV_DHT11_H */
