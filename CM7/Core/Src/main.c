/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma2d.h"
#include "dsihost.h"
#include "ltdc.h"
#include "quadspi.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stlogo.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define DSI_RGB888                 0x00000005U
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */
static void LetsDrawSometging(void);

extern void SDRAM_demo (void);
extern void SDRAM_DMA_demo (void);


void QSPI_demo (void);
void QSPI_TestDist(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void LTDC_MspInit(LTDC_HandleTypeDef *hltdc)
{
  if(hltdc->Instance == LTDC)
  {
    /** Enable the LTDC clock */
    __HAL_RCC_LTDC_CLK_ENABLE();


    /** Toggle Sw reset of LTDC IP */
    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();
  }
}

void DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d)
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

void DSI_MspInit(DSI_HandleTypeDef *hdsi)
{
  if(hdsi->Instance == DSI)
  {
    /** Enable DSI Host and wrapper clocks */
    __HAL_RCC_DSI_CLK_ENABLE();

    /** Soft Reset the DSI Host and wrapper */
    __HAL_RCC_DSI_FORCE_RESET();
    __HAL_RCC_DSI_RELEASE_RESET();
  }
}

int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  if(Size <= 1U)
  {
    if(HAL_DSI_ShortWrite(hlcd_dsi, ChannelNbr, DSI_DCS_SHORT_PKT_WRITE_P1, Reg, (uint32_t)pData[Size]) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }
  else
  {
    if(HAL_DSI_LongWrite(hlcd_dsi, ChannelNbr, DSI_DCS_LONG_PKT_WRITE, Size, (uint32_t)Reg, pData) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
  }

  return ret;
}

int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = BSP_ERROR_NONE;

  if(HAL_DSI_Read(hlcd_dsi, ChannelNbr, pData, Size, DSI_DCS_SHORT_PKT_READ, Reg, pData) != HAL_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }

  return ret;
}

LCD_Drv_t                *Lcd_Drv = NULL;

int32_t OTM8009A_Probe(uint32_t ColorCoding, uint32_t Orientation)
{
  int32_t ret;
  uint32_t id;
  OTM8009A_IO_t              IOCtx;
  static OTM8009A_Object_t   OTM8009AObj;

  /* Configure the audio driver */
  IOCtx.Address     = 0;
  IOCtx.GetTick     = BSP_GetTick;
  IOCtx.WriteReg    = DSI_IO_Write;
  IOCtx.ReadReg     = DSI_IO_Read;

  if(OTM8009A_RegisterBusIO(&OTM8009AObj, &IOCtx) != OTM8009A_OK)
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  else
  {
    Lcd_CompObj = &OTM8009AObj;

    if(OTM8009A_ReadID(Lcd_CompObj, &id) != OTM8009A_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }

    else
    {
      Lcd_Drv = (LCD_Drv_t *)(void *) &OTM8009A_LCD_Driver;
      if(Lcd_Drv->Init(Lcd_CompObj, ColorCoding, Orientation) != OTM8009A_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }
  return ret;
}

int32_t BSP_LCD_Init(uint32_t Instance, uint32_t Orientation,DSI_HandleTypeDef *hdsi )
{
	hlcd_dsi = hdsi;

  return BSP_LCD_InitEx(Instance, Orientation, LCD_PIXEL_FORMAT_RGB888, LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGHT);
}

int32_t BSP_LCD_InitEx(uint32_t Instance, uint32_t Orientation, uint32_t PixelFormat, uint32_t Width, uint32_t Height)
{
	int32_t ret = BSP_ERROR_NONE;
	uint32_t ctrl_pixel_format, ltdc_pixel_format;
	MX_LTDC_LayerConfig_t config;

	ltdc_pixel_format = LTDC_PIXEL_FORMAT_ARGB8888;

	ctrl_pixel_format = OTM8009A_FORMAT_RGB888;
	Lcd_Ctx[Instance].BppFactor = 4U;
    /* Store pixel format, xsize and ysize information */
    Lcd_Ctx[Instance].PixelFormat = PixelFormat;
    Lcd_Ctx[Instance].XSize  = Width;
    Lcd_Ctx[Instance].YSize  = Height;
    /* Toggle Hardware Reset of the LCD using its XRES signal (active low) */

    GPIO_InitTypeDef  gpio_init_structure;

    __HAL_RCC_GPIOG_CLK_ENABLE(); /*LCD RESET CLK ENABLE*/

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
    /* Initialize LCD special pins GPIOs */

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

    /* Initializes peripherals instance value */
    hlcd_ltdc.Instance = LTDC;
    hlcd_dma2d.Instance = DMA2D;
    hlcd_dsi->Instance = DSI;

    /* MSP initialization */
    LTDC_MspInit(&hlcd_ltdc);
    DMA2D_MspInit(&hlcd_dma2d);
    DSI_MspInit(hlcd_dsi);
    MX_DSIHOST_DSI_Init();


    if(MX_LTDC_ClockConfig(&hlcd_ltdc) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
     if(MX_LTDC_Init(&hlcd_ltdc, Width, Height) != HAL_OK)
     {
       ret = BSP_ERROR_PERIPH_FAILURE;
     }
    }

    if(ret == BSP_ERROR_NONE)
    {
      /* Before configuring LTDC layer, ensure SDRAM is initialized */
#if !defined(DATA_IN_ExtSDRAM)
      /* Initialize the SDRAM */
      if(BSP_SDRAM_Init(0) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_PERIPH_FAILURE;
      }
#endif /* DATA_IN_ExtSDRAM */

      /* Configure default LTDC Layer 0. This configuration can be override by calling
      BSP_LCD_ConfigLayer() at application level */
      config.X0          = 0;
      config.X1          = Width;
      config.Y0          = 0;
      config.Y1          = Height;
      config.PixelFormat = ltdc_pixel_format;
      config.Address     = LCD_LAYER_0_ADDRESS;
      if(MX_LTDC_ConfigLayer(&hlcd_ltdc, 0, &config) != HAL_OK)
      {
        ret = BSP_ERROR_PERIPH_FAILURE;
      }
      else
      {
        /* Enable the DSI host and wrapper after the LTDC initialization
        To avoid any synchronization issue, the DSI shall be started after enabling the LTDC */
        (void)HAL_DSI_Start(hlcd_dsi);

        /* Enable the DSI BTW for read operations */
        (void)HAL_DSI_ConfigFlowControl(hlcd_dsi, DSI_FLOW_CONTROL_BTA);

#if (USE_LCD_CTRL_OTM8009A == 1)
        /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
        depending on configuration of DSI */
        if(OTM8009A_Probe(ctrl_pixel_format, Orientation) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_UNKNOWN_COMPONENT;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
#endif
      }
    /* By default the reload is activated and executed immediately */
    Lcd_Ctx[Instance].ReloadEnable = 1U;
   }
  return ret;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
/* USER CODE BEGIN Boot_Mode_Sequence_0 */
  int32_t timeout;
/* USER CODE END Boot_Mode_Sequence_0 */

/* USER CODE BEGIN Boot_Mode_Sequence_1 */
  /* Wait until CPU2 boots and enters in stop mode or timeout*/
  timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));
  if ( timeout < 0 )
  {
  Error_Handler();
  }
/* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();
/* USER CODE BEGIN Boot_Mode_Sequence_2 */
/* When system initialization is finished, Cortex-M7 will release Cortex-M4 by means of
HSEM notification */
/*HW semaphore Clock enable*/
__HAL_RCC_HSEM_CLK_ENABLE();
/*Take HSEM */
HAL_HSEM_FastTake(HSEM_ID_0);
/*Release HSEM in order to notify the CPU2(CM4)*/
HAL_HSEM_Release(HSEM_ID_0,0);
/* wait until CPU2 wakes up from stop mode */
timeout = 0xFFFF;
while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) == RESET) && (timeout-- > 0));
if ( timeout < 0 )
{
Error_Handler();
}
/* USER CODE END Boot_Mode_Sequence_2 */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
//  MX_DSIHOST_DSI_Init();
//  MX_FMC_Init();
//  MX_LTDC_Initt();
//  MX_DMA2D_Init();
//  MX_QUADSPI_Init();
  /* USER CODE BEGIN 2 */

  /* Configure the Wakeup push-button in EXTI Mode */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO);
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /*##-1- Initialize the LCD #################################################*/
  /* Initialize the LCD */
  BSP_LCD_Init(0, LCD_ORIENTATION_LANDSCAPE,&hdsi);
  UTIL_LCD_SetFuncDriver(&LCD_Driver);
  UTIL_LCD_SetFont(&UTIL_LCD_DEFAULT_FONT);
  QSPI_demo();
  HAL_Delay(2000);
  LetsDrawSometging();
  HAL_Delay(5000);

