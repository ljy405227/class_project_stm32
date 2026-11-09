#ifndef __DRV_FONT8X16_H
#define __DRV_FONT8X16_H

#include <stdint.h>

#define DRV_FONT8X16_FIRST   32U
#define DRV_FONT8X16_LAST    126U
#define DRV_FONT8X16_WIDTH   8U
#define DRV_FONT8X16_HEIGHT  16U

extern const uint8_t g_drv_font8x16[DRV_FONT8X16_LAST - DRV_FONT8X16_FIRST + 1U][DRV_FONT8X16_HEIGHT];

#endif /* __DRV_FONT8X16_H */
