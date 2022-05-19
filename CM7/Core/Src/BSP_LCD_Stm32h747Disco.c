
#include <BSP_LCD_Stm32h747Disco.h>


#include "main.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32H747I_DISCO
  * @{
  */

/** @defgroup STM32H747I_DISCO_LCD LCD
  * @{
  */
/** @defgroup STM32H747I_DISCO_LCD_Private_Variables Private Variables
  * @{
  */
static LCD_Drv_t                *Lcd_Drvv = NULL;
/**
  * @}
  */

/** @defgroup STM32H747I_DISCO_LCD_Private_TypesDefinitions Private TypesDefinitions
  * @{
  */
const LCD_UTILS_Drv_t LCD_Driverr =
{
  BSP_LCD_DrawBitmap,
  BSP_LCD_FillRGBRect,
  BSP_LCD_DrawHLine,
  BSP_LCD_DrawVLine,
  BSP_LCD_FillRect,
  BSP_LCD_ReadPixel,
  BSP_LCD_WritePixel,
  BSP_LCD_GetXSize,
  BSP_LCD_GetYSize,
  BSP_LCD_SetActiveLayer,
  BSP_LCD_GetPixelFormat
};

typedef struct
{
  uint32_t      HACT;
  uint32_t      VACT;
  uint32_t      HSYNC;
  uint32_t      HBP;
  uint32_t      HFP;
  uint32_t      VSYNC;
  uint32_t      VBP;
  uint32_t      VFP;

  /* Configure the D-PHY Timings */
  uint32_t      ClockLaneHS2LPTime;
  uint32_t      ClockLaneLP2HSTime;
  uint32_t      DataLaneHS2LPTime;
  uint32_t      DataLaneLP2HSTime;
  uint32_t      DataLaneMaxReadTime;
  uint32_t      StopWaitTime;
} LCD_HDMI_Timing_t;
/**
  * @}
  */

/** @defgroup STM32H747I_DISCO_LCD_Exported_Variables Exported Variables
  * @{
  */
void                *Lcd_CompObj = NULL;
DSI_HandleTypeDef   *hlcd_dsi;
DMA2D_HandleTypeDef hlcd_dma2d;
LTDC_HandleTypeDef  *hlcd_ltdc;
BSP_LCD_Ctx_t       Lcd_Ctxx[LCD_INSTANCES_NBR];
/**
  * @}
  */

/** @defgroup STM32H747I_DISCO_LCD_Private_FunctionPrototypes Private FunctionPrototypes
  * @{
  */

static int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);
static int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);


static int32_t OTM8009A_Probe(uint32_t ColorCoding, uint32_t Orientation);


static void DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d);
static void DMA2D_MspDeInit(DMA2D_HandleTypeDef *hdma2d);
static void LL_FillBuffer(uint32_t Instance, uint32_t *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t Color);
static void LL_ConvertLineToRGB(uint32_t Instance, uint32_t *pSrc, uint32_t *pDst, uint32_t xSize, uint32_t ColorMode);
static void LCD_InitSequence(void);
static void LCD_DeInitSequence(void);
/**
  * @}
  */
/** @defgroup STM32H747I_DISCO_LCD_Private_Macros Private Macros
  * @{
  */
#define CONVERTRGB5652ARGB8888(Color)((((((((Color) >> (11U)) & 0x1FU) * 527U) + 23U) >> (6U)) << (16U)) |\
                                     (((((((Color) >> (5U)) & 0x3FU) * 259U) + 33U) >> (6U)) << (8U)) |\
                                     (((((Color) & 0x1FU) * 527U) + 23U) >> (6U)) | (0xFF000000U))

/**
  * @}
  */
/** @defgroup STM32H747I_DISCO_LCD_Exported_Functions LCD Exported Functions
  * @{
  */
/**
  * @brief  Initializes the LCD in default mode.
  * @param  Instance    LCD Instance
  * @param  Orientation LCD_ORIENTATION_PORTRAIT or LCD_ORIENTATION_LANDSCAPE
  * @retval BSP status
  */
int32_t BSP_LCD_Init(uint32_t Instance, uint32_t Orientation)
{
  return BSP_LCD_InitEx(Instance, Orientation, LCD_PIXEL_FORMAT_RGB888, LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGHT);
}

/**
  * @brief  Initializes the LCD.
  * @param  Instance    LCD Instance
  * @param  Orientation LCD_ORIENTATION_PORTRAIT or LCD_ORIENTATION_LANDSCAPE
  * @param  PixelFormat LCD_PIXEL_FORMAT_RBG565 or LCD_PIXEL_FORMAT_RBG888
  * @param  Width       Display width
  * @param  Height      Display height
  * @retval BSP status
  */