//  LCD_SetHint();
//  HAL_Delay(2000);
//
//  for(uint32_t feature = 0; feature < 4; feature++)
//  {
//	  LCD_Show_Feature(feature);
//	  HAL_Delay(1000);
//  }

//  SDRAM_demo();
//  HAL_Delay(3000);
//

//  HAL_Delay(3000);
//


//  Touchscreen_demo1();

static uint32_t LedTime =0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  if(LedTime + 400 < HAL_GetTick() )
	  {
		  LedTime = HAL_GetTick();

		  BSP_LED_Toggle(LED_GREEN);
		  BSP_LED_Toggle(LED_ORANGE);
		  BSP_LED_Toggle(LED_RED);
		  BSP_LED_Toggle(LED_BLUE);
	  }

//	  QSPI_TestDist();

//	  while(HAL_GPIO_ReadPin(BUTTON_WAKEUP_GPIO_PORT, BUTTON_WAKEUP_PIN) == true)
//	  {
//		  /*Stop process*/
//	  }
//	  UTIL_LCD_Clear(UTIL_LCD_COLOR_LIGHTRED);
//	  HAL_Delay(30);
//
//
//	  while(HAL_GPIO_ReadPin(BUTTON_WAKEUP_GPIO_PORT, BUTTON_WAKEUP_PIN) == true)
//	  {
//		  /*Stop process*/
//	  }
//	  UTIL_LCD_Clear(UTIL_LCD_COLOR_GREEN);
//	  HAL_Delay(30);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  PeriphClkInitStruct.PLL2.PLL2M = 16;
  PeriphClkInitStruct.PLL2.PLL2N = 129;
  PeriphClkInitStruct.PLL2.PLL2P = 2;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

static void LetsDrawSometging(void)
{
  uint32_t x_size;
  uint32_t y_size;
  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);
  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /*  Set the LCD Text Color */
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_DARKBLUE);
  UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"HELLO DISPLAY", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 300, (uint8_t *)"This example shows the different", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 345, (uint8_t *)"Its really simple but difficult :/ ", CENTER_MODE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 380, (uint8_t *)"Disturbances test will be activated", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 410, (uint8_t *)"in time 5 seconds!", CENTER_MODE);

  UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size- 100, UTIL_LCD_COLOR_MAGENTA);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size- 102, UTIL_LCD_COLOR_MAGENTA);
 }


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
