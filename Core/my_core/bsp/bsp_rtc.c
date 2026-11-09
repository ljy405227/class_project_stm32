#include "bsp_rtc.h"
#include "config.h"

#if CFG_RTC_EN

#include "rtc.h"
#include "debug.h"
#include <stdio.h>
#include <string.h>

#define RTC_MAGIC   0x32F3U

static uint8_t month_from_name(const char *name)
{
    static const char *const months[12] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    uint8_t i;

    for (i = 0U; i < 12U; i++)
    {
        if (strncmp(name, months[i], 3U) == 0)
        {
            return (uint8_t)(i + 1U);
        }
    }
    return 1U;
}

void bsp_rtc_init(void)
{
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};
    char month_name[4] = "Jan";
    int day = 1;
    int year = 2026;
    int hour = 0;
    int minute = 0;
    int second = 0;

    if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) == RTC_MAGIC)
    {
        return;
    }

    (void)sscanf(__DATE__, "%3s %d %d", month_name, &day, &year);
    (void)sscanf(__TIME__, "%d:%d:%d", &hour, &minute, &second);

    time.Hours = (uint8_t)hour;
    time.Minutes = (uint8_t)minute;
    time.Seconds = (uint8_t)second;
    time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    time.StoreOperation = RTC_STOREOPERATION_RESET;

    date.WeekDay = RTC_WEEKDAY_MONDAY;
    date.Month = month_from_name(month_name);
    date.Date = (uint8_t)day;
    date.Year = (uint8_t)(year - 2000);

    if ((HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN) != HAL_OK) ||
        (HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK))
    {
        DEBUG_ERROR("RTC set time failed\r\n");
    }
    else
    {
        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, RTC_MAGIC);
    }
}

void bsp_rtc_get(uint8_t *hours, uint8_t *minutes, uint8_t *seconds,
                 uint16_t *year, uint8_t *month, uint8_t *day)
{
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};

    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

    if (hours != NULL)
    {
        *hours = time.Hours;
    }
    if (minutes != NULL)
    {
        *minutes = time.Minutes;
    }
    if (seconds != NULL)
    {
        *seconds = time.Seconds;
    }
    if (year != NULL)
    {
        *year = (uint16_t)(2000U + date.Year);
    }
    if (month != NULL)
    {
        *month = date.Month;
    }
    if (day != NULL)
    {
        *day = date.Date;
    }
}

#endif /* CFG_RTC_EN */
