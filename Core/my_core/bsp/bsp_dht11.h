#ifndef __BSP_DHT11_H
#define __BSP_DHT11_H

#include "main.h"
#include <stdint.h>

void bsp_dht11_init(void);
int bsp_dht11_read(uint8_t *temp, uint8_t *hum);

#endif /* __BSP_DHT11_H */
