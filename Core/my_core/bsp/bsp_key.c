#include "bsp_key.h"
#include "config.h"

#if CFG_KEY_EN

#define KEY_DEBOUNCE_TICKS 2U

static GPIO_TypeDef *const key_port[KEY_COUNT] = {
    KEY1_GPIO_Port, KEY2_GPIO_Port, KEY3_GPIO_Port, KEY4_GPIO_Port
};

static const uint16_t key_pin[KEY_COUNT] = {
    KEY1_Pin, KEY2_Pin, KEY3_Pin, KEY4_Pin
};

static GPIO_TypeDef *const row_port[3] = {
    KEY_ROW1_GPIO_Port, KEY_ROW2_GPIO_Port, KEY_ROW3_GPIO_Port
};

static const uint16_t row_pin[3] = {
    KEY_ROW1_Pin, KEY_ROW2_Pin, KEY_ROW3_Pin
};

static key_mode_t s_mode = KEY_MODE_INDEPENDENT;
static uint8_t s_state[KEY_COUNT];
static uint8_t s_debounce[KEY_COUNT];
static uint16_t s_press_ticks[KEY_COUNT];
static uint8_t s_long_sent[KEY_COUNT];
static key_event_t s_event[KEY_COUNT];
static uint8_t s_exti_flag[KEY_COUNT];
static uint8_t s_matrix_last;
static uint8_t s_matrix_cnt;
static uint8_t s_matrix_key;

static void key_rows_all_high(void)
{
    uint8_t i;
    for (i = 0U; i < 3U; i++)
    {
        HAL_GPIO_WritePin(row_port[i], row_pin[i], GPIO_PIN_SET);
    }
}

void key_exti_enable(uint8_t en)
{
    uint8_t i;
    for (i = 0U; i < KEY_COUNT; i++)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(key_pin[i]);
    }

    if (en != 0U)
    {
        HAL_NVIC_EnableIRQ(KEY1_EXTI_IRQn);
        HAL_NVIC_EnableIRQ(KEY2_EXTI_IRQn);
        HAL_NVIC_EnableIRQ(KEY3_EXTI_IRQn);
        HAL_NVIC_EnableIRQ(KEY4_EXTI_IRQn);
    }
    else
    {
        HAL_NVIC_DisableIRQ(KEY1_EXTI_IRQn);
        HAL_NVIC_DisableIRQ(KEY2_EXTI_IRQn);
        HAL_NVIC_DisableIRQ(KEY3_EXTI_IRQn);
        HAL_NVIC_DisableIRQ(KEY4_EXTI_IRQn);
    }
}

void key_set_mode(key_mode_t mode)
{
    if ((mode == KEY_MODE_MATRIX) && (CFG_KEY_MATRIX_EN == 0))
    {
        mode = KEY_MODE_INDEPENDENT;
    }

    s_mode = mode;
    key_rows_all_high();

    if (mode == KEY_MODE_INDEPENDENT)
    {
        HAL_GPIO_WritePin(KEY_ROW1_GPIO_Port, KEY_ROW1_Pin, GPIO_PIN_RESET);
        key_exti_enable(CFG_KEY_EXTI_EN);
    }
    else
    {
        key_exti_enable(0U);
    }
}

key_mode_t key_get_mode(void)
{
    return s_mode;
}

void key_init(void)
{
    uint8_t i;
    for (i = 0U; i < KEY_COUNT; i++)
    {
        s_state[i] = 0U;
        s_debounce[i] = 0U;
        s_press_ticks[i] = 0U;
        s_long_sent[i] = 0U;
        s_event[i] = KEY_EVENT_NONE;
        s_exti_flag[i] = 0U;
    }
    s_matrix_last = 0U;
    s_matrix_cnt = 0U;
    s_matrix_key = 0U;
    key_set_mode(KEY_MODE_INDEPENDENT);
}

