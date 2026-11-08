#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "main.h"
#include <stdbool.h>
#include <stdint.h>

#define KEY_COUNT 4U

typedef enum
{
    KEY_MODE_INDEPENDENT = 0,
    KEY_MODE_MATRIX = 1
} key_mode_t;

typedef enum
{
    KEY_EVENT_NONE = 0,
    KEY_EVENT_SHORT,
    KEY_EVENT_LONG
} key_event_t;

void key_init(void);
void key_scan(void);
key_event_t key_get_event(uint8_t idx);
void key_clear_event(uint8_t idx);
bool is_key_down(uint8_t idx);
void key_res(void);
void key_set_mode(key_mode_t mode);
key_mode_t key_get_mode(void);
uint8_t key_matrix_scan(void);
void key_exti_enable(uint8_t en);
uint8_t key_exti_take(uint8_t idx);

#endif /* __BSP_KEY_H */
