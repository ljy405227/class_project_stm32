#include "bsp_shell.h"
#include "config.h"
#include "main.h"
#include "usart.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_beep.h"
#include "bsp_adc.h"
#include "bsp_dht11.h"
#include "bsp_eeprom.h"
#include "bsp_spiflash.h"
#include "bsp_pwm_out.h"
#include "bsp_pwm_in.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if CFG_SHELL_EN

#define SHELL_RX_SIZE    128U
#define SHELL_LINE_SIZE  80U

static uint8_t s_rx_byte;
static volatile uint8_t s_rx_buf[SHELL_RX_SIZE];
static volatile uint16_t s_rx_head;
static volatile uint16_t s_rx_tail;

static char s_line[SHELL_LINE_SIZE];
static uint8_t s_line_len;
static uint8_t s_esc;
static uint8_t s_last_cr;

typedef void (*shell_fn_t)(int argc, char **argv);

typedef struct
{
    const char *name;
    const char *help;
    shell_fn_t fn;
} shell_cmd_t;

static void shell_puts(const char *s)
{
    printf("%s", s);
}

static void shell_prompt(void)
{
    printf("\r\n> ");
}

static int shell_pop(uint8_t *b)
{
    if (s_rx_head == s_rx_tail)
    {
        return 0;
    }
    *b = s_rx_buf[s_rx_tail];
    s_rx_tail = (uint16_t)((s_rx_tail + 1U) % SHELL_RX_SIZE);
    return 1;
}

static void shell_push(uint8_t b)
{
    uint16_t next = (uint16_t)((s_rx_head + 1U) % SHELL_RX_SIZE);
    if (next != s_rx_tail)
    {
        s_rx_buf[s_rx_head] = b;
        s_rx_head = next;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        shell_push(s_rx_byte);
        (void)HAL_UART_Receive_IT(&huart1, &s_rx_byte, 1U);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        (void)HAL_UART_Receive_IT(&huart1, &s_rx_byte, 1U);
    }
}

static void cmd_help(int argc, char **argv);

#if CFG_LED_EN
static void cmd_led(int argc, char **argv)
{
    uint8_t idx;
    uint8_t i;

    if (argc == 1)
    {
        for (i = 0U; i < 8U; i++)
        {
            printf("LED%u=%s ", (unsigned)i, bsp_led_get(i) ? "on" : "off");
        }
        printf("\r\n");
        return;
    }
    if (argc != 3)
    {
        shell_puts("usage: led <0-7> <on|off|toggle>\r\n");
        return;
    }
    idx = (uint8_t)strtoul(argv[1], NULL, 0);
    if (idx > 7U)
    {
        shell_puts("led index must be 0-7\r\n");
        return;
    }
    if (strcmp(argv[2], "on") == 0)
    {
        bsp_led_on(idx);
    }
    else if (strcmp(argv[2], "off") == 0)
    {
        bsp_led_off(idx);
    }
    else if (strcmp(argv[2], "toggle") == 0)
    {
        bsp_led_toggle(idx);
    }
    else
    {
        shell_puts("usage: led <0-7> <on|off|toggle>\r\n");
        return;
    }
    printf("LED%u %s\r\n", (unsigned)idx, argv[2]);
}
#else
static void cmd_led(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    shell_puts("LED disabled (CFG_LED_EN=0)\r\n");
}
#endif

#if CFG_BEEP_EN
static void cmd_beep(int argc, char **argv)
{
    uint32_t ms;

    if (argc != 2)
    {
        shell_puts("usage: beep <ms>\r\n");
        return;
    }
    ms = (uint32_t)strtoul(argv[1], NULL, 0);
    if (ms == 0U)
    {
        ms = 1U;
    }
    if (ms > 5000U)
    {
        ms = 5000U;
    }
    bsp_beep_ms(ms);
    printf("beep %lu ms\r\n", (unsigned long)ms);
}
#else
static void cmd_beep(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    shell_puts("BEEP disabled (CFG_BEEP_EN=0)\r\n");
}
#endif

#if CFG_KEY_EN
static void cmd_key(int argc, char **argv)
{
    uint8_t i;
    (void)argc;
    (void)argv;

    for (i = 0U; i < KEY_COUNT; i++)
    {
        printf("KEY%u=%u ", (unsigned)(i + 1U), is_key_down(i) ? 1U : 0U);
    }
    printf("\r\n");
}
#else
static void cmd_key(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    shell_puts("KEY disabled (CFG_KEY_EN=0)\r\n");
}
#endif

