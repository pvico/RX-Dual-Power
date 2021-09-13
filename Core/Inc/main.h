/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l0xx_hal.h"

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
void SystemClock_Config();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW1_Pin GPIO_PIN_9
#define SW1_GPIO_Port GPIOB
#define CTL1_Pin GPIO_PIN_14
#define CTL1_GPIO_Port GPIOC
#define CTL2_Pin GPIO_PIN_15
#define CTL2_GPIO_Port GPIOC
#define MAGNET_Pin GPIO_PIN_0
#define MAGNET_GPIO_Port GPIOA
#define MAGNET_EXTI_IRQn EXTI0_1_IRQn
#define V_MAIN_Pin GPIO_PIN_1
#define V_MAIN_GPIO_Port GPIOA
#define V_STBY_Pin GPIO_PIN_4
#define V_STBY_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOA
#define S_PORT_Pin GPIO_PIN_9
#define S_PORT_GPIO_Port GPIOA
#define STAT_STBY_Pin GPIO_PIN_10
#define STAT_STBY_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_14
#define SW2_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
