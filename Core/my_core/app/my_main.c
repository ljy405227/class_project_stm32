#include "my_main.h"
#include "config.h"
#include "debug.h"
#include "my_tool.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "tim.h"

#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_beep.h"
#include "bsp_eeprom.h"
#include "bsp_spiflash.h"
#include "bsp_dht11.h"
#include "bsp_adc.h"
#include "bsp_pwm_in.h"
#include "bsp_pwm_out.h"
#include "bsp_shell.h"

static void app_init(void)
{
    my_dwt_init();
    PRINT_STARTUP_LOG();

#if CFG_LED_EN
    bsp_led_init();
#endif
#if CFG_KEY_EN
    key_init();
#endif
#if CFG_BEEP_EN
    bsp_beep_init();
#endif
#if CFG_EEPROM_EN
    bsp_eeprom_init();
#endif
#if CFG_SPIFLASH_EN
    bsp_spiflash_init();
#endif
#if CFG_DHT11_EN
    bsp_dht11_init();
#endif
#if CFG_ADC_EN
    bsp_adc_init();
#endif
#if CFG_PWM_IN_EN
    bsp_pwm_in_init();
#endif
#if CFG_PWM_OUT_EN
    bsp_pwm_out_init();
#endif
}

void my_main(void)
{
    app_init();

#if CFG_SPIFLASH_EN
    DEBUG_LOG("SPI Flash JEDEC ID: 0x%06X (expect 0xEF4018)\r\n", bsp_spiflash_read_id());
#endif
#if CFG_EEPROM_EN
    {
        uint8_t first = 0xFFU;
        if (bsp_eeprom_read(0U, &first, 1U) == HAL_OK)
        {
            DEBUG_LOG("EEPROM[0] = 0x%02X\r\n", first);
        }
        else
        {
            DEBUG_WARN("EEPROM read failed\r\n");
        }
    }
#endif

    DEBUG_LOG("%s: %d===task starting...\r\n", __func__, __LINE__);

#if CFG_SHELL_EN
    shell_init();
#endif
}

void app_task_key(void *argument)
{
    (void)argument;

#if CFG_KEY_EN
    for (;;)
    {
        key_scan();
        osDelay(KEY_SCAN_PERIOD_MS);
    }
#else
    osThreadExit();
#endif
}

void app_task_control(void *argument)
{
    (void)argument;

#if CFG_KEY_EN
    for (;;)
    {
        uint8_t k;
        for (k = 0U; k < KEY_COUNT; k++)
        {
            key_event_t ev = key_get_event(k);
            if (ev == KEY_EVENT_SHORT)
            {
                DEBUG_LOG("KEY%u short\r\n", (unsigned)(k + 1U));
#if CFG_BEEP_EN
                bsp_beep_ms(30U);
#endif
                key_clear_event(k);
            }
            else if (ev == KEY_EVENT_LONG)
            {
                DEBUG_LOG("KEY%u long\r\n", (unsigned)(k + 1U));
#if CFG_BEEP_EN
                bsp_beep_ms(100U);
#endif
                key_clear_event(k);
            }
        }
        osDelay(KEY_SCAN_PERIOD_MS);
    }
#else
    osThreadExit();
#endif
}

void app_task_shell(void *argument)
{
    (void)argument;

#if CFG_SHELL_EN
    for (;;)
    {
        shell_poll();
        osDelay(10U);
    }
#else
    osThreadExit();
#endif
}


static SemaphoreHandle_t s_timer_sem;

void app_timer_irq_handler(void)
{
    BaseType_t woken = pdFALSE;

    if (s_timer_sem != NULL)
    {
        // 释放信号量s_timer_sem
        (void)xSemaphoreGiveFromISR(s_timer_sem, &woken);
        // 触发一次上下文切换
        portYIELD_FROM_ISR(woken);
    }
}

void app_task_timer(void *argument)
{
    uint32_t count = 0U;

    (void)argument;

    s_timer_sem = xSemaphoreCreateBinary();
    if (s_timer_sem == NULL)
    {
        DEBUG_ERROR("%s: %d===TIM6 semaphore create failed\r\n", __func__, __LINE__);
        osThreadExit();
    }
    if (HAL_TIM_Base_Start_IT(&htim6) != HAL_OK)
    {
        DEBUG_ERROR("%s: %d===TIM6 start failed\r\n", __func__, __LINE__);
        osThreadExit();
    }

    for (;;)
    {
        if (xSemaphoreTake(s_timer_sem, portMAX_DELAY) == pdTRUE)
        {
            count++;
            // DEBUG_LOG("TIM6 IRQ: %d s\r\n", count);
        }
    }
}

