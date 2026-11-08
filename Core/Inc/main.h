/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY3_Pin GPIO_PIN_2
#define KEY3_GPIO_Port GPIOE
#define KEY3_EXTI_IRQn EXTI2_IRQn
#define KEY4_Pin GPIO_PIN_3
#define KEY4_GPIO_Port GPIOE
#define KEY4_EXTI_IRQn EXTI3_IRQn
#define KEY_ROW1_Pin GPIO_PIN_4
#define KEY_ROW1_GPIO_Port GPIOE
#define KEY_ROW2_Pin GPIO_PIN_5
#define KEY_ROW2_GPIO_Port GPIOE
#define KEY_ROW3_Pin GPIO_PIN_6
#define KEY_ROW3_GPIO_Port GPIOE
#define DHT11_DATA_Pin GPIO_PIN_13
#define DHT11_DATA_GPIO_Port GPIOC
#define LED_F0_Pin GPIO_PIN_0
#define LED_F0_GPIO_Port GPIOF
#define LED_F1_Pin GPIO_PIN_1
#define LED_F1_GPIO_Port GPIOF
#define LED_F2_Pin GPIO_PIN_2
#define LED_F2_GPIO_Port GPIOF
#define LED_F3_Pin GPIO_PIN_3
#define LED_F3_GPIO_Port GPIOF
#define LED_F4_Pin GPIO_PIN_4
#define LED_F4_GPIO_Port GPIOF
#define LED_F5_Pin GPIO_PIN_5
#define LED_F5_GPIO_Port GPIOF
#define LED_F6_Pin GPIO_PIN_6
#define LED_F6_GPIO_Port GPIOF
#define LED_F7_Pin GPIO_PIN_7
#define LED_F7_GPIO_Port GPIOF
#define PH0_OSC_IN_Pin GPIO_PIN_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define ADC_IN0_Pin GPIO_PIN_0
#define ADC_IN0_GPIO_Port GPIOA
#define ADC_IN1_Pin GPIO_PIN_1
#define ADC_IN1_GPIO_Port GPIOA
#define ADC_IN2_Pin GPIO_PIN_2
#define ADC_IN2_GPIO_Port GPIOA
#define ADC_IN3_Pin GPIO_PIN_3
#define ADC_IN3_GPIO_Port GPIOA
#define ADC_IN4_Pin GPIO_PIN_4
#define ADC_IN4_GPIO_Port GPIOA
#define PWM_IN_Pin GPIO_PIN_6
#define PWM_IN_GPIO_Port GPIOA
#define PWM_OUT_Pin GPIO_PIN_7
#define PWM_OUT_GPIO_Port GPIOA
#define W25Q128_CS_Pin GPIO_PIN_14
#define W25Q128_CS_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOC
#define USART1_TX_Pin GPIO_PIN_9
#define USART1_TX_GPIO_Port GPIOA
#define USART1_RX_Pin GPIO_PIN_10
#define USART1_RX_GPIO_Port GPIOA
#define SPI1_SCK_Pin GPIO_PIN_3
#define SPI1_SCK_GPIO_Port GPIOB
#define SPI1_MISO_Pin GPIO_PIN_4
#define SPI1_MISO_GPIO_Port GPIOB
#define SPI1_MOSI_Pin GPIO_PIN_5
#define SPI1_MOSI_GPIO_Port GPIOB
#define I2C1_SCL_Pin GPIO_PIN_8
#define I2C1_SCL_GPIO_Port GPIOB
#define I2C1_SDA_Pin GPIO_PIN_9
#define I2C1_SDA_GPIO_Port GPIOB
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOE
#define KEY1_EXTI_IRQn EXTI0_IRQn
#define KEY2_Pin GPIO_PIN_1
#define KEY2_GPIO_Port GPIOE
#define KEY2_EXTI_IRQn EXTI1_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
