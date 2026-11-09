#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#include <stdint.h>

void bsp_rtc_init(void);
void bsp_rtc_get(uint8_t *hours, uint8_t *minutes, uint8_t *seconds,
                 uint16_t *year, uint8_t *month, uint8_t *day);

#endif /* __BSP_RTC_H */
