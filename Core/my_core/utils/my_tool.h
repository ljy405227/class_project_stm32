#ifndef __MY_TOOL_H
#define __MY_TOOL_H

#include "main.h"
#include <stdint.h>

float my_constrain(float val, float min, float max);

#define MY_FABSF(x) ((x) < 0.0f ? -(x) : (x))

void my_dwt_init(void);
void my_delay_us(uint32_t us);
void my_delay_ms(uint32_t ms);   /* 任务上下文调用，内部自动选择 osDelay/HAL_Delay */

#endif /* __MY_TOOL_H */