int32_t BSP_LCD_InitEx(uint32_t Instance, uint32_t Orientation, uint32_t PixelFormat, uint32_t Width, uint32_t Height)
{
  int32_t ret = DRV_ERR_NONE;
  uint32_t ctrl_pixel_format, ltdc_pixel_format, dsi_pixel_format;
  MX_LTDC_LayerConfig_t config;

  if((Orientation > LCD_ORIENTATION_LANDSCAPE) || (Instance >= LCD_INSTANCES_NBR) || \
     ((PixelFormat != LCD_PIXEL_FORMAT_RGB565) && (PixelFormat != LTDC_PIXEL_FORMAT_RGB888)))
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if(PixelFormat == LCD_PIXEL_FORMAT_RGB565)
    {
      ltdc_pixel_format = LTDC_PIXEL_FORMAT_RGB565;
      dsi_pixel_format = DSI_RGB565;
      ctrl_pixel_format = OTM8009A_FORMAT_RBG565;
      Lcd_Ctxx[Instance].BppFactor = 2U;
    }
    else /* LCD_PIXEL_FORMAT_RGB888 */
    {
      ltdc_pixel_format = LTDC_PIXEL_FORMAT_ARGB8888;
      dsi_pixel_format = DSI_RGB888;
      ctrl_pixel_format = OTM8009A_FORMAT_RGB888;
      Lcd_Ctxx[Instance].BppFactor = 4U;
    }

    /* Store pixel format, xsize and ysize information */
    Lcd_Ctxx[Instance].PixelFormat = PixelFormat;
    Lcd_Ctxx[Instance].XSize  = Width;
    Lcd_Ctxx[Instance].YSize  = Height;

    /* Toggle Hardware Reset of the LCD using its XRES signal (active low) */
    BSP_LCD_Reset(Instance);


    /* Initialize LCD special pins GPIOs */
    LCD_InitSequence();

    DMA2D_MspInit(&hlcd_dma2d);

    MX_DSIHOST_DSI_Init();

    MX_LTDC_Init();



    if(ret == DRV_ERR_NONE)
    {
      /* Before configuring LTDC layer, ensure SDRAM is initialized */
#if !defined(DATA_IN_ExtSDRAM)
      /* Initialize the SDRAM */
      if(BSP_SDRAM_Init(0) != DRV_ERR_NONE)
      {
        return DRV_ERROR_PERIPH_FAILURE;
      }
#endif /* DATA_IN_ExtSDRAM */


        /* Enable the DSI host and wrapper after the LTDC initialization
        To avoid any synchronization issue, the DSI shall be started after enabling the LTDC */
        (void)HAL_DSI_Start(hlcd_dsi);

        /* Enable the DSI BTW for read operations */
        (void)HAL_DSI_ConfigFlowControl(hlcd_dsi, DSI_FLOW_CONTROL_BTA);


        if(OTM8009A_Probe(ctrl_pixel_format, Orientation) != DRV_ERR_NONE)
        {
          ret = DRV_ERROR_UNKNOWN_COMPONENT;
        }
        else
        {
          ret = DRV_ERR_NONE;
        }


    /* By default the reload is activated and executed immediately */
    Lcd_Ctxx[Instance].ReloadEnable = 1U;
   }
  }

  return ret;
}

/**
  * @brief  De-Initializes the LCD resources.
  * @param  Instance    LCD Instance
  * @retval BSP status
  */
int32_t BSP_LCD_DeInit(uint32_t Instance)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    LCD_DeInitSequence();
#if (USE_HAL_LTDC_REGISTER_CALLBACKS == 0)
    LTDC_MspDeInit(hlcd_ltdc);
#endif /* (USE_HAL_LTDC_REGISTER_CALLBACKS == 0) */

    DMA2D_MspDeInit(&hlcd_dma2d);

#if (USE_HAL_DSI_REGISTER_CALLBACKS == 0)
    DSI_MspDeInit(hlcd_dsi);
#endif /* (USE_HAL_DSI_REGISTER_CALLBACKS == 0) */
    if(HAL_DSI_DeInit(hlcd_dsi) != HAL_OK)
    {
      ret = DRV_ERROR_PERIPH_FAILURE;
    }
    else
    {
      (void)HAL_LTDC_DeInit(hlcd_ltdc);
      if(HAL_DMA2D_DeInit(&hlcd_dma2d) != HAL_OK)
      {
        ret = DRV_ERROR_PERIPH_FAILURE;
      }
      else
      {
        Lcd_Ctxx[Instance].IsMspCallbacksValid = 0;
      }
    }
  }

  return ret;
}

