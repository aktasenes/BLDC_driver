/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Buzzer_Pin GPIO_PIN_0
#define Buzzer_GPIO_Port GPIOA
#define I_in_Pin GPIO_PIN_4
#define I_in_GPIO_Port GPIOA
#define V_in_Pin GPIO_PIN_5
#define V_in_GPIO_Port GPIOA
#define PWM__Pin GPIO_PIN_0
#define PWM__GPIO_Port GPIOB
#define PWM_B1_Pin GPIO_PIN_1
#define PWM_B1_GPIO_Port GPIOB
#define TX_Pin GPIO_PIN_10
#define TX_GPIO_Port GPIOB
#define RX_Pin GPIO_PIN_11
#define RX_GPIO_Port GPIOB
#define L1_Pin GPIO_PIN_10
#define L1_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define L2_Pin GPIO_PIN_6
#define L2_GPIO_Port GPIOB
#define H2_Pin GPIO_PIN_7
#define H2_GPIO_Port GPIOB
#define L3_Pin GPIO_PIN_8
#define L3_GPIO_Port GPIOB
#define H3_Pin GPIO_PIN_9
#define H3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
