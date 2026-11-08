#ifndef __DRV_W25Q128_H
#define __DRV_W25Q128_H

#include "main.h"
#include <stdint.h>

typedef struct
{
    SPI_HandleTypeDef *hspi;
    GPIO_TypeDef *cs_port;
    uint16_t cs_pin;
    uint32_t timeout;
} drv_w25q128_t;

void drv_w25q128_init(drv_w25q128_t *dev, SPI_HandleTypeDef *hspi,
                      GPIO_TypeDef *cs_port, uint16_t cs_pin, uint32_t timeout);
uint32_t drv_w25q128_read_id(drv_w25q128_t *dev);
int drv_w25q128_read(drv_w25q128_t *dev, uint32_t addr, uint8_t *buf, uint32_t len);
int drv_w25q128_page_write(drv_w25q128_t *dev, uint32_t addr, const uint8_t *buf, uint32_t len);
int drv_w25q128_sector_erase(drv_w25q128_t *dev, uint32_t addr);
int drv_w25q128_wait_busy(drv_w25q128_t *dev);

#endif /* __DRV_W25Q128_H */