void BSP_LCD_Reset(uint32_t Instance)
{
  GPIO_InitTypeDef  gpio_init_structure;

  LCD_RESET_GPIO_CLK_ENABLE();

  /* Configure the GPIO Reset pin */
  gpio_init_structure.Pin   = LCD_RESET_PIN;
  gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull  = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LCD_RESET_GPIO_PORT , &gpio_init_structure);

  /* Activate XRES active low */
  HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT , LCD_RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(20);/* wait 20 ms */
  HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT , LCD_RESET_PIN, GPIO_PIN_SET);/* Deactivate XRES */
  HAL_Delay(10);/* Wait for 10ms after releasing XRES before sending commands */
}

/**
  * @brief  Configure LCD control pins (Back-light, Display Enable and TE)
  * @retval None
  */
static void LCD_InitSequence(void)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* LCD_BL_CTRL GPIO configuration */
  LCD_BL_CTRL_GPIO_CLK_ENABLE();

  gpio_init_structure.Pin       = LCD_BL_CTRL_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Pull      = GPIO_NOPULL;

  HAL_GPIO_Init(LCD_BL_CTRL_GPIO_PORT, &gpio_init_structure);
  /* Assert back-light LCD_BL_CTRL pin */
  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET);

  /* LCD_TE_CTRL GPIO configuration */
  LCD_TE_GPIO_CLK_ENABLE();

  gpio_init_structure.Pin       = LCD_TE_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_INPUT;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(LCD_TE_GPIO_PORT, &gpio_init_structure);
  /* Assert back-light LCD_BL_CTRL pin */
  HAL_GPIO_WritePin(LCD_TE_GPIO_PORT, LCD_TE_PIN, GPIO_PIN_SET);

      /** @brief NVIC configuration for LTDC interrupt that is now enabled */
  HAL_NVIC_SetPriority(LTDC_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(LTDC_IRQn);

  /** @brief NVIC configuration for DMA2D interrupt that is now enabled */
  HAL_NVIC_SetPriority(DMA2D_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);

  /** @brief NVIC configuration for DSI interrupt that is now enabled */
  HAL_NVIC_SetPriority(DSI_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(DSI_IRQn);
}

/**
  * @brief  DeInitializes LCD GPIO special pins MSP.
  * @retval None
  */
static void LCD_DeInitSequence(void)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* LCD_BL_CTRL GPIO configuration */
  /* LCD_BL_CTRL GPIO deactivation */
  gpio_init_structure.Pin       = LCD_BL_CTRL_PIN;
  HAL_GPIO_DeInit(LCD_BL_CTRL_GPIO_PORT, gpio_init_structure.Pin);

  /* LCD_TE_CTRL GPIO configuration */
}

/**
  * @brief  MX LTDC layer configuration.
  * @param  hltdc      LTDC handle
  * @param  LayerIndex Layer 0 or 1
  * @param  Config     Layer configuration
  * @retval HAL status
  */
__weak HAL_StatusTypeDef MX_LTDC_ConfigLayer(LTDC_HandleTypeDef *hltdc, uint32_t LayerIndex, MX_LTDC_LayerConfig_t *Config)
{
  LTDC_LayerCfgTypeDef pLayerCfg;

  pLayerCfg.WindowX0 = Config->X0;
  pLayerCfg.WindowX1 = Config->X1;
  pLayerCfg.WindowY0 = Config->Y0;
  pLayerCfg.WindowY1 = Config->Y1;
  pLayerCfg.PixelFormat = Config->PixelFormat;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = Config->Address;
  pLayerCfg.ImageWidth = (Config->X1 - Config->X0);
  pLayerCfg.ImageHeight = (Config->Y1 - Config->Y0);
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  return HAL_LTDC_ConfigLayer(hltdc, &pLayerCfg, LayerIndex);
}

/**
  * @brief  LTDC Clock Config for LCD DSI display.
  * @param  hltdc  LTDC Handle
  *         Being __weak it can be overwritten by the application
  * @retval HAL_status
  */
