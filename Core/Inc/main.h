/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

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
#define LCD_RS_Pin GPIO_PIN_0
#define LCD_RS_GPIO_Port GPIOA
#define LCD_E_Pin GPIO_PIN_1
#define LCD_E_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define OUT1_Pin GPIO_PIN_3
#define OUT1_GPIO_Port GPIOA
#define LCD_D4_Pin GPIO_PIN_4
#define LCD_D4_GPIO_Port GPIOA
#define LCD_D5_Pin GPIO_PIN_5
#define LCD_D5_GPIO_Port GPIOA
#define LCD_D6_Pin GPIO_PIN_6
#define LCD_D6_GPIO_Port GPIOA
#define LCD_D7_Pin GPIO_PIN_7
#define LCD_D7_GPIO_Port GPIOA
#define BUTTON_Push_Pin GPIO_PIN_0
#define BUTTON_Push_GPIO_Port GPIOB
#define BUTTON_Push_EXTI_IRQn EXTI0_1_IRQn
#define OUT2_Pin GPIO_PIN_8
#define OUT2_GPIO_Port GPIOA
#define OUT3_Pin GPIO_PIN_9
#define OUT3_GPIO_Port GPIOA
#define OUT4_Pin GPIO_PIN_10
#define OUT4_GPIO_Port GPIOA
#define OUT5_Pin GPIO_PIN_11
#define OUT5_GPIO_Port GPIOA
#define OUT6_Pin GPIO_PIN_12
#define OUT6_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOB
#define OUT7_Pin GPIO_PIN_6
#define OUT7_GPIO_Port GPIOB
#define VCP_RX_Pin GPIO_PIN_7
#define VCP_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
