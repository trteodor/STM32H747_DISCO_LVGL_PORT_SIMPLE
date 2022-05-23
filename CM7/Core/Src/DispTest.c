



#include "main.h"
#include "dma2d.h"
#include "dsihost.h"
#include "i2c.h"
#include "ltdc.h"
#include "mdma.h"
#include "quadspi.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "image_320x240_argb8888.h"
#include "life_augmented_argb8888.h"
#include <string.h>
#include <stdio.h>

#include "ltdc.h"
#include "dsihost.h"




/* USER CODE BEGIN PTD */
extern LTDC_HandleTypeDef *hlcd_ltdc;
extern DMA2D_HandleTypeDef   hdma2d;
extern DSI_HandleTypeDef *hlcd_dsi;
DSI_VidCfgTypeDef hdsivideo_handle;
DSI_CmdCfgTypeDef CmdCfgg;
DSI_LPCmdTypeDef LPCmdd;
DSI_PLLInitTypeDef dsiPllInitt;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;



/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define VSYNC           1
#define VBP             1
#define VFP             1
#define VACT            480
#define HSYNC           1
#define HBP             1
#define HFP             1
#define HACT            400      /* !!!! SCREEN DIVIDED INTO 2 AREAS !!!! */


#define LEFT_AREA         1
#define RIGHT_AREA        2

#define __DSI_MASK_TE()   (GPIOJ->AFR[0] &= (0xFFFFF0FFU))   /* Mask DSI TearingEffect Pin*/
#define __DSI_UNMASK_TE() (GPIOJ->AFR[0] |= ((uint32_t)(GPIO_AF13_DSI) << 8)) /* UnMask DSI TearingEffect Pin*/


/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static volatile int32_t pending_buffer = -1;
static volatile int32_t active_area = 0;
static volatile uint32_t ImageIndex = 0;
static const uint32_t * Images[] =
{
  image_320x240_argb8888,
  life_augmented_argb8888,
};

static uint8_t pColLeft[]    = {0x00, 0x00, 0x01, 0x8F}; /*   0 -> 399 */
static uint8_t pColRight[]   = {0x01, 0x90, 0x03, 0x1F}; /* 400 -> 799 */
static uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
static uint8_t pScanCol[]    = {0x02, 0x15};             /* Scan @ 533 */



static volatile uint8_t FlagDmaTransmitEnd;
/* Private function prototypes -----------------------------------------------*/

static void CopyPicture(uint32_t *pSrc,
                           uint32_t *pDst,
                           uint16_t x,
                           uint16_t y,
                           uint16_t xsize,
                           uint16_t ysize);


void DMA2D_TransmitCpltCallBack(DMA2D_HandleTypeDef *hdma2d);
static void LL_DMAFlushBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize,void(*DMAtrEndCb)(void));

void LvglFlushBuffer(uint32_t *pixelMap, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize,void(*DMAtrEndCb)(void));

static uint8_t LCD_Init(void);
static void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address);
static void LTDC_Init(void);

static int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);
static int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size);
int32_t LCD_GetXSize(uint32_t Instance, uint32_t *XSize);
int32_t LCD_GetYSize(uint32_t Instance, uint32_t *YSize);



void LCD_MspInit(void);

const LCD_UTILS_Drv_t LCD_UTIL_Driver =
{
  BSP_LCD_DrawBitmap,
  BSP_LCD_FillRGBRect,
  BSP_LCD_DrawHLine,
  BSP_LCD_DrawVLine,
  BSP_LCD_FillRect,
  BSP_LCD_ReadPixel,
  BSP_LCD_WritePixel,
  LCD_GetXSize,
  LCD_GetYSize,
  BSP_LCD_SetActiveLayer,
  BSP_LCD_GetPixelFormat
};









