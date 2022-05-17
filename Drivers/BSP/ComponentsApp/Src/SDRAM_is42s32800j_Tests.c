

/* Includes ------------------------------------------------------------------*/
#include <Display_OTM8009A.h>
#include "main.h"


/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE            ((uint32_t)0x0100)
#define WRITE_READ_ADDR        ((uint32_t)0x1000)



#define SDRAM_INSTANCES_NBR       1U
#define SDRAM_DEVICE_ADDR         0xD0000000U
#define SDRAM_DEVICE_SIZE         0x2000000U

/* MDMA definitions for SDRAM DMA transfer */
#define SDRAM_MDMAx_CLK_ENABLE             __HAL_RCC_MDMA_CLK_ENABLE
#define SDRAM_MDMAx_CLK_DISABLE            __HAL_RCC_MDMA_CLK_DISABLE
#define SDRAM_MDMAx_CHANNEL                MDMA_Channel0
#define SDRAM_MDMAx_IRQn                   MDMA_IRQn
#define SDRAM_MDMA_IRQHandler              MDMA_IRQHandler


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t sdram_aTxBuffer[BUFFER_SIZE];
uint32_t sdram_aRxBuffer[BUFFER_SIZE];

/* DMA transfer complete flag */
__IO uint32_t uwMDMA_Transfer_Complete = 0;
/* Private function prototypes -----------------------------------------------*/
static void SDRAM_SetHint(void);
static void SDRAM_DMA_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SDRAM Demo
  * @param  None
  * @retval None
  */
void SDRAM_demo (void)
{
  SDRAM_SetHint();

   UTIL_LCD_DisplayStringAt(20, 100, (uint8_t *)"SDRAM Initialization : OK.", LEFT_MODE);

  /* Fill the buffer to write */
  Fill_Buffer(sdram_aTxBuffer, BUFFER_SIZE, 0xA244250F);

  /* Write data to the SDRAM memory */
  if(HAL_SDRAM_Write_32b(&hsdram1, (uint32_t *)(SDRAM_DEVICE_ADDR + WRITE_READ_ADDR), (uint32_t*)sdram_aTxBuffer, BUFFER_SIZE) != DRV_ERR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : OK.", LEFT_MODE);
  }

  /* Read back data from the SDRAM memory */
  if(HAL_SDRAM_Read_32b(&hsdram1, (uint32_t *)(SDRAM_DEVICE_ADDR + WRITE_READ_ADDR), (uint32_t*)sdram_aRxBuffer, BUFFER_SIZE) != DRV_ERR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : OK.", LEFT_MODE);
  }

  if(Buffercmp(sdram_aTxBuffer, sdram_aRxBuffer, BUFFER_SIZE) > 0)
  {
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM COMPARE : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 160, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test : OK.", LEFT_MODE);
  }

}

/**
  * @brief  SDRAM DMA Demo
  * @param  None
  * @retval None
  */
void SDRAM_DMA_demo (void)
{

  SDRAM_DMA_SetHint();

  /*Let's write only for demo purposes the XferCpltCallback Directly... */
  __HAL_LINKDMA(&hsdram1, hmdma, hmdma_mdma_channel40_sw_0);   /*If Use mdma... to learn in future :) i dk why it's necessary now but without it don't working corr*/
  hmdma_mdma_channel40_sw_0.XferCpltCallback = HAL_SDRAM_DMA_XferCpltCallback;

  UTIL_LCD_DisplayStringAt(20, 100, (uint8_t *)"SDRAM Initialization : OK.", LEFT_MODE);

  /* Fill the buffer to write */
  Fill_Buffer(sdram_aTxBuffer, BUFFER_SIZE, 0xA244250F);

  /* Write data to the SDRAM memory */
  uwMDMA_Transfer_Complete = 0;

  if(HAL_SDRAM_Write_DMA(&hsdram1, (uint32_t *)(SDRAM_DEVICE_ADDR + WRITE_READ_ADDR), sdram_aTxBuffer, BUFFER_SIZE) != DRV_ERR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : OK.", LEFT_MODE);
  }
  /* Wait until DMA transfer is complete */
  while (!uwMDMA_Transfer_Complete)
  {}

  /* Read back data from the SDRAM memory */
  uwMDMA_Transfer_Complete = 0;
  if(HAL_SDRAM_Read_DMA(&hsdram1, (uint32_t *)(SDRAM_DEVICE_ADDR + WRITE_READ_ADDR), sdram_aRxBuffer, BUFFER_SIZE) != DRV_ERR_NONE)
  {
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : OK.", LEFT_MODE);
  }
  /* Wait until DMA transfer is complete */
  while (!uwMDMA_Transfer_Complete)
  {}

  if(Buffercmp(sdram_aTxBuffer, sdram_aRxBuffer, BUFFER_SIZE) > 0)
  {
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM COMPARE : FAILED.", LEFT_MODE);
    UTIL_LCD_DisplayStringAt(20, 160, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    UTIL_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test : OK.", LEFT_MODE);
  }
}
/**
  * @brief  Display SDRAM Demo Hint
  * @param  None
  * @retval None
  */
static void SDRAM_SetHint(void)
{
  uint32_t x_size, y_size;

  DISP_LCD_GetXSize(0, &x_size);
  DISP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SDRAM", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on SDRAM", CENTER_MODE);

   /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size- 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size- 102, UTIL_LCD_COLOR_BLUE);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
 }

/**
  * @brief  Display SDRAM DMA Demo Hint
  * @param  None
  * @retval None
  */
static void SDRAM_DMA_SetHint(void)
{
  uint32_t x_size, y_size;

  DISP_LCD_GetXSize(0, &x_size);
  DISP_LCD_GetYSize(0, &y_size);

  /* Clear the LCD */
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

  /* Set LCD Demo description */
  UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_SetFont(&Font24);
  UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"SDRAM DMA", CENTER_MODE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on SDRAM in DMA mode", CENTER_MODE);

  /* Set the LCD Text Color */
  UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size- 100, UTIL_LCD_COLOR_BLUE);
  UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size- 102, UTIL_LCD_COLOR_BLUE);

  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
  UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
}
/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLength; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
  /* Clean Data Cache to update the content of the SDRAM */
  /*TODO: To clarification!!!! This generate sometimes hard fault?? */
//  SCB_CleanDCache_by_Addr((uint32_t*)pBuffer, uwBufferLength*4);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  /* Invalidate Data Cache to get the updated content of the SRAM*/
	  /*TODO: To clarification!!!! This generate sometimes hard fault?? */
//  SCB_CleanInvalidateDCache_by_Addr((uint32_t *)pBuffer2, BufferLength*4);

  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  DMA conversion complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
void HAL_SDRAM_DMA_XferCpltCallback(MDMA_HandleTypeDef *hmdma)
{
  /* Set transfer complete flag */
  uwMDMA_Transfer_Complete = 1;
}

/**
  * @brief  DMA transfer complete error callback.
  * @param  hdma: DMA handle
  * @retval None
  */
void HAL_SDRAM_DMA_XferErrorCallback(MDMA_HandleTypeDef *hmdma)
{
  UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
  UTIL_LCD_DisplayStringAt(20, 215, (uint8_t *)"MDMA ERROR", LEFT_MODE);
}
