#ifndef __BSP_SPIFLASH_H
#define __BSP_SPIFLASH_H

#include "main.h"
#include <stdint.h>

void bsp_spiflash_init(void);
uint32_t bsp_spiflash_read_id(void);
int bsp_spiflash_read(uint32_t addr, uint8_t *buf, uint32_t len);
int bsp_spiflash_page_write(uint32_t addr, const uint8_t *buf, uint32_t len);
int bsp_spiflash_sector_erase(uint32_t addr);

#endif /* __BSP_SPIFLASH_H */
