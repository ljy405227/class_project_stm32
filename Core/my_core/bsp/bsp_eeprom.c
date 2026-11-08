#include "bsp_eeprom.h"
#include "config.h"
#include "i2c.h"
#include "drv_at24c02.h"

#if CFG_EEPROM_EN

#define AT24C02_ADDR7   0x50U

static drv_at24c02_t s_eeprom;

void bsp_eeprom_init(void)
{
    drv_at24c02_init(&s_eeprom, &hi2c1, AT24C02_ADDR7, 100U);
}

HAL_StatusTypeDef bsp_eeprom_read(uint8_t mem_addr, uint8_t *buf, uint16_t len)
{
    return drv_at24c02_read(&s_eeprom, mem_addr, buf, len);
}

HAL_StatusTypeDef bsp_eeprom_write(uint8_t mem_addr, const uint8_t *buf, uint16_t len)
{
    return drv_at24c02_write(&s_eeprom, mem_addr, buf, len);
}

#endif /* CFG_EEPROM_EN */