void LCD_Task(void)
{
	static int i=0;
    if(pending_buffer < 0)
    {
    	LL_DMAFlushBuffer((uint32_t *)Images[ImageIndex++], (uint32_t *)LCD_FRAME_BUFFER, 0, 0, 10*i, 480,(void*)DMA2D_TransmitCpltCallBack);

	i++;
	if(i> 80) i =0;


      if(ImageIndex >= 2)
      {
        ImageIndex = 0;
      }
      pending_buffer = 1;

      /* UnMask the TE */
      __DSI_UNMASK_TE();
    }
    /* Wait some time before switching to next image */
    HAL_Delay(50);
}


static void(*TransmisionCpltCb)(void) = NULL;


void LvglFlushBuffer(uint32_t *pixelMap, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize,void(*DMAtrEndCb)(void))
{

//    if(pending_buffer < 0)
    {
        /* UnMask the TE */
    	TransmisionCpltCb = (void*)DMAtrEndCb;



    	LL_DMAFlushBuffer(pixelMap, (uint32_t *)LCD_FRAME_BUFFER, x, y, xsize, ysize,(void*)DMAtrEndCb);
        __DSI_UNMASK_TE();
    }
    /* Wait some time before switching to next image */
//    HAL_Delay(30);
}



static void LCD_BriefDisplay(void)
{
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_FillRect(0, 0, Lcd_Ctxx[0].XSize, 112,UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_FillRect(0, 112, Lcd_Ctxx[0].XSize, 368, UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DisplayStringAtLine(1, (uint8_t *)"   LCD_DSI_CmdMode_TearingEffect_ExtPin");
  UTIL_LCD_SetFont(&Font16);
  UTIL_LCD_DisplayStringAtLine(4, (uint8_t *)"This example shows how to display images on LCD DSI and prevent");
  UTIL_LCD_DisplayStringAtLine(5, (uint8_t *)"Tearing Effect using DSI_TE pin ");

}


void LCD_OTM8009a_InitFull(void)
{
	  hlcd_dsi = &hdsi;
	  hlcd_ltdc = &hltdc;

	  /* Initialize the LCD DSI in Command mode with LANDSCAPE orientation */
	   LCD_Init();

	   /* Set the LCD Context */
	   Lcd_Ctxx[0].ActiveLayer = 0;
	   Lcd_Ctxx[0].PixelFormat = LCD_PIXEL_FORMAT_ARGB8888;
	   Lcd_Ctxx[0].BppFactor = 4; /* 4 Bytes Per Pixel for ARGB8888 */
	   Lcd_Ctxx[0].XSize = 800;
	   Lcd_Ctxx[0].YSize = 480;

	   /* Initialize the LCD   */
	   if( LCD_Init() != DRV_ERR_NONE)
	   {
	     Error_Handler();
	   }

	   /* Disable DSI Wrapper in order to access and configure the LTDC */
	   __HAL_DSI_WRAPPER_DISABLE(hlcd_dsi);

	   /* Initialize LTDC layer 0 iused for Hint */
	   LCD_LayertInit(0, LCD_FRAME_BUFFER);
	   UTIL_LCD_SetFuncDriver(&LCD_UTIL_Driver);
	   /* Update pitch : the draw is done on the whole physical X Size */
	   HAL_LTDC_SetPitch(hlcd_ltdc, Lcd_Ctxx[0].XSize, 0);

	   /* Enable DSI Wrapper so DSI IP will drive the LTDC */
	   __HAL_DSI_WRAPPER_ENABLE(hlcd_dsi);

	   HAL_DSI_LongWrite(hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft);
	   HAL_DSI_LongWrite(hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_PASET, pPage);

		/* Display example brief   */
		LCD_BriefDisplay();

		/* Show first image */
		CopyPicture((uint32_t *)Images[ImageIndex++], (uint32_t *)LCD_FRAME_BUFFER, 240, 160, 320, 240);


	   pending_buffer = 0;
	   active_area = LEFT_AREA;

	   HAL_DSI_LongWrite(hlcd_dsi, 0, DSI_DCS_LONG_PKT_WRITE, 2, OTM8009A_CMD_WRTESCN, pScanCol);

	   /* Send Display On DCS Command to display */
	   HAL_DSI_ShortWrite(hlcd_dsi,
	                      0,
	                      DSI_DCS_SHORT_PKT_WRITE_P1,
	                      OTM8009A_CMD_DISPON,
	                      0x00);

	   HAL_Delay(1000);
}




/**
  * @brief  Gets the LCD X size.
  * @param  Instance  LCD Instance
  * @param  XSize     LCD width
  * @retval BSP status
  */
int32_t LCD_GetXSize(uint32_t Instance, uint32_t *XSize)
{
  *XSize = Lcd_Ctxx[0].XSize;

  return DRV_ERR_NONE;
}

/**
  * @brief  Gets the LCD Y size.
  * @param  Instance  LCD Instance
  * @param  YSize     LCD Height
  * @retval BSP status
  */
int32_t LCD_GetYSize(uint32_t Instance, uint32_t *YSize)
{
  *YSize = Lcd_Ctxx[0].YSize;

  return DRV_ERR_NONE;
}


/**
  * @brief  Tearing Effect DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_TearingEffectCallback(DSI_HandleTypeDef *hdsi)
{
  /* Mask the TE */
  __DSI_MASK_TE();

  /* Refresh the right part of the display */
  HAL_DSI_Refresh(hdsi);
}

/**
  * @brief  End of Refresh DSI callback.
  * @param  hdsi: pointer to a DSI_HandleTypeDef structure that contains
  *               the configuration information for the DSI.
  * @retval None
  */
void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
    if(active_area == LEFT_AREA)
    {
      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(hdsi);
      /* Update LTDC configuaration */
      LTDC_LAYER(hlcd_ltdc, 0)->CFBAR = LCD_FRAME_BUFFER + 400 * 4;
      __HAL_LTDC_RELOAD_CONFIG(hlcd_ltdc);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(hdsi);

      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColRight);
      /* Refresh the right part of the display */
      HAL_DSI_Refresh(hdsi);

    }
    else if(active_area == RIGHT_AREA)
    {

      /* Disable DSI Wrapper */
      __HAL_DSI_WRAPPER_DISABLE(hlcd_dsi);
      /* Update LTDC configuaration */
      LTDC_LAYER(hlcd_ltdc, 0)->CFBAR = LCD_FRAME_BUFFER;
      __HAL_LTDC_RELOAD_CONFIG(hlcd_ltdc);
      /* Enable DSI Wrapper */
      __HAL_DSI_WRAPPER_ENABLE(hlcd_dsi);

      HAL_DSI_LongWrite(hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pColLeft);
      if(TransmisionCpltCb != 0){
    	  TransmisionCpltCb();
      }

    }
  active_area = (active_area == LEFT_AREA)? RIGHT_AREA : LEFT_AREA;
}