__weak HAL_StatusTypeDef MX_LTDC_ClockConfig(LTDC_HandleTypeDef *hltdc)
{
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  PeriphClkInitStruct.PeriphClockSelection   = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M      = 5U;
  PeriphClkInitStruct.PLL3.PLL3N      = 132U;
  PeriphClkInitStruct.PLL3.PLL3P      = 2U;
  PeriphClkInitStruct.PLL3.PLL3Q      = 2U;
  PeriphClkInitStruct.PLL3.PLL3R      = 24U;
  PeriphClkInitStruct.PLL3.PLL3RGE    = RCC_PLLCFGR_PLL3RGE_2;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN  = 0U;
  return HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

/**
  * @brief  LTDC Clock Config for LCD 2 DPI display.
  * @param  hltdc  LTDC Handle
  *         Being __weak it can be overwritten by the application
  * @retval HAL_status
  */
__weak HAL_StatusTypeDef MX_LTDC_ClockConfig2(LTDC_HandleTypeDef *hltdc)
{
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  PeriphClkInitStruct.PeriphClockSelection    = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M      = 1U;
  PeriphClkInitStruct.PLL3.PLL3N      = 13U;
  PeriphClkInitStruct.PLL3.PLL3P      = 2U;
  PeriphClkInitStruct.PLL3.PLL3Q      = 2U;
  PeriphClkInitStruct.PLL3.PLL3R      = 12U;
  PeriphClkInitStruct.PLL3.PLL3RGE    = RCC_PLLCFGR_PLL3RGE_2;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN  = 0U;
  return HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}

int32_t BSP_LCD_ConfigLayer(uint32_t Instance, uint32_t LayerIndex, BSP_LCD_LayerConfig_t *Config)
{
  int32_t ret = DRV_ERR_NONE;
  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if (MX_LTDC_ConfigLayer(hlcd_ltdc, LayerIndex, Config) != HAL_OK)
    {
      ret = DRV_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Set the LCD Active Layer.
  * @param  Instance    LCD Instance
  * @param  LayerIndex  LCD layer index
  * @retval BSP status
  */
int32_t BSP_LCD_SetActiveLayer(uint32_t Instance, uint32_t LayerIndex)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    Lcd_Ctxx[Instance].ActiveLayer = LayerIndex;
  }

  return ret;
}
/**
  * @brief  Gets the LCD Active LCD Pixel Format.
  * @param  Instance    LCD Instance
  * @param  PixelFormat Active LCD Pixel Format
  * @retval BSP status
  */
int32_t BSP_LCD_GetPixelFormat(uint32_t Instance, uint32_t *PixelFormat)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    /* Only RGB565 format is supported */
    *PixelFormat = Lcd_Ctxx[Instance].PixelFormat;
  }

  return ret;
}
/**
  * @brief  Control the LTDC reload
  * @param  Instance    LCD Instance
  * @param  ReloadType can be one of the following values
  *         - BSP_LCD_RELOAD_NONE
  *         - BSP_LCD_RELOAD_IMMEDIATE
  *         - BSP_LCD_RELOAD_VERTICAL_BLANKING
  * @retval BSP status
  */
int32_t BSP_LCD_Relaod(uint32_t Instance, uint32_t ReloadType)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else if(ReloadType == BSP_LCD_RELOAD_NONE)
  {
    Lcd_Ctxx[Instance].ReloadEnable = 0U;
  }
  else if(HAL_LTDC_Reload (hlcd_ltdc, ReloadType) != HAL_OK)
  {
    ret = DRV_ERROR_PERIPH_FAILURE;
  }
  else
  {
    Lcd_Ctxx[Instance].ReloadEnable = 1U;
  }

  return ret;
}

/**
  * @brief  Sets an LCD Layer visible
  * @param  Instance    LCD Instance
  * @param  LayerIndex  Visible Layer
  * @param  State  New state of the specified layer
  *          This parameter can be one of the following values:
  *            @arg  ENABLE
  *            @arg  DISABLE
  * @retval BSP status
  */
int32_t BSP_LCD_SetLayerVisible(uint32_t Instance, uint32_t LayerIndex, FunctionalState State)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if(State == ENABLE)
    {
      __HAL_LTDC_LAYER_ENABLE(hlcd_ltdc, LayerIndex);
    }
    else
    {
      __HAL_LTDC_LAYER_DISABLE(hlcd_ltdc, LayerIndex);
    }

    if(Lcd_Ctxx[Instance].ReloadEnable == 1U)
    {
      __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(hlcd_ltdc);
    }
  }

  return ret;
}

/**
  * @brief  Configures the transparency.
  * @param  Instance      LCD Instance
  * @param  LayerIndex    Layer foreground or background.
  * @param  Transparency  Transparency
  *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF
  * @retval BSP status
  */
int32_t BSP_LCD_SetTransparency(uint32_t Instance, uint32_t LayerIndex, uint8_t Transparency)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Lcd_Ctxx[Instance].ReloadEnable == 1U)
    {
      (void)HAL_LTDC_SetAlpha(hlcd_ltdc, Transparency, LayerIndex);
    }
    else
    {
      (void)HAL_LTDC_SetAlpha_NoReload(hlcd_ltdc, Transparency, LayerIndex);
    }
  }

  return ret;
}

