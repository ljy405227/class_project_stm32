#ifndef __BSP_EEPROM_H
#define __BSP_EEPROM_H

#include "main.h"
#include <stdint.h>

void bsp_eeprom_init(void);
HAL_StatusTypeDef bsp_eeprom_read(uint8_t mem_addr, uint8_t *buf, uint16_t len);
HAL_StatusTypeDef bsp_eeprom_write(uint8_t mem_addr, const uint8_t *buf, uint16_t len);

#endif /* __BSP_EEPROM_H */