int32_t DISP_GetTick(void)
{
  return (int32_t)HAL_GetTick();
}

/**
  * @brief  Initializes the DSI LCD.
  * The ititialization is done as below:
  *     - DSI PLL ititialization
  *     - DSI ititialization
  *     - LTDC ititialization
  *     - OTM8009A LCD Display IC Driver ititialization
  * @param  None
  * @retval LCD state
  */
static uint8_t LCD_Init(void){

  GPIO_InitTypeDef GPIO_Init_Structure;
  DSI_PHY_TimerTypeDef  PhyTimings;
  OTM8009A_IO_t              IOCtx;
  static OTM8009A_Object_t   OTM8009AObj;
  static void                *Lcd_CompObj = NULL;

  /* Toggle Hardware Reset of the DSI LCD using
     its XRES signal (active low) */
  BSP_LCD_Reset(0);

  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  LCD_MspInit();

  /* LCD clock configuration */
  /* LCD clock configuration */
  /* PLL3_VCO Input = HSE_VALUE/PLL3M = 5 Mhz */
  /* PLL3_VCO Output = PLL3_VCO Input * PLL3N = 800 Mhz */
  /* PLLLCDCLK = PLL3_VCO Output/PLL3R = 800/19 = 42 Mhz */
  /* LTDC clock frequency = PLLLCDCLK = 42 Mhz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLL3.PLL3M = 5;
  PeriphClkInitStruct.PLL3.PLL3N = 160;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 2;
  PeriphClkInitStruct.PLL3.PLL3R = 19;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hlcd_dsi->Instance = DSI;

  HAL_DSI_DeInit(hlcd_dsi);

  dsiPllInitt.PLLNDIV  = 100;
  dsiPllInitt.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInitt.PLLODF  = DSI_PLL_OUT_DIV1;

  hlcd_dsi->Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  hlcd_dsi->Init.TXEscapeCkdiv = 0x4;


  HAL_DSI_Init(hlcd_dsi, &(dsiPllInitt));

  /* Configure the DSI for Command mode */
  CmdCfgg.VirtualChannelID      = 0;
  CmdCfgg.HSPolarity            = DSI_HSYNC_ACTIVE_HIGH;
  CmdCfgg.VSPolarity            = DSI_VSYNC_ACTIVE_HIGH;
  CmdCfgg.DEPolarity            = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfgg.ColorCoding           = DSI_RGB888;
  CmdCfgg.CommandSize           = HACT;
  CmdCfgg.TearingEffectSource   = DSI_TE_EXTERNAL;
  CmdCfgg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfgg.VSyncPol              = DSI_VSYNC_FALLING;
  CmdCfgg.AutomaticRefresh      = DSI_AR_DISABLE;
  CmdCfgg.TEAcknowledgeRequest  = DSI_TE_ACKNOWLEDGE_DISABLE;
  HAL_DSI_ConfigAdaptedCommandMode(hlcd_dsi, &CmdCfgg);

  LPCmdd.LPGenShortWriteNoP    = DSI_LP_GSW0P_ENABLE;
  LPCmdd.LPGenShortWriteOneP   = DSI_LP_GSW1P_ENABLE;
  LPCmdd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_ENABLE;
  LPCmdd.LPGenShortReadNoP     = DSI_LP_GSR0P_ENABLE;
  LPCmdd.LPGenShortReadOneP    = DSI_LP_GSR1P_ENABLE;
  LPCmdd.LPGenShortReadTwoP    = DSI_LP_GSR2P_ENABLE;
  LPCmdd.LPGenLongWrite        = DSI_LP_GLW_ENABLE;
  LPCmdd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_ENABLE;
  LPCmdd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_ENABLE;
  LPCmdd.LPDcsShortReadNoP     = DSI_LP_DSR0P_ENABLE;
  LPCmdd.LPDcsLongWrite        = DSI_LP_DLW_ENABLE;
  HAL_DSI_ConfigCommand(hlcd_dsi, &LPCmdd);

  /* Initialize LTDC */
  LTDC_Init();

  /* Start DSI */
  HAL_DSI_Start(hlcd_dsi);

  /* Configure DSI PHY HS2LP and LP2HS timings */
  PhyTimings.ClockLaneHS2LPTime = 35;
  PhyTimings.ClockLaneLP2HSTime = 35;
  PhyTimings.DataLaneHS2LPTime = 35;
  PhyTimings.DataLaneLP2HSTime = 35;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 10;
  HAL_DSI_ConfigPhyTimer(hlcd_dsi, &PhyTimings);

  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver) */
  IOCtx.Address     = 0;
  IOCtx.GetTick     = DISP_GetTick;
  IOCtx.WriteReg    = DSI_IO_Write;
  IOCtx.ReadReg     = DSI_IO_Read;
  OTM8009A_RegisterBusIO(&OTM8009AObj, &IOCtx);
  Lcd_CompObj=(&OTM8009AObj);
  OTM8009A_Init(Lcd_CompObj, OTM8009A_COLMOD_RGB888, LCD_ORIENTATION_LANDSCAPE);

  LPCmdd.LPGenShortWriteNoP    = DSI_LP_GSW0P_DISABLE;
  LPCmdd.LPGenShortWriteOneP   = DSI_LP_GSW1P_DISABLE;
  LPCmdd.LPGenShortWriteTwoP   = DSI_LP_GSW2P_DISABLE;
  LPCmdd.LPGenShortReadNoP     = DSI_LP_GSR0P_DISABLE;
  LPCmdd.LPGenShortReadOneP    = DSI_LP_GSR1P_DISABLE;
  LPCmdd.LPGenShortReadTwoP    = DSI_LP_GSR2P_DISABLE;
  LPCmdd.LPGenLongWrite        = DSI_LP_GLW_DISABLE;
  LPCmdd.LPDcsShortWriteNoP    = DSI_LP_DSW0P_DISABLE;
  LPCmdd.LPDcsShortWriteOneP   = DSI_LP_DSW1P_DISABLE;
  LPCmdd.LPDcsShortReadNoP     = DSI_LP_DSR0P_DISABLE;
  LPCmdd.LPDcsLongWrite        = DSI_LP_DLW_DISABLE;
  HAL_DSI_ConfigCommand(hlcd_dsi, &LPCmdd);

  HAL_DSI_ConfigFlowControl(hlcd_dsi, DSI_FLOW_CONTROL_BTA);
  HAL_DSI_ForceRXLowPower(hlcd_dsi, ENABLE);

  /* Enable GPIOJ clock */
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /* Configure DSI_TE pin from MB1166 : Tearing effect on separated GPIO from KoD LCD */
  /* that is mapped on GPIOJ2 as alternate DSI function (DSI_TE)                      */
  /* This pin is used only when the LCD and DSI link is configured in command mode    */
  GPIO_Init_Structure.Pin       = GPIO_PIN_2;
  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_NOPULL;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF13_DSI;
  HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);

  return DRV_ERR_NONE;
}

