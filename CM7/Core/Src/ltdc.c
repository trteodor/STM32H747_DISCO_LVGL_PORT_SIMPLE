/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ltdc.c
  * @brief   This file provides code for the configuration
  *          of the LTDC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "ltdc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

LTDC_HandleTypeDef hltdc;

/* LTDC init function */
void MX_LTDC_Initt(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AH;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AH;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 0;
  hltdc.Init.VerticalSync = 3;
  hltdc.Init.AccumulatedHBP = 2;
  hltdc.Init.AccumulatedVBP = 5;
  hltdc.Init.AccumulatedActiveW = 802;
  hltdc.Init.AccumulatedActiveH = 485;
  hltdc.Init.TotalWidth = 803;
  hltdc.Init.TotalHeigh = 487;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 20;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 800;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 480;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB888;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0xD0000000;
  pLayerCfg.ImageWidth = 800;
  pLayerCfg.ImageHeight = 480;
  pLayerCfg.Backcolor.Blue = 20;
  pLayerCfg.Backcolor.Green = 40;
  pLayerCfg.Backcolor.Red = 60;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */

  /* USER CODE END LTDC_Init 2 */

}

//void HAL_LTDC_MspInit(LTDC_HandleTypeDef* ltdcHandle)
//{
//
//  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
//  if(ltdcHandle->Instance==LTDC)
//  {
//  /* USER CODE BEGIN LTDC_MspInit 0 */
//
//  /* USER CODE END LTDC_MspInit 0 */
//
//  /** Initializes the peripherals clock
//  */
//    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
//    PeriphClkInitStruct.PLL3.PLL3M = 5;
//    PeriphClkInitStruct.PLL3.PLL3N = 160;
//    PeriphClkInitStruct.PLL3.PLL3P = 2;
//    PeriphClkInitStruct.PLL3.PLL3Q = 2;
//    PeriphClkInitStruct.PLL3.PLL3R = 21;
//    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
//    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
//    PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* LTDC clock enable */
//    __HAL_RCC_LTDC_CLK_ENABLE();
//
//    /* LTDC interrupt Init */
//    HAL_NVIC_SetPriority(LTDC_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(LTDC_IRQn);
//  /* USER CODE BEGIN LTDC_MspInit 1 */
//
//  /* USER CODE END LTDC_MspInit 1 */
//  }
//}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* ltdcHandle)
{

  if(ltdcHandle->Instance==LTDC)
  {
  /* USER CODE BEGIN LTDC_MspDeInit 0 */

  /* USER CODE END LTDC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LTDC_CLK_DISABLE();

    /* LTDC interrupt Deinit */
    HAL_NVIC_DisableIRQ(LTDC_IRQn);
  /* USER CODE BEGIN LTDC_MspDeInit 1 */

  /* USER CODE END LTDC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