/**
  * @brief  Sets an LCD layer frame buffer address.
  * @param  Instance    LCD Instance
  * @param  LayerIndex  Layer foreground or background
  * @param  Address     New LCD frame buffer value
  * @retval BSP status
  */
int32_t BSP_LCD_SetLayerAddress(uint32_t Instance, uint32_t LayerIndex, uint32_t Address)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Lcd_Ctxx[Instance].ReloadEnable == 1U)
    {
      (void)HAL_LTDC_SetAddress(hlcd_ltdc, Address, LayerIndex);
    }
    else
    {
      (void)HAL_LTDC_SetAddress_NoReload(hlcd_ltdc, Address, LayerIndex);
    }
  }

  return ret;
}

/**
  * @brief  Sets display window.
  * @param  Instance    LCD Instance
  * @param  LayerIndex  Layer index
  * @param  Xpos   LCD X position
  * @param  Ypos   LCD Y position
  * @param  Width  LCD window width
  * @param  Height LCD window height
  * @retval BSP status
  */
int32_t BSP_LCD_SetLayerWindow(uint32_t Instance, uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Lcd_Ctxx[Instance].ReloadEnable == 1U)
    {
      /* Reconfigure the layer size  and position */
      (void)HAL_LTDC_SetWindowSize(hlcd_ltdc, Width, Height, LayerIndex);
      (void)HAL_LTDC_SetWindowPosition(hlcd_ltdc, Xpos, Ypos, LayerIndex);
    }
    else
    {
      /* Reconfigure the layer size and position */
      (void)HAL_LTDC_SetWindowSize_NoReload(hlcd_ltdc, Width, Height, LayerIndex);
      (void)HAL_LTDC_SetWindowPosition_NoReload(hlcd_ltdc, Xpos, Ypos, LayerIndex);
    }

    Lcd_Ctxx[Instance].XSize = Width;
    Lcd_Ctxx[Instance].YSize = Height;
  }

  return ret;
}

/**
  * @brief  Configures and sets the color keying.
  * @param  Instance    LCD Instance
  * @param  LayerIndex  Layer foreground or background
  * @param  Color       Color reference
  * @retval BSP status
  */
int32_t BSP_LCD_SetColorKeying(uint32_t Instance, uint32_t LayerIndex, uint32_t Color)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Lcd_Ctxx[Instance].ReloadEnable == 1U)
    {
      /* Configure and Enable the color Keying for LCD Layer */
      (void)HAL_LTDC_ConfigColorKeying(hlcd_ltdc, Color, LayerIndex);
      (void)HAL_LTDC_EnableColorKeying(hlcd_ltdc, LayerIndex);
    }
    else
    {
      /* Configure and Enable the color Keying for LCD Layer */
      (void)HAL_LTDC_ConfigColorKeying_NoReload(hlcd_ltdc, Color, LayerIndex);
      (void)HAL_LTDC_EnableColorKeying_NoReload(hlcd_ltdc, LayerIndex);
    }
  }
  return ret;
}

int32_t BSP_LCD_ResetColorKeying(uint32_t Instance, uint32_t LayerIndex)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Lcd_Ctxx[Instance].ReloadEnable == 1U)
    {
      /* Disable the color Keying for LCD Layer */
      (void)HAL_LTDC_DisableColorKeying(hlcd_ltdc, LayerIndex);
    }
    else
    {
      /* Disable the color Keying for LCD Layer */
      (void)HAL_LTDC_DisableColorKeying_NoReload(hlcd_ltdc, LayerIndex);
    }
  }

  return ret;
}

int32_t BSP_LCD_GetXSize(uint32_t Instance, uint32_t *XSize)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else if(Lcd_Drvv->GetXSize != NULL)
  {
    *XSize = Lcd_Ctxx[Instance].XSize;
  }

  return ret;
}

int32_t BSP_LCD_GetYSize(uint32_t Instance, uint32_t *YSize)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else if(Lcd_Drvv->GetYSize != NULL)
  {
    *YSize = Lcd_Ctxx[Instance].YSize;
  }

  return ret;
}

int32_t BSP_LCD_DisplayOn(uint32_t Instance)
{
  int32_t ret;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Lcd_Drvv->DisplayOn(Lcd_CompObj) != DRV_ERR_NONE)
    {
      ret = DRV_ERROR_PERIPH_FAILURE;
    }
    else
    {
      ret = DRV_ERR_NONE;
    }
  }

  return ret;
}

int32_t BSP_LCD_DisplayOff(uint32_t Instance)
{
  int32_t ret;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Lcd_Drvv->DisplayOff(Lcd_CompObj) != DRV_ERR_NONE)
    {
      ret = DRV_ERROR_PERIPH_FAILURE;
    }
    else
    {
      ret = DRV_ERR_NONE;
    }
  }

  return ret;
}

