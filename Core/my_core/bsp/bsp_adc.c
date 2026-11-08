#include "bsp_adc.h"
#include "config.h"
#include "adc.h"
#include "debug.h"

#if CFG_ADC_EN

static uint32_t s_adc_dma_buf[BSP_ADC_CHANNELS];

void bsp_adc_init(void)
{
    if (HAL_ADC_Start_DMA(&hadc1, s_adc_dma_buf, BSP_ADC_CHANNELS) != HAL_OK)
    {
        DEBUG_ERROR("ADC DMA start failed\r\n");
    }
}

uint16_t bsp_adc_get(uint8_t ch)
{
    if (ch >= BSP_ADC_CHANNELS)
    {
        return 0U;
    }
    return ((uint16_t *)s_adc_dma_buf)[ch];
}

float bsp_adc_get_voltage(uint8_t ch)
{
    return (float)bsp_adc_get(ch) * 3.3f / 4095.0f;
}

#endif /* CFG_ADC_EN */
