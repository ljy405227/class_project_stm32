#include "drv_at24c02.h"
#include "my_tool.h"

#define AT24C02_PAGE_SIZE       8U
#define AT24C02_WRITE_DELAY_MS  5U

void drv_at24c02_init(drv_at24c02_t *dev, I2C_HandleTypeDef *hi2c, uint8_t addr7, uint32_t timeout)
{
    if (dev == NULL)
    {
        return;
    }
    dev->hi2c = hi2c;
    dev->addr7 = addr7;
    dev->timeout = timeout;
}

HAL_StatusTypeDef drv_at24c02_read(drv_at24c02_t *dev, uint8_t mem_addr, uint8_t *buf, uint16_t len)
{
    if ((dev == NULL) || (dev->hi2c == NULL) || (buf == NULL) || (len == 0U))
    {
        return HAL_ERROR;
    }
    return HAL_I2C_Mem_Read(dev->hi2c, (uint16_t)(dev->addr7 << 1), mem_addr,
                            I2C_MEMADD_SIZE_8BIT, buf, len, dev->timeout);
}

HAL_StatusTypeDef drv_at24c02_write(drv_at24c02_t *dev, uint8_t mem_addr, const uint8_t *buf, uint16_t len)
{
    uint16_t written = 0U;

    if ((dev == NULL) || (dev->hi2c == NULL) || (buf == NULL) || (len == 0U))
    {
        return HAL_ERROR;
    }

    while (written < len)
    {
        uint16_t page_left = (uint16_t)(AT24C02_PAGE_SIZE - ((mem_addr + written) % AT24C02_PAGE_SIZE));
        uint16_t chunk = (uint16_t)((len - written) < page_left ? (len - written) : page_left);
        HAL_StatusTypeDef st = HAL_I2C_Mem_Write(dev->hi2c, (uint16_t)(dev->addr7 << 1),
                                                 (uint16_t)(mem_addr + written), I2C_MEMADD_SIZE_8BIT,
                                                 (uint8_t *)(buf + written), chunk, dev->timeout);
        if (st != HAL_OK)
        {
            return st;
        }
        my_delay_ms(AT24C02_WRITE_DELAY_MS);
        written = (uint16_t)(written + chunk);
    }

    return HAL_OK;
}
