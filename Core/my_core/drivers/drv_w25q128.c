#include "drv_w25q128.h"

#define W25Q_CMD_WRITE_ENABLE   0x06U
#define W25Q_CMD_READ_STATUS1   0x05U
#define W25Q_CMD_READ_DATA      0x03U
#define W25Q_CMD_PAGE_PROGRAM   0x02U
#define W25Q_CMD_SECTOR_ERASE   0x20U
#define W25Q_CMD_JEDEC_ID       0x9FU

static void w25q_cs_low(drv_w25q128_t *dev)
{
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
}

static void w25q_cs_high(drv_w25q128_t *dev)
{
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

void drv_w25q128_init(drv_w25q128_t *dev, SPI_HandleTypeDef *hspi,
                      GPIO_TypeDef *cs_port, uint16_t cs_pin, uint32_t timeout)
{
    if (dev == NULL)
    {
        return;
    }
    dev->hspi = hspi;
    dev->cs_port = cs_port;
    dev->cs_pin = cs_pin;
    dev->timeout = timeout;
    w25q_cs_high(dev);
}

int drv_w25q128_wait_busy(drv_w25q128_t *dev)
{
    uint8_t cmd = W25Q_CMD_READ_STATUS1;
    uint8_t status = 0xFFU;
    uint32_t tick = HAL_GetTick();

    w25q_cs_low(dev);
    if (HAL_SPI_Transmit(dev->hspi, &cmd, 1, dev->timeout) != HAL_OK)
    {
        w25q_cs_high(dev);
        return -1;
    }
    do
    {
        if (HAL_SPI_Receive(dev->hspi, &status, 1, dev->timeout) != HAL_OK)
        {
            w25q_cs_high(dev);
            return -1;
        }
        if ((HAL_GetTick() - tick) > 1000U)
        {
            w25q_cs_high(dev);
            return -2;
        }
    } while ((status & 0x01U) != 0U);
    w25q_cs_high(dev);
    return 0;
}

static int w25q_write_enable(drv_w25q128_t *dev)
{
    uint8_t cmd = W25Q_CMD_WRITE_ENABLE;

    w25q_cs_low(dev);
    if (HAL_SPI_Transmit(dev->hspi, &cmd, 1, dev->timeout) != HAL_OK)
    {
        w25q_cs_high(dev);
        return -1;
    }
    w25q_cs_high(dev);
    return 0;
}

uint32_t drv_w25q128_read_id(drv_w25q128_t *dev)
{
    uint8_t cmd = W25Q_CMD_JEDEC_ID;
    uint8_t id[3] = {0xFFU, 0xFFU, 0xFFU};

    w25q_cs_low(dev);
    if (HAL_SPI_Transmit(dev->hspi, &cmd, 1, dev->timeout) != HAL_OK)
    {
        w25q_cs_high(dev);
        return 0xFFFFFFU;
    }
    if (HAL_SPI_Receive(dev->hspi, id, 3, dev->timeout) != HAL_OK)
    {
        w25q_cs_high(dev);
        return 0xFFFFFFU;
    }
    w25q_cs_high(dev);

    return ((uint32_t)id[0] << 16) | ((uint32_t)id[1] << 8) | id[2];
}

int drv_w25q128_read(drv_w25q128_t *dev, uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint8_t cmd[4];

    if ((dev == NULL) || (buf == NULL) || (len == 0U))
    {
        return -1;
    }
    cmd[0] = W25Q_CMD_READ_DATA;
    cmd[1] = (uint8_t)(addr >> 16);
    cmd[2] = (uint8_t)(addr >> 8);
    cmd[3] = (uint8_t)addr;

    w25q_cs_low(dev);
    if (HAL_SPI_Transmit(dev->hspi, cmd, 4, dev->timeout) != HAL_OK)
    {
        w25q_cs_high(dev);
        return -1;
    }
    if (HAL_SPI_Receive(dev->hspi, buf, (uint16_t)len, dev->timeout) != HAL_OK)
    {
        w25q_cs_high(dev);
        return -1;
    }
    w25q_cs_high(dev);
    return 0;
}

int drv_w25q128_page_write(drv_w25q128_t *dev, uint32_t addr, const uint8_t *buf, uint32_t len)
{
    uint8_t cmd[4];

    if ((dev == NULL) || (buf == NULL) || (len == 0U) || (len > 256U))
    {
        return -1;
    }
    if (w25q_write_enable(dev) != 0)
    {
        return -1;
    }

    cmd[0] = W25Q_CMD_PAGE_PROGRAM;
    cmd[1] = (uint8_t)(addr >> 16);
    cmd[2] = (uint8_t)(addr >> 8);
    cmd[3] = (uint8_t)addr;

    w25q_cs_low(dev);
    if (HAL_SPI_Transmit(dev->hspi, cmd, 4, dev->timeout) != HAL_OK)
    {
        w25q_cs_high(dev);
        return -1;
    }
    if (HAL_SPI_Transmit(dev->hspi, (uint8_t *)buf, (uint16_t)len, dev->timeout) != HAL_OK)
    {
        w25q_cs_high(dev);
        return -1;
    }
    w25q_cs_high(dev);

    return drv_w25q128_wait_busy(dev);
}

int drv_w25q128_sector_erase(drv_w25q128_t *dev, uint32_t addr)
{
    uint8_t cmd[4];

    if (w25q_write_enable(dev) != 0)
    {
        return -1;
    }

    cmd[0] = W25Q_CMD_SECTOR_ERASE;
    cmd[1] = (uint8_t)(addr >> 16);
    cmd[2] = (uint8_t)(addr >> 8);
    cmd[3] = (uint8_t)addr;

    w25q_cs_low(dev);
    if (HAL_SPI_Transmit(dev->hspi, cmd, 4, dev->timeout) != HAL_OK)
    {
        w25q_cs_high(dev);
        return -1;
    }
    w25q_cs_high(dev);

    return drv_w25q128_wait_busy(dev);
}