int32_t BSP_LCD_SetBrightness(uint32_t Instance, uint32_t Brightness)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Lcd_Drvv->SetBrightness(Lcd_CompObj, Brightness) != DRV_ERR_NONE)
    {
      ret = DRV_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

int32_t BSP_LCD_GetBrightness(uint32_t Instance, uint32_t *Brightness)
{
  int32_t ret = DRV_ERR_NONE;

  if(Instance >= LCD_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {
    if(Lcd_Drvv->GetBrightness(Lcd_CompObj, Brightness) != DRV_ERR_NONE)
    {
      ret = DRV_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

int32_t BSP_LCD_DrawBitmap(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp)
{
  int32_t ret = DRV_ERR_NONE;
  uint32_t index, width, height, bit_pixel;
  uint32_t Address;
  uint32_t input_color_mode;
  uint8_t *pbmp;

  /* Get bitmap data address offset */
  index = (uint32_t)pBmp[10] + ((uint32_t)pBmp[11] << 8) + ((uint32_t)pBmp[12] << 16)  + ((uint32_t)pBmp[13] << 24);

  /* Read bitmap width */
  width = (uint32_t)pBmp[18] + ((uint32_t)pBmp[19] << 8) + ((uint32_t)pBmp[20] << 16)  + ((uint32_t)pBmp[21] << 24);

  /* Read bitmap height */
  height = (uint32_t)pBmp[22] + ((uint32_t)pBmp[23] << 8) + ((uint32_t)pBmp[24] << 16)  + ((uint32_t)pBmp[25] << 24);

  /* Read bit/pixel */
  bit_pixel = (uint32_t)pBmp[28] + ((uint32_t)pBmp[29] << 8);

  /* Set the address */
  Address = hlcd_ltdc->LayerCfg[Lcd_Ctxx[Instance].ActiveLayer].FBStartAdress + (((Lcd_Ctxx[Instance].XSize*Ypos) + Xpos)*Lcd_Ctxx[Instance].BppFactor);

  /* Get the layer pixel format */
  if ((bit_pixel/8U) == 4U)
  {
    input_color_mode = DMA2D_INPUT_ARGB8888;
  }
  else if ((bit_pixel/8U) == 2U)
  {
    input_color_mode = DMA2D_INPUT_RGB565;
  }
  else
  {
    input_color_mode = DMA2D_INPUT_RGB888;
  }

  /* Bypass the bitmap header */
  pbmp = pBmp + (index + (width * (height - 1U) * (bit_pixel/8U)));

  /* Convert picture to ARGB8888 pixel format */
  for(index=0; index < height; index++)
  {
    /* Pixel format conversion */
    LL_ConvertLineToRGB(Instance, (uint32_t *)pbmp, (uint32_t *)Address, width, input_color_mode);

    /* Increment the source and destination buffers */
    Address+=  (Lcd_Ctxx[Instance].XSize * Lcd_Ctxx[Instance].BppFactor);
    pbmp -= width*(bit_pixel/8U);
  }

  return ret;
}

int32_t BSP_LCD_FillRGBRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height)
{
    uint32_t i;

  uint32_t color, j;
  for(i = 0; i < Height; i++)
  {
    for(j = 0; j < Width; j++)
    {
      color = *pData | (*(pData + 1) << 8) | (*(pData + 2) << 16) | (*(pData + 3) << 24);
      BSP_LCD_WritePixel(Instance, Xpos + j, Ypos + i, color);
      pData += Lcd_Ctxx[Instance].BppFactor;
    }
  }
  return DRV_ERR_NONE;
}

int32_t BSP_LCD_DrawHLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  uint32_t  Xaddress;

  /* Get the line address */
  Xaddress = hlcd_ltdc->LayerCfg[Lcd_Ctxx[Instance].ActiveLayer].FBStartAdress + (Lcd_Ctxx[Instance].BppFactor*((Lcd_Ctxx[Instance].XSize*Ypos) + Xpos));

  /* Write line */
  if((Xpos + Length) > Lcd_Ctxx[Instance].XSize)
  {
    Length = Lcd_Ctxx[Instance].XSize - Xpos;
  }
  LL_FillBuffer(Instance, (uint32_t *)Xaddress, Length, 1, 0, Color);

  return DRV_ERR_NONE;
}

int32_t BSP_LCD_DrawVLine(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  uint32_t  Xaddress;

  /* Get the line address */
  Xaddress = (hlcd_ltdc->LayerCfg[Lcd_Ctxx[Instance].ActiveLayer].FBStartAdress) + (Lcd_Ctxx[Instance].BppFactor*(Lcd_Ctxx[Instance].XSize*Ypos + Xpos));

  /* Write line */
  if((Ypos + Length) > Lcd_Ctxx[Instance].YSize)
  {
    Length = Lcd_Ctxx[Instance].YSize - Ypos;
  }
 LL_FillBuffer(Instance, (uint32_t *)Xaddress, 1, Length, (Lcd_Ctxx[Instance].XSize - 1U), Color);

  return DRV_ERR_NONE;
}

/**
  * @brief  Draws a full rectangle in currently active layer.
  * @param  Instance   LCD Instance
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Width Rectangle width
  * @param  Height Rectangle height
  * @param  Color Pixel color
  * @retval BSP status.
  */
int32_t BSP_LCD_FillRect(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color)
{
  uint32_t  Xaddress;

  /* Get the rectangle start address */
  Xaddress = (hlcd_ltdc->LayerCfg[Lcd_Ctxx[Instance].ActiveLayer].FBStartAdress) + (Lcd_Ctxx[Instance].BppFactor*(Lcd_Ctxx[Instance].XSize*Ypos + Xpos));

  /* Fill the rectangle */
 LL_FillBuffer(Instance, (uint32_t *)Xaddress, Width, Height, (Lcd_Ctxx[Instance].XSize - Width), Color);

  return DRV_ERR_NONE;
}

int32_t BSP_LCD_ReadPixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t *Color)
{
  if(hlcd_ltdc->LayerCfg[Lcd_Ctxx[Instance].ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    /* Read data value from SDRAM memory */
    *Color = *(__IO uint32_t*) (hlcd_ltdc->LayerCfg[Lcd_Ctxx[Instance].ActiveLayer].FBStartAdress + (4U*(Ypos*Lcd_Ctxx[Instance].XSize + Xpos)));
  }
  else /* if((hlcd_ltdc->LayerCfg[layer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) */
  {
    /* Read data value from SDRAM memory */
    *Color = *(__IO uint16_t*) (hlcd_ltdc->LayerCfg[Lcd_Ctxx[Instance].ActiveLayer].FBStartAdress + (2U*(Ypos*Lcd_Ctxx[Instance].XSize + Xpos)));
  }

  return DRV_ERR_NONE;
}

int32_t BSP_LCD_WritePixel(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Color)
{
  if(hlcd_ltdc->LayerCfg[Lcd_Ctxx[Instance].ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
  {
    /* Write data value to SDRAM memory */
    *(__IO uint32_t*) (hlcd_ltdc->LayerCfg[Lcd_Ctxx[Instance].ActiveLayer].FBStartAdress + (4U*(Ypos*Lcd_Ctxx[Instance].XSize + Xpos))) = Color;
  }
  else
  {
    /* Write data value to SDRAM memory */
    *(__IO uint16_t*) (hlcd_ltdc->LayerCfg[Lcd_Ctxx[Instance].ActiveLayer].FBStartAdress + (2U*(Ypos*Lcd_Ctxx[Instance].XSize + Xpos))) = Color;
  }

  return DRV_ERR_NONE;
}

static void LL_FillBuffer(uint32_t Instance, uint32_t *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t Color)
{
  uint32_t output_color_mode, input_color = Color;

  switch(Lcd_Ctxx[Instance].PixelFormat)
  {
  case LCD_PIXEL_FORMAT_RGB565:
    output_color_mode = DMA2D_OUTPUT_RGB565; /* RGB565 */
    input_color = CONVERTRGB5652ARGB8888(Color);
    break;
  case LCD_PIXEL_FORMAT_RGB888:
  default:
    output_color_mode = DMA2D_OUTPUT_ARGB8888; /* ARGB8888 */
    break;
  }

  /* Register to memory mode with ARGB8888 as color Mode */
  hlcd_dma2d.Init.Mode         = DMA2D_R2M;
  hlcd_dma2d.Init.ColorMode    = output_color_mode;
  hlcd_dma2d.Init.OutputOffset = OffLine;

  hlcd_dma2d.Instance = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hlcd_dma2d) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hlcd_dma2d, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hlcd_dma2d, input_color, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        (void)HAL_DMA2D_PollForTransfer(&hlcd_dma2d, 25);
      }
    }
  }
}


static void LL_ConvertLineToRGB(uint32_t Instance, uint32_t *pSrc, uint32_t *pDst, uint32_t xSize, uint32_t ColorMode)
{
  uint32_t output_color_mode;

  switch(Lcd_Ctxx[Instance].PixelFormat)
  {
  case LCD_PIXEL_FORMAT_RGB565:
    output_color_mode = DMA2D_OUTPUT_RGB565; /* RGB565 */
    break;
  case LCD_PIXEL_FORMAT_RGB888:
  default:
    output_color_mode = DMA2D_OUTPUT_ARGB8888; /* ARGB8888 */
    break;
  }

  /* Configure the DMA2D Mode, Color Mode and output offset */
  hlcd_dma2d.Init.Mode         = DMA2D_M2M_PFC;
  hlcd_dma2d.Init.ColorMode    = output_color_mode;
  hlcd_dma2d.Init.OutputOffset = 0;

  /* Foreground Configuration */
  hlcd_dma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hlcd_dma2d.LayerCfg[1].InputAlpha = 0xFF;
  hlcd_dma2d.LayerCfg[1].InputColorMode = ColorMode;
  hlcd_dma2d.LayerCfg[1].InputOffset = 0;

  hlcd_dma2d.Instance = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hlcd_dma2d) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hlcd_dma2d, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hlcd_dma2d, (uint32_t)pSrc, (uint32_t)pDst, xSize, 1) == HAL_OK)
      {
        /* Polling For DMA transfer */
        (void)HAL_DMA2D_PollForTransfer(&hlcd_dma2d, 50);
      }
    }
  }
}