#if CFG_ADC_EN
static void cmd_adc(int argc, char **argv)
{
    uint8_t ch;
    uint8_t i;
    uint32_t mv;

    if (argc == 2)
    {
        ch = (uint8_t)strtoul(argv[1], NULL, 0);
        if (ch >= BSP_ADC_CHANNELS)
        {
            shell_puts("adc channel must be 0-4\r\n");
            return;
        }
        mv = (uint32_t)(bsp_adc_get_voltage(ch) * 1000.0f);
        printf("ADC%u: %u  %lu mV\r\n", (unsigned)ch, bsp_adc_get(ch), (unsigned long)mv);
        return;
    }
    if (argc != 1)
    {
        shell_puts("usage: adc [0-4]\r\n");
        return;
    }
    for (i = 0U; i < BSP_ADC_CHANNELS; i++)
    {
        mv = (uint32_t)(bsp_adc_get_voltage(i) * 1000.0f);
        printf("ADC%u: %u  %lu mV\r\n", (unsigned)i, bsp_adc_get(i), (unsigned long)mv);
    }
}
#else
static void cmd_adc(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    shell_puts("ADC disabled (CFG_ADC_EN=0)\r\n");
}
#endif

#if CFG_DHT11_EN
static void cmd_dht11(int argc, char **argv)
{
    uint8_t temp = 0U;
    uint8_t hum = 0U;
    int ret;

    (void)argc;
    (void)argv;
    ret = bsp_dht11_read(&temp, &hum);
    if (ret == 0)
    {
        printf("DHT11: %u%%RH  %u C\r\n", (unsigned)hum, (unsigned)temp);
    }
    else
    {
        printf("DHT11 read failed: %d\r\n", ret);
    }
}
#else
static void cmd_dht11(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    shell_puts("DHT11 disabled (CFG_DHT11_EN=0)\r\n");
}
#endif

#if CFG_SPIFLASH_EN
static void cmd_flash(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    printf("SPI Flash JEDEC ID: 0x%06X (expect 0xEF4018)\r\n", bsp_spiflash_read_id());
}
#else
static void cmd_flash(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    shell_puts("SPI Flash disabled (CFG_SPIFLASH_EN=0)\r\n");
}
#endif

#if CFG_EEPROM_EN
static void cmd_eeprom(int argc, char **argv)
{
    uint8_t addr;
    uint8_t len;
    uint8_t buf[16];
    uint8_t i;

    if (argc != 3)
    {
        shell_puts("usage: eeprom <addr 0-255> <len 1-16>\r\n");
        return;
    }
    addr = (uint8_t)strtoul(argv[1], NULL, 0);
    len = (uint8_t)strtoul(argv[2], NULL, 0);
    if ((len == 0U) || (len > 16U))
    {
        shell_puts("len must be 1-16\r\n");
        return;
    }
    if (bsp_eeprom_read(addr, buf, len) != HAL_OK)
    {
        shell_puts("EEPROM read failed\r\n");
        return;
    }
    printf("EEPROM[%u..%u]:", (unsigned)addr, (unsigned)(addr + len - 1U));
    for (i = 0U; i < len; i++)
    {
        printf(" %02X", buf[i]);
    }
    printf("\r\n");
}
#else
static void cmd_eeprom(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    shell_puts("EEPROM disabled (CFG_EEPROM_EN=0)\r\n");
}
#endif

#if CFG_PWM_OUT_EN
static void cmd_pwm(int argc, char **argv)
{
    uint32_t freq;
    uint32_t duty;

    if (argc != 3)
    {
        shell_puts("usage: pwm <freq_hz> <duty 0-100>\r\n");
        return;
    }
    freq = (uint32_t)strtoul(argv[1], NULL, 0);
    duty = (uint32_t)strtoul(argv[2], NULL, 0);
    if (duty > 100U)
    {
        duty = 100U;
    }
    bsp_pwm_out_set(freq, (float)duty);
    printf("PWM out: %lu Hz, %lu%%\r\n", (unsigned long)freq, (unsigned long)duty);
}
#else
static void cmd_pwm(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    shell_puts("PWM output disabled (CFG_PWM_OUT_EN=0)\r\n");
}
#endif

#if CFG_PWM_IN_EN
static void cmd_pwmread(int argc, char **argv)
{
    uint32_t freq = 0U;
    float duty = 0.0f;
    uint32_t duty_x10;

    (void)argc;
    (void)argv;
    if (bsp_pwm_in_get(&freq, &duty) != 0)
    {
        shell_puts("PWM input: no signal\r\n");
        return;
    }
    duty_x10 = (uint32_t)(duty * 10.0f);
    printf("PWM in: %lu Hz, %lu.%lu%%\r\n",
           (unsigned long)freq, (unsigned long)(duty_x10 / 10U), (unsigned long)(duty_x10 % 10U));
}
#else
static void cmd_pwmread(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    shell_puts("PWM input disabled (CFG_PWM_IN_EN=0)\r\n");
}
#endif

static void cmd_free(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    printf("FreeRTOS free heap: %u bytes, tasks: %u\r\n",
           (unsigned)xPortGetFreeHeapSize(), (unsigned)uxTaskGetNumberOfTasks());
}

static void cmd_uptime(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    printf("Uptime: %lu s\r\n", (unsigned long)(HAL_GetTick() / 1000U));
}

