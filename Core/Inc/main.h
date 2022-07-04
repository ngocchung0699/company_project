/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define LED_DO_Pin GPIO_PIN_1
#define LED_DO_GPIO_Port GPIOC
#define LED_XANH_Pin GPIO_PIN_3
#define LED_XANH_GPIO_Port GPIOC
#define RESET_SIM_Pin GPIO_PIN_5
#define RESET_SIM_GPIO_Port GPIOC
#define PWRKEY_Pin GPIO_PIN_0
#define PWRKEY_GPIO_Port GPIOB
#define USIM_DET_Pin GPIO_PIN_1
#define USIM_DET_GPIO_Port GPIOB
#define SIM_DTR_Pin GPIO_PIN_13
#define SIM_DTR_GPIO_Port GPIOB
#define SIM_RST_Pin GPIO_PIN_14
#define SIM_RST_GPIO_Port GPIOB
#define OF_SIM_Pin GPIO_PIN_3
#define OF_SIM_GPIO_Port GPIOB
#define CE_BQ_Pin GPIO_PIN_4
#define CE_BQ_GPIO_Port GPIOB
#define CHG_Pin GPIO_PIN_5
#define CHG_GPIO_Port GPIOB
#define PGOOD_Pin GPIO_PIN_8
#define PGOOD_GPIO_Port GPIOB
#define Jack_PW_Pin GPIO_PIN_9
#define Jack_PW_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