/*******************************************************************************
                       BSP Routines:
                                       LTDC
                                       DMA2D
                                       DSI
*******************************************************************************/


static void DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d)
{
  if(hdma2d->Instance == DMA2D)
  {
    /** Enable the DMA2D clock */
    __HAL_RCC_DMA2D_CLK_ENABLE();

    /** Toggle Sw reset of DMA2D IP */
    __HAL_RCC_DMA2D_FORCE_RESET();
    __HAL_RCC_DMA2D_RELEASE_RESET();
  }
}

static void DMA2D_MspDeInit(DMA2D_HandleTypeDef *hdma2d)
{
  if(hdma2d->Instance == DMA2D)
  {
    /** Disable IRQ of DMA2D IP */
    HAL_NVIC_DisableIRQ(DMA2D_IRQn);

    /** Force and let in reset state DMA2D */
    __HAL_RCC_DMA2D_FORCE_RESET();

    /** Disable the DMA2D */
    __HAL_RCC_DMA2D_CLK_DISABLE();
  }
}


static int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = DRV_ERR_NONE;

  if(Size <= 1U)
  {
    if(HAL_DSI_ShortWrite(hlcd_dsi, ChannelNbr, DSI_DCS_SHORT_PKT_WRITE_P1, Reg, (uint32_t)pData[Size]) != HAL_OK)
    {
      ret = DRV_ERROR_BUS_FAILURE;
    }
  }
  else
  {
    if(HAL_DSI_LongWrite(hlcd_dsi, ChannelNbr, DSI_DCS_LONG_PKT_WRITE, Size, (uint32_t)Reg, pData) != HAL_OK)
    {
      ret = DRV_ERROR_BUS_FAILURE;
    }
  }

  return ret;
}

