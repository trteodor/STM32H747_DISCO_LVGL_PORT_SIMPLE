/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dsihost.c
  * @brief   This file provides code for the configuration
  *          of the DSIHOST instances.
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
#include "dsihost.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

DSI_HandleTypeDef hdsi;

/* DSIHOST init function */

void MX_DSIHOST_DSI_Init(void)
{

  /* USER CODE BEGIN DSIHOST_Init 0 */
	  DSI_PLLInitTypeDef PLLInit;
	  DSI_VidCfgTypeDef VidCfg;

	  hdsi.Instance = DSI;
	  hdsi.Init.AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;
	  hdsi.Init.TXEscapeCkdiv = 4;
	  hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
	  PLLInit.PLLNDIV = 100;
	  PLLInit.PLLIDF = DSI_PLL_IN_DIV5;
	  PLLInit.PLLODF = DSI_PLL_OUT_DIV1;
	  if (HAL_DSI_Init(&hdsi, &PLLInit) != HAL_OK)
	  {
	    return HAL_ERROR;
	  }

	  /* Timing parameters for all Video modes */
	  /*
	  The lane byte clock is set 62500 Khz
	  The pixel clock is set to 27429 Khz
	  */
	  VidCfg.VirtualChannelID = 0;
	  VidCfg.ColorCoding = LCD_PIXEL_FORMAT_RGB888;
	  VidCfg.LooselyPacked = DSI_LOOSELY_PACKED_DISABLE;
	  VidCfg.Mode = DSI_VID_MODE_BURST;
	  VidCfg.PacketSize = LCD_DEFAULT_WIDTH;
	  VidCfg.NumberOfChunks = 0;
	  VidCfg.NullPacketSize = 0xFFFU;
	  VidCfg.HSPolarity = DSI_HSYNC_ACTIVE_HIGH;
	  VidCfg.VSPolarity = DSI_VSYNC_ACTIVE_HIGH;
	  VidCfg.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;
	  VidCfg.HorizontalSyncActive = (OTM8009A_480X800_HSYNC * 62500U)/27429U;
	  VidCfg.HorizontalBackPorch = (OTM8009A_480X800_HBP * 62500U)/27429U;
	  VidCfg.HorizontalLine = ((LCD_DEFAULT_WIDTH + OTM8009A_480X800_HSYNC + OTM8009A_480X800_HBP + OTM8009A_480X800_HFP) * 62500U)/27429U;
	  VidCfg.VerticalSyncActive = OTM8009A_480X800_VSYNC;
	  VidCfg.VerticalBackPorch = OTM8009A_480X800_VBP;
	  VidCfg.VerticalFrontPorch = OTM8009A_480X800_VFP;
	  VidCfg.VerticalActive = LCD_DEFAULT_HEIGHT;
	  VidCfg.LPCommandEnable = DSI_LP_COMMAND_ENABLE;
	  VidCfg.LPLargestPacketSize = 4;
	  VidCfg.LPVACTLargestPacketSize = 4;

	  VidCfg.LPHorizontalFrontPorchEnable  = DSI_LP_HFP_ENABLE;
	  VidCfg.LPHorizontalBackPorchEnable   = DSI_LP_HBP_ENABLE;
	  VidCfg.LPVerticalActiveEnable        = DSI_LP_VACT_ENABLE;
	  VidCfg.LPVerticalFrontPorchEnable    = DSI_LP_VFP_ENABLE;
	  VidCfg.LPVerticalBackPorchEnable     = DSI_LP_VBP_ENABLE;
	  VidCfg.LPVerticalSyncActiveEnable    = DSI_LP_VSYNC_ENABLE;
	  VidCfg.FrameBTAAcknowledgeEnable     = DSI_FBTAA_DISABLE;

	  HAL_DSI_ConfigVideoMode(&hdsi, &VidCfg);


#ifdef AAA
  /* USER CODE END DSIHOST_Init 0 */

  DSI_PLLInitTypeDef PLLInit = {0};
  DSI_HOST_TimeoutTypeDef HostTimeouts = {0};
  DSI_PHY_TimerTypeDef PhyTimings = {0};
  DSI_LPCmdTypeDef LPCmd = {0};
  DSI_CmdCfgTypeDef CmdCfg = {0};

  /* USER CODE BEGIN DSIHOST_Init 1 */

  /* USER CODE END DSIHOST_Init 1 */
  hdsi.Instance = DSI;
  hdsi.Init.AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;
  hdsi.Init.TXEscapeCkdiv = 4;
  hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  PLLInit.PLLNDIV = 99;
  PLLInit.PLLIDF = DSI_PLL_IN_DIV5;
  PLLInit.PLLODF = DSI_PLL_OUT_DIV1;
  if (HAL_DSI_Init(&hdsi, &PLLInit) != HAL_OK)
  {
    Error_Handler();
  }
  HostTimeouts.TimeoutCkdiv = 1;
  HostTimeouts.HighSpeedTransmissionTimeout = 10000;
  HostTimeouts.LowPowerReceptionTimeout = 5000;
  HostTimeouts.HighSpeedReadTimeout = 0;
  HostTimeouts.LowPowerReadTimeout = 0;
  HostTimeouts.HighSpeedWriteTimeout = 0;
  HostTimeouts.HighSpeedWritePrespMode = DSI_HS_PM_DISABLE;
  HostTimeouts.LowPowerWriteTimeout = 0;
  HostTimeouts.BTATimeout = 0;
  if (HAL_DSI_ConfigHostTimeouts(&hdsi, &HostTimeouts) != HAL_OK)
  {
    Error_Handler();
  }
  PhyTimings.ClockLaneHS2LPTime = 28;
  PhyTimings.ClockLaneLP2HSTime = 33;
  PhyTimings.DataLaneHS2LPTime = 15;
  PhyTimings.DataLaneLP2HSTime = 25;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 0;
  if (HAL_DSI_ConfigPhyTimer(&hdsi, &PhyTimings) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_ConfigFlowControl(&hdsi, DSI_FLOW_CONTROL_BTA) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetSlewRateAndDelayTuning(&hdsi, DSI_SLEW_RATE_HSTX, DSI_CLOCK_LANE, 4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetSlewRateAndDelayTuning(&hdsi, DSI_HS_DELAY, DSI_CLOCK_LANE, 4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetSlewRateAndDelayTuning(&hdsi, DSI_SLEW_RATE_HSTX, DSI_DATA_LANES, 4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetSlewRateAndDelayTuning(&hdsi, DSI_HS_DELAY, DSI_DATA_LANES, 4) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetLowPowerRXFilter(&hdsi, 0) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_ConfigErrorMonitor(&hdsi, HAL_DSI_ERROR_TX|HAL_DSI_ERROR_RX) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetSDD(&hdsi, ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  LPCmd.LPGenShortWriteNoP = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite = DSI_LP_DLW_DISABLE;
  LPCmd.LPMaxReadPacket = DSI_LP_MRDP_DISABLE;
  LPCmd.AcknowledgeRequest = DSI_ACKNOWLEDGE_DISABLE;
  if (HAL_DSI_ConfigCommand(&hdsi, &LPCmd) != HAL_OK)
  {
    Error_Handler();
  }
  CmdCfg.VirtualChannelID = 0;
  CmdCfg.ColorCoding = DSI_RGB888;
  CmdCfg.CommandSize = 800;
  CmdCfg.TearingEffectSource = DSI_TE_EXTERNAL;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.HSPolarity = DSI_HSYNC_ACTIVE_LOW;
  CmdCfg.VSPolarity = DSI_VSYNC_ACTIVE_LOW;
  CmdCfg.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.VSyncPol = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh = DSI_AR_ENABLE;
  CmdCfg.TEAcknowledgeRequest = DSI_TE_ACKNOWLEDGE_DISABLE;
  if (HAL_DSI_ConfigAdaptedCommandMode(&hdsi, &CmdCfg) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetGenericVCID(&hdsi, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DSIHOST_Init 2 */
#endif
  /* USER CODE END DSIHOST_Init 2 */

}

void HAL_DSI_MspInit(DSI_HandleTypeDef* dsiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(dsiHandle->Instance==DSI)
  {
  /* USER CODE BEGIN DSI_MspInit 0 */

  /* USER CODE END DSI_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_DSI;
    PeriphClkInitStruct.DsiClockSelection = RCC_DSICLKSOURCE_PHY;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* DSI clock enable */
    __HAL_RCC_DSI_CLK_ENABLE();

    __HAL_RCC_GPIOJ_CLK_ENABLE();
    /**DSIHOST GPIO Configuration
    DSI_D1P     ------> DSIHOST_D1P
    DSI_D1N     ------> DSIHOST_D1N
    DSI_CKP     ------> DSIHOST_CKP
    DSI_CKN     ------> DSIHOST_CKN
    DSI_D0P     ------> DSIHOST_D0P
    DSI_D0N     ------> DSIHOST_D0N
    PJ2     ------> DSIHOST_TE
    */
    GPIO_InitStruct.Pin = DSI_TE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DSI;
    HAL_GPIO_Init(DSI_TE_GPIO_Port, &GPIO_InitStruct);

    /* DSI interrupt Init */
    HAL_NVIC_SetPriority(DSI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DSI_IRQn);
  /* USER CODE BEGIN DSI_MspInit 1 */

  /* USER CODE END DSI_MspInit 1 */
  }
}

void HAL_DSI_MspDeInit(DSI_HandleTypeDef* dsiHandle)
{

  if(dsiHandle->Instance==DSI)
  {
  /* USER CODE BEGIN DSI_MspDeInit 0 */

  /* USER CODE END DSI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_DSI_CLK_DISABLE();

    /**DSIHOST GPIO Configuration
    DSI_D1P     ------> DSIHOST_D1P
    DSI_D1N     ------> DSIHOST_D1N
    DSI_CKP     ------> DSIHOST_CKP
    DSI_CKN     ------> DSIHOST_CKN
    DSI_D0P     ------> DSIHOST_D0P
    DSI_D0N     ------> DSIHOST_D0N
    PJ2     ------> DSIHOST_TE
    */
    HAL_GPIO_DeInit(DSI_TE_GPIO_Port, DSI_TE_Pin);

    /* DSI interrupt Deinit */
    HAL_NVIC_DisableIRQ(DSI_IRQn);
  /* USER CODE BEGIN DSI_MspDeInit 1 */

  /* USER CODE END DSI_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