void key_scan(void)
{
    uint8_t i;

    if (s_mode == KEY_MODE_MATRIX)
    {
#if CFG_KEY_MATRIX_EN
        (void)key_matrix_scan();
#endif
        return;
    }

    for (i = 0U; i < KEY_COUNT; i++)
    {
        uint8_t pressed = (HAL_GPIO_ReadPin(key_port[i], key_pin[i]) == GPIO_PIN_RESET) ? 1U : 0U;

        if (pressed != 0U)
        {
            if (s_state[i] == 0U)
            {
                s_state[i] = 1U;
                s_debounce[i] = 0U;
            }
            else if (s_state[i] == 1U)
            {
                if (++s_debounce[i] >= KEY_DEBOUNCE_TICKS)
                {
                    s_state[i] = 2U;
                    s_press_ticks[i] = 0U;
                    s_long_sent[i] = 0U;
                }
            }
            else
            {
                if (s_press_ticks[i] < 0xFFFFU)
                {
                    s_press_ticks[i]++;
                }
                if ((s_long_sent[i] == 0U) && (s_press_ticks[i] >= KEY_LONG_TICKS))
                {
                    if (s_event[i] == KEY_EVENT_NONE)
                    {
                        s_event[i] = KEY_EVENT_LONG;
                    }
                    s_long_sent[i] = 1U;
                }
            }
        }
        else
        {
            if (s_state[i] == 1U)
            {
                s_state[i] = 0U;
                s_debounce[i] = 0U;
            }
            else if (s_state[i] == 2U)
            {
                if ((s_long_sent[i] == 0U) && (s_event[i] == KEY_EVENT_NONE))
                {
                    s_event[i] = KEY_EVENT_SHORT;
                }
                s_state[i] = 0U;
                s_press_ticks[i] = 0U;
            }
        }
    }
}

key_event_t key_get_event(uint8_t idx)
{
    if (idx >= KEY_COUNT)
    {
        return KEY_EVENT_NONE;
    }
    return s_event[idx];
}

void key_clear_event(uint8_t idx)
{
    if (idx < KEY_COUNT)
    {
        s_event[idx] = KEY_EVENT_NONE;
    }
}

bool is_key_down(uint8_t idx)
{
    if (idx >= KEY_COUNT)
    {
        return false;
    }
    return (s_state[idx] == 2U);
}

void key_res(void)
{
    uint8_t i;
    for (i = 0U; i < KEY_COUNT; i++)
    {
        s_event[i] = KEY_EVENT_NONE;
        s_state[i] = 0U;
        s_debounce[i] = 0U;
        s_press_ticks[i] = 0U;
        s_long_sent[i] = 0U;
    }
    s_matrix_last = 0U;
    s_matrix_cnt = 0U;
    s_matrix_key = 0U;
}

#if CFG_KEY_MATRIX_EN
uint8_t key_matrix_scan(void)
{
    uint8_t row;
    uint8_t col;
    uint8_t key = 0U;

    for (row = 0U; row < 3U; row++)
    {
        uint8_t r;
        for (r = 0U; r < 3U; r++)
        {
            HAL_GPIO_WritePin(row_port[r], row_pin[r], (r == row) ? GPIO_PIN_RESET : GPIO_PIN_SET);
        }
        for (col = 0U; col < KEY_COUNT; col++)
        {
            if (HAL_GPIO_ReadPin(key_port[col], key_pin[col]) == GPIO_PIN_RESET)
            {
                key = (uint8_t)(row * KEY_COUNT + col + 1U);
                break;
            }
        }
        if (key != 0U)
        {
            break;
        }
    }
    key_rows_all_high();

    if (key == s_matrix_last)
    {
        if (s_matrix_cnt < 0xFFU)
        {
            s_matrix_cnt++;
        }
    }
    else
    {
        s_matrix_last = key;
        s_matrix_cnt = 0U;
    }

    if (s_matrix_cnt >= KEY_DEBOUNCE_TICKS)
    {
        s_matrix_key = key;
    }
    else if (key == 0U)
    {
        s_matrix_key = 0U;
    }

    return s_matrix_key;
}
#else
uint8_t key_matrix_scan(void)
{
    return 0U;
}
#endif

uint8_t key_exti_take(uint8_t idx)
{
    uint8_t v;
    if (idx >= KEY_COUNT)
    {
        return 0U;
    }
    v = s_exti_flag[idx];
    s_exti_flag[idx] = 0U;
    return v;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint8_t i;
    for (i = 0U; i < KEY_COUNT; i++)
    {
        if (GPIO_Pin == key_pin[i])
        {
            s_exti_flag[i] = 1U;
            break;
        }
    }
}

#endif /* CFG_KEY_EN */