static int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = DRV_ERR_NONE;

  if(HAL_DSI_Read(hlcd_dsi, ChannelNbr, pData, Size, DSI_DCS_SHORT_PKT_READ, Reg, pData) != HAL_OK)
  {
    ret = DRV_ERROR_BUS_FAILURE;
  }

  return ret;
}


int32_t BSP_LCDGetTick(void)
{
  return (int32_t)HAL_GetTick();
}

static int32_t OTM8009A_Probe(uint32_t ColorCoding, uint32_t Orientation)
{
  int32_t ret;
  uint32_t id;
  OTM8009A_IO_t              IOCtx;
  static OTM8009A_Object_t   OTM8009AObj;

  /* Configure the audio driver */
  IOCtx.Address     = 0;
  IOCtx.GetTick     = BSP_LCDGetTick;
  IOCtx.WriteReg    = DSI_IO_Write;
  IOCtx.ReadReg     = DSI_IO_Read;

  if(OTM8009A_RegisterBusIO(&OTM8009AObj, &IOCtx) != OTM8009A_OK)
  {
    ret = DRV_ERROR_BUS_FAILURE;
  }
  else
  {
    Lcd_CompObj = &OTM8009AObj;

    if(OTM8009A_ReadID(Lcd_CompObj, &id) != OTM8009A_OK)
    {
      ret = DRV_ERROR_COMPONENT_FAILURE;
    }

    else
    {
      Lcd_Drvv = (LCD_Drv_t *)(void *) &OTM8009A_LCD_Driver;
      if(Lcd_Drvv->Init(Lcd_CompObj, ColorCoding, Orientation) != OTM8009A_OK)
      {
        ret = DRV_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = DRV_ERR_NONE;
      }
    }
  }
  return ret;
}