/**
  * @brief  Initialize the LTDC
  * @param  None
  * @retval None
  */
static void LTDC_Init(void)
{
  /* DeInit */
  hlcd_ltdc->Instance = LTDC;
  HAL_LTDC_DeInit(hlcd_ltdc);

  /* LTDC Config */
  /* Timing and polarity */
  hlcd_ltdc->Init.HorizontalSync = HSYNC;
  hlcd_ltdc->Init.VerticalSync = VSYNC;
  hlcd_ltdc->Init.AccumulatedHBP = HSYNC+HBP;
  hlcd_ltdc->Init.AccumulatedVBP = VSYNC+VBP;
  hlcd_ltdc->Init.AccumulatedActiveH = VSYNC+VBP+VACT;
  hlcd_ltdc->Init.AccumulatedActiveW = HSYNC+HBP+HACT;
  hlcd_ltdc->Init.TotalHeigh = VSYNC+VBP+VACT+VFP;
  hlcd_ltdc->Init.TotalWidth = HSYNC+HBP+HACT+HFP;

  /* background value */
  hlcd_ltdc->Init.Backcolor.Blue = 0;
  hlcd_ltdc->Init.Backcolor.Green = 0;
  hlcd_ltdc->Init.Backcolor.Red = 0;

  /* Polarity */
  hlcd_ltdc->Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hlcd_ltdc->Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hlcd_ltdc->Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hlcd_ltdc->Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hlcd_ltdc->Instance = LTDC;

  HAL_LTDC_Init(hlcd_ltdc);
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
static void LCD_LayertInit(uint16_t LayerIndex, uint32_t Address)
{
    LTDC_LayerCfgTypeDef  layercfg;

  /* Layer Init */
  layercfg.WindowX0 = 0;
  layercfg.WindowX1 = Lcd_Ctxx[0].XSize/2 ;
  layercfg.WindowY0 = 0;
  layercfg.WindowY1 = Lcd_Ctxx[0].YSize;
  layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  layercfg.FBStartAdress = Address;
  layercfg.Alpha = 255;
  layercfg.Alpha0 = 0;
  layercfg.Backcolor.Blue = 0;
  layercfg.Backcolor.Green = 0;
  layercfg.Backcolor.Red = 0;
  layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  layercfg.ImageWidth = Lcd_Ctxx[0].XSize / 2;
  layercfg.ImageHeight = Lcd_Ctxx[0].YSize;

  HAL_LTDC_ConfigLayer(hlcd_ltdc, &layercfg, LayerIndex);
}

/**
  * @brief  DCS or Generic short/long write command
  * @param  ChannelNbr Virtual channel ID
  * @param  Reg Register to be written
  * @param  pData pointer to a buffer of data to be write
  * @param  Size To precise command to be used (short or long)
  * @retval BSP status
  */
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

/**
  * @brief  DCS or Generic read command
  * @param  ChannelNbr Virtual channel ID
  * @param  Reg Register to be read
  * @param  pData pointer to a buffer to store the payload of a read back operation.
  * @param  Size  Data size to be read (in byte).
  * @retval BSP status
  */
static int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData, uint16_t Size)
{
  int32_t ret = DRV_ERR_NONE;

  if(HAL_DSI_Read(hlcd_dsi, ChannelNbr, pData, Size, DSI_DCS_SHORT_PKT_READ, Reg, pData) != HAL_OK)
  {
    ret = DRV_ERROR_BUS_FAILURE;
  }

  return ret;
}

