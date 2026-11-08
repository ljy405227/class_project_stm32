#ifndef __DRV_AT24C02_H
#define __DRV_AT24C02_H

#include "main.h"
#include <stdint.h>

typedef struct
{
    I2C_HandleTypeDef *hi2c;
    uint8_t addr7;
    uint32_t timeout;
} drv_at24c02_t;

void drv_at24c02_init(drv_at24c02_t *dev, I2C_HandleTypeDef *hi2c, uint8_t addr7, uint32_t timeout);
HAL_StatusTypeDef drv_at24c02_read(drv_at24c02_t *dev, uint8_t mem_addr, uint8_t *buf, uint16_t len);
HAL_StatusTypeDef drv_at24c02_write(drv_at24c02_t *dev, uint8_t mem_addr, const uint8_t *buf, uint16_t len);

#endif /* __DRV_AT24C02_H */
