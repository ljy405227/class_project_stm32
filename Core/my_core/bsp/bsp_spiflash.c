#include "bsp_spiflash.h"
#include "config.h"
#include "spi.h"
#include "drv_w25q128.h"

#if CFG_SPIFLASH_EN

static drv_w25q128_t s_flash;

void bsp_spiflash_init(void)
{
    drv_w25q128_init(&s_flash, &hspi1, W25Q128_CS_GPIO_Port, W25Q128_CS_Pin, 100U);
}

uint32_t bsp_spiflash_read_id(void)
{
    return drv_w25q128_read_id(&s_flash);
}

int bsp_spiflash_read(uint32_t addr, uint8_t *buf, uint32_t len)
{
    return drv_w25q128_read(&s_flash, addr, buf, len);
}

int bsp_spiflash_page_write(uint32_t addr, const uint8_t *buf, uint32_t len)
{
    return drv_w25q128_page_write(&s_flash, addr, buf, len);
}

int bsp_spiflash_sector_erase(uint32_t addr)
{
    return drv_w25q128_sector_erase(&s_flash, addr);
}

#endif /* CFG_SPIFLASH_EN */
