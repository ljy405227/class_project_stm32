#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * 功能总开关：
 *   1 = 应用层启用该模块；0 = 应用层不初始化/不调用该模块。
 *   CubeMX 生成的 MX_xxx_Init() 始终执行，关闭模块不会跳过外设初始化。
 */
#define CFG_LED_EN              1
#define CFG_KEY_EN              1
#define CFG_KEY_MATRIX_EN       1
#define CFG_KEY_EXTI_EN         1
#define CFG_BEEP_EN             1
#define CFG_EEPROM_EN           0
#define CFG_SPIFLASH_EN         0
#define CFG_DHT11_EN            0
#define CFG_ADC_EN              0
#define CFG_PWM_IN_EN           0
#define CFG_PWM_OUT_EN          0
#define CFG_SHELL_EN            1
#define CFG_TIM6_EN             1
#define CFG_SEG_EN              1

/* 调试打印 */
#define CFG_DEBUG_LOG_EN        1

/* 板级参数 */
#define BEEP_ACTIVE_LOW         1       /* 原理图 Q9 为 PNP，PC8 低电平鸣叫 */
#define KEY_SCAN_PERIOD_MS      10
#define KEY_LONG_TICKS          150     /* 150 * 10ms = 1.5s 长按 */

#endif /* __CONFIG_H */