void LCD_MspInit(void)
{

  /** @brief Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /** @brief Toggle Sw reset of LTDC IP */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();

  /** @brief Enable the DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /** @brief Toggle Sw reset of DMA2D IP */
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DMA2D_RELEASE_RESET();

  /** @brief Enable DSI Host and wrapper clocks */
  __HAL_RCC_DSI_CLK_ENABLE();

  /** @brief Soft Reset the DSI Host and wrapper */
  __HAL_RCC_DSI_FORCE_RESET();
  __HAL_RCC_DSI_RELEASE_RESET();

  /** @brief NVIC configuration for LTDC interrupt that is now enabled */
  HAL_NVIC_SetPriority(LTDC_IRQn, 9, 0xf);
  HAL_NVIC_EnableIRQ(LTDC_IRQn);

  /** @brief NVIC configuration for DMA2D interrupt that is now enabled */
  HAL_NVIC_SetPriority(DMA2D_IRQn, 9, 0xf);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);

  /** @brief NVIC configuration for DSI interrupt that is now enabled */
  HAL_NVIC_SetPriority(DSI_IRQn, 9, 0xf);
  HAL_NVIC_EnableIRQ(DSI_IRQn);
}



/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color
  * @param  xSize: Buffer width
  * @param  ColorMode: Input color mode
  * @retval None
  */
