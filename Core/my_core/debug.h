#ifndef __DEBUG_H
#define __DEBUG_H

#include <stdio.h>
#include "config.h"


#if CFG_DEBUG_LOG_EN
#define DEBUG_LOG(...)      printf(__VA_ARGS__)
#define DEBUG_WARN(...)     do { printf("[WARN] %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); } while (0)
#define DEBUG_ERROR(...)    do { printf("[ERROR] %s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); } while (0)
#else
#define DEBUG_LOG(...)      ((void)0)
#define DEBUG_WARN(...)     ((void)0)
#define DEBUG_ERROR(...)    ((void)0)
#endif

#define PRINT_STARTUP_LOG() \
    do { \
        DEBUG_LOG("\r\n"); \
        DEBUG_LOG("==============================================\r\n"); \
        DEBUG_LOG("startup...\r\n"); \
        DEBUG_LOG("----------------------------------------------\r\n"); \
        DEBUG_LOG("  Project : calss_project_stm32\r\n"); \
        DEBUG_LOG("  Board   : STM32F407ZGT6\r\n"); \
        DEBUG_LOG("  System  : freertos\r\n"); \
        DEBUG_LOG("  Version : V1.0   [Maintenance]\r\n"); \
        DEBUG_LOG("  Author  : ª∂¿÷∂π <2703867620@qq.com>\r\n"); \
        DEBUG_LOG("  git     : https://github.com/ljy405227/class_project_stm32.git\r\n"); \
        DEBUG_LOG("  Repo    : class_stm32/calss_project_stm32\r\n"); \
        DEBUG_LOG("  Build   : %s %s\r\n", __DATE__, __TIME__); \
        DEBUG_LOG("==============================================\r\n"); \
        DEBUG_LOG("\r\n"); \
    } while (0)

#endif /* __DEBUG_H */
