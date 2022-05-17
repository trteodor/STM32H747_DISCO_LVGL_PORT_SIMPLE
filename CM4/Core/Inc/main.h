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
#include "stm32h7xx_hal.h"

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
#define HDMI_CEC_Pin GPIO_PIN_6
#define HDMI_CEC_GPIO_Port GPIOB
#define OSC32_OUT_Pin GPIO_PIN_15
#define OSC32_OUT_GPIO_Port GPIOC
#define OSC32_IN_Pin GPIO_PIN_14
#define OSC32_IN_GPIO_Port GPIOC
#define SPDIF_RX0_Pin GPIO_PIN_7
#define SPDIF_RX0_GPIO_Port GPIOD
#define SAI1_SCK_A_Pin GPIO_PIN_5
#define SAI1_SCK_A_GPIO_Port GPIOE
#define SAI1_FS_A_Pin GPIO_PIN_4
#define SAI1_FS_A_GPIO_Port GPIOE
#define SAI1_SD_B_Pin GPIO_PIN_3
#define SAI1_SD_B_GPIO_Port GPIOE
#define SAI1_SD_A_Pin GPIO_PIN_6
#define SAI1_SD_A_GPIO_Port GPIOE
#define CEC_CK_MCO1_Pin GPIO_PIN_8
#define CEC_CK_MCO1_GPIO_Port GPIOA
#define SAI1_MCLK_A_Pin GPIO_PIN_7
#define SAI1_MCLK_A_GPIO_Port GPIOG
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOH
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOH
#define PMOD_14_ARD_D3_Pin GPIO_PIN_8
#define PMOD_14_ARD_D3_GPIO_Port GPIOF
#define ARD_A1_Pin GPIO_PIN_10
#define ARD_A1_GPIO_Port GPIOF
#define ARD_D6_Pin GPIO_PIN_7
#define ARD_D6_GPIO_Port GPIOJ
#define ARD_D9_Pin GPIO_PIN_6
#define ARD_D9_GPIO_Port GPIOJ
#define ARD_A4_Pin GPIO_PIN_2
#define ARD_A4_GPIO_Port GPIOC
#define ARD_A5_Pin GPIO_PIN_3
#define ARD_A5_GPIO_Port GPIOC
#define ARD_A2_Pin GPIO_PIN_0
#define ARD_A2_GPIO_Port GPIOA
#define ARD_A3_Pin GPIO_PIN_1
#define ARD_A3_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