static void CopyPicture(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{

  uint32_t destination = (uint32_t)pDst + (y * 800 + x) * 4;
  uint32_t source      = (uint32_t)pSrc;

  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
  hdma2d.Init.Mode         = DMA2D_M2M;
  hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 800 - xsize;
  hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/
  hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  hdma2d.XferCpltCallback  = NULL;

  /*##-3- Foreground Configuration ###########################################*/
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

  hdma2d.Instance          = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&hdma2d, 100);
      }
    }
  }
}



static void LL_DMAFlushBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize,void(*DMAtrEndCb)(void))
{

  uint32_t destination = (uint32_t)pDst + (y * 800 + x) * 4;
  uint32_t source      = (uint32_t)pSrc;

  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
  hdma2d.Init.Mode         = DMA2D_M2M;
  hdma2d.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 800 - xsize;
  hdma2d.Init.AlphaInverted = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion*/
  hdma2d.Init.RedBlueSwap   = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */

  /*##-2- DMA2D Callbacks Configuration ######################################*/
  hdma2d.XferCpltCallback  = (void *)DMA2D_TransmitCpltCallBack;

  /*##-3- Foreground Configuration ###########################################*/
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR; /* No ForeGround Red/Blue swap */
  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

  hdma2d.Instance          = DMA2D;


  FlagDmaTransmitEnd = 1;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start_IT(&hdma2d, source, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */
//    	  HAL_DMA2D_PollForTransfer(&hdma2d, 100);
//        while(FlagDmaTransmitEnd == 1)
//        {
//        	DMAtrEndCb();
//        }
      }
    }
  }
}

void DMA2D_TransmitCpltCallBack(DMA2D_HandleTypeDef *hdma2d)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hdma2d);
  FlagDmaTransmitEnd = 0;

}