static const char *shell_task_state(eTaskState state)
{
    switch (state)
    {
        case eRunning:   return "RUN";
        case eReady:     return "READY";
        case eBlocked:   return "BLOCK";
        case eSuspended: return "SUSP";
        case eDeleted:   return "DEL";
        default:         return "?";
    }
}

static void cmd_tasks(int argc, char **argv)
{
    TaskStatus_t status[16];
    UBaseType_t count;
    UBaseType_t i;

    (void)argc;
    (void)argv;

    count = uxTaskGetSystemState(status, 16U, NULL);
    printf("tasks: %u, free heap: %u B\r\n",
           (unsigned)count, (unsigned)xPortGetFreeHeapSize());
    printf("%-16s %-6s %-5s %s\r\n", "Name", "State", "Prio", "StackFree");
    for (i = 0U; (i < count) && (i < 16U); i++)
    {
        printf("%-16s %-6s %-5u %u B\r\n",
               status[i].pcTaskName,
               shell_task_state(status[i].eCurrentState),
               (unsigned)status[i].uxCurrentPriority,
               (unsigned)(status[i].usStackHighWaterMark * sizeof(StackType_t)));
    }
}

static const shell_cmd_t s_cmds[] = {
    {"help",    "list commands",              cmd_help},
    {"?",       "list commands",              cmd_help},
    {"led",     "led <0-7> on|off|toggle",    cmd_led},
    {"beep",    "beep <ms>",                  cmd_beep},
    {"key",     "show key states",            cmd_key},
    {"adc",     "adc [0-4]",                  cmd_adc},
    {"dht11",   "read DHT11",                 cmd_dht11},
    {"flash",   "read SPI Flash JEDEC ID",    cmd_flash},
    {"eeprom",  "eeprom <addr> <len>",        cmd_eeprom},
    {"pwm",     "pwm <freq_hz> <duty>",       cmd_pwm},
    {"pwmread", "read TIM3 PWM input",        cmd_pwmread},
    {"free",    "show FreeRTOS heap",         cmd_free},
    {"tasks",   "show task name/state/stack", cmd_tasks},
    {"ps",      "show task name/state/stack", cmd_tasks},
    {"uptime",  "show uptime",                cmd_uptime},
};

static void cmd_help(int argc, char **argv)
{
    uint32_t i;
    (void)argc;
    (void)argv;

    shell_puts("commands:\r\n");
    for (i = 0U; i < (sizeof(s_cmds) / sizeof(s_cmds[0])); i++)
    {
        printf("  %-8s %s\r\n", s_cmds[i].name, s_cmds[i].help);
    }
}

static void shell_dispatch(char *line)
{
    char *argv[6];
    int argc = 0;
    char *p;
    uint32_t i;

    p = strtok(line, " \t");
    while ((p != NULL) && (argc < 6))
    {
        argv[argc++] = p;
        p = strtok(NULL, " \t");
    }
    if (argc == 0)
    {
        return;
    }

    for (i = 0U; i < (sizeof(s_cmds) / sizeof(s_cmds[0])); i++)
    {
        if (strcmp(argv[0], s_cmds[i].name) == 0)
        {
            s_cmds[i].fn(argc, argv);
            return;
        }
    }
    printf("unknown command: %s\r\n", argv[0]);
}

static void shell_handle_char(uint8_t ch)
{
    if (s_esc != 0U)
    {
        s_esc++;
        if (s_esc >= 3U)
        {
            s_esc = 0U;
        }
        return;
    }
    if (ch == 0x1BU)
    {
        s_esc = 1U;
        return;
    }

    if ((ch == '\r') || (ch == '\n'))
    {
        if ((ch == '\n') && (s_last_cr != 0U))
        {
            s_last_cr = 0U;
            return;
        }
        s_last_cr = (ch == '\r') ? 1U : 0U;
        s_line[s_line_len] = '\0';
        printf("\r\n");
        if (s_line_len > 0U)
        {
            shell_dispatch(s_line);
        }
        s_line_len = 0U;
        shell_prompt();
        return;
    }

    s_last_cr = 0U;

    if ((ch == 8U) || (ch == 127U))
    {
        if (s_line_len > 0U)
        {
            s_line_len--;
            shell_puts("\b \b");
        }
        return;
    }

    if ((ch >= 32U) && (ch < 127U) && (s_line_len < (SHELL_LINE_SIZE - 1U)))
    {
        s_line[s_line_len++] = (char)ch;
        printf("%c", ch);
    }
}

void shell_poll(void)
{
    uint8_t b;
    while (shell_pop(&b) != 0)
    {
        shell_handle_char(b);
    }
}

void shell_init(void)
{
    s_rx_head = 0U;
    s_rx_tail = 0U;
    s_line_len = 0U;
    s_esc = 0U;
    s_last_cr = 0U;

    (void)HAL_UART_Receive_IT(&huart1, &s_rx_byte, 1U);
    shell_puts("\r\nshell ready. type 'help'\r\n");
    shell_prompt();
}

#endif /* CFG_SHELL_EN */
