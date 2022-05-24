/**
 * @file disp.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_conf.h"
#include "lvgl/lvgl.h"
#include <string.h>

#include "tft.h"
#include "stm32h7xx_hal.h"
#include "DRVs_ErrorCodes.h"
#include "TouchC_ft6x06.h"
#include "dsihost.h"
#include "ltdc.h"
#include "dma.h"

/*********************
 *      DEFINES
 *********************/

#define LCD_FB_START_ADDRESS 0xD0000000
#define ZONES               4       /*Divide the screen into zones to handle tearing effect*/
#define HACT                (OTM8009A_800X480_WIDTH / ZONES)
#define LAYER0_ADDRESS      (LCD_FB_START_ADDRESS)

/**********************
 *  STATIC PROTOTYPES
 **********************/
OTM8009A_Object_t OTM8009AObj;
OTM8009A_IO_t IOCtx;

void MX_GPIO_Init_LCD(void);
void MX_DSIHOST_DSI_Init(void);
void MX_LTDC_Init(void);
void MX_DMA_Init(void);
int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t* pData, uint16_t Size);
int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t* pData, uint16_t Size);

void Error_Handler(void);

typedef struct {
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
}__colr;


uint8_t pColLeft[] = { 0x00, 0x00, 0x01, 0x8f }; /*   0 -> 399 */
uint8_t pColRight[] = { 0x01, 0x90, 0x03, 0x1F }; /* 400 -> 799 */



static void tft_flush_cb(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p);

static void DMA_TransferComplete(DMA_HandleTypeDef *han);


static uint16_t * my_fb = (uint16_t *)LAYER0_ADDRESS;

#define __DSI_MASK_TE()   (GPIOJ->AFR[0] &= (0xFFFFF0FFU))   /* Mask DSI TearingEffect Pin*/
#define __DSI_UNMASK_TE() (GPIOJ->AFR[0] |= ((uint32_t)(GPIO_AF13_DSI) << 8)) /* UnMask DSI TearingEffect Pin*/


static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static volatile uint32_t LCD_ActiveRegion;
static volatile int32_t x1_flush;
static volatile int32_t y1_flush;
static volatile int32_t x2_flush;
static volatile int32_t y2_flush;
static volatile int32_t y_flush_act;
static volatile int32_t num_of_pixels;
static volatile const lv_color_t * buf_to_flush;

static volatile bool refr_qry;
static volatile uint32_t t_last = 0;

#if TFT_NO_TEARING
uint8_t pPage[]       = {0x00, 0x00, 0x01, 0xDF}; /*   0 -> 479 */
#endif

/* When changing these parameters, you should also change the settings of the LTDC & DSIHOST in the .ioc project
   (set the "horizontal pixles" value 800, 400 or 200 in LTDC section and "Maximum command size" in DSIHOST section) */
uint8_t pCols[4][4] =
{
    {0x00, 0x00, 0x00, 0xC7}, /*   0 -> 199 */
    {0x00, 0xC8, 0x01, 0x8F}, /* 200 -> 399 */
    {0x01, 0x90, 0x02, 0x57}, /* 400 -> 599 */
    {0x02, 0x58, 0x03, 0x1F}, /* 600 -> 799 */
};



/**********************
 *   GLOBAL FUNCTIONS
 **********************/


int32_t LCD_TFT_GetTick(void)
{
  return (int32_t)HAL_GetTick();
}

void monitor_cb(lv_disp_drv_t * d, uint32_t t, uint32_t p)
{
    t_last = t;
}

void tft_init_1(void){
	MX_GPIO_Init_LCD();

	/************************/
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);/* wait 20 ms */
	HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);/* Deactivate XRES */
	HAL_Delay(10);/* Wait for 10ms after releasing XRES before sending commands */


	__HAL_LTDC_DISABLE(&hltdc);
	DSI_LPCmdTypeDef LPCmd;

	HAL_DSI_Start(&hdsi);

	/* Configure the audio driver */
	IOCtx.Address     = 0;
	IOCtx.GetTick     = LCD_TFT_GetTick;
	IOCtx.WriteReg    = DSI_IO_Write;
	IOCtx.ReadReg     = DSI_IO_Read;
	OTM8009A_RegisterBusIO(&OTM8009AObj, &IOCtx);

	OTM8009A_Init(&OTM8009AObj ,OTM8009A_FORMAT_RBG565, OTM8009A_ORIENTATION_LANDSCAPE);
	HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPOFF, 0x00);


	LPCmd.LPDcsShortReadNoP = DSI_LP_DSR0P_DISABLE;
	LPCmd.LPDcsLongWrite = DSI_LP_DLW_DISABLE;
	HAL_DSI_ConfigCommand(&hdsi, &LPCmd);


	HAL_LTDC_SetPitch(&hltdc, 800, 0);
	__HAL_LTDC_ENABLE(&hltdc);


	/************************/
	MX_DMA_Init();
}

void TouchCntrlFt6x06_Read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
	static TouchStateFt6x06_t PreviousTouchState;

	if(BSP_TS_GetIT_State_OTM8009a() == Touch_IRQ_FlagSet_ft6x06 || PreviousTouchState == Touch_Touched_ft6x06 )
	{
		int16_t readX,readY;

		if(BSP_TS_GetTouchPointAndState(&readX,  &readY) == Touch_Touched_ft6x06)
		{
			 data->state = LV_INDEV_STATE_PRESSED;
			 data->point.x = readX;
			 data->point.y = readY;
			 PreviousTouchState = Touch_Touched_ft6x06;
		}
		else
		{
			 data->state = LV_INDEV_STATE_RELEASED;
			 data->point.x = readX;
			 data->point.y = readY;
			 PreviousTouchState = Touch_Released_ft6x06;
		}
	}
	else
	{
	    data->state = LV_INDEV_STATE_RELEASED;
	}

}



void LCD_SetUpdateRegion(int idx);


void tft_init_2(void)
{
	LCD_SetUpdateRegion(0);
	HAL_DSI_ShortWrite(&hdsi,
			0,
			DSI_DCS_SHORT_PKT_WRITE_P1,
			OTM8009A_CMD_DISPON,
			0x00);

	/*##-5- Select Callbacks functions called after Transfer complete and Transfer error */
	HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream0, HAL_DMA_XFER_CPLT_CB_ID, DMA_TransferComplete);

	static lv_color_t disp_buf1[TFT_HOR_RES * 48];
	static lv_color_t disp_buf2[TFT_HOR_RES * 48];
	static lv_disp_draw_buf_t buf;
	lv_disp_draw_buf_init(&buf, disp_buf1, disp_buf2, TFT_HOR_RES * 48);

	lv_disp_drv_init(&disp_drv);
	disp_drv.draw_buf = &buf;
	disp_drv.flush_cb = tft_flush_cb;
	disp_drv.monitor_cb = monitor_cb;
	disp_drv.hor_res = 800;
	disp_drv.ver_res = 480;
	lv_disp_drv_register(&disp_drv);


    lv_indev_drv_init(&indev_drv);
    indev_drv.type =LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = TouchCntrlFt6x06_Read;
    lv_indev_drv_register(&indev_drv);
    HAL_Delay(10);

}



/**********************
 *   STATIC FUNCTIONS
 **********************/

static void tft_flush_cb(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p)
{
//    lv_disp_flush_ready(drv);
//    return;
	SCB_CleanInvalidateDCache();

	/*Truncate the area to the screen*/
	int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
	int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
	int32_t act_x2 = area->x2 > TFT_HOR_RES - 1 ? TFT_HOR_RES - 1 : area->x2;
	int32_t act_y2 = area->y2 > TFT_VER_RES - 1 ? TFT_VER_RES - 1 : area->y2;

	x1_flush = act_x1;
	y1_flush = act_y1;
	x2_flush = act_x2;
	y2_flush = act_y2;
	y_flush_act = act_y1;
	buf_to_flush = color_p;



	HAL_StatusTypeDef err;

//	__DSI_UNMASK_TE();

  	err = HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0,(uint32_t)buf_to_flush, (uint32_t)&my_fb[y_flush_act * TFT_HOR_RES + x1_flush],
			  	  	  	   (x2_flush - x1_flush + 1));
	if(err != HAL_OK)
	{
		while(1);	/*Halt on error*/
	}
}



void LCD_SetUpdateRegion(int idx)
{
  HAL_DSI_LongWrite(&hdsi, 0, DSI_DCS_LONG_PKT_WRITE, 4, OTM8009A_CMD_CASET, pCols[idx]);
}

void HAL_DSI_TearingEffectCallback(DSI_HandleTypeDef *hdsi)
{
//
}

void HAL_DSI_EndOfRefreshCallback(DSI_HandleTypeDef *hdsi)
{
    if(LCD_ActiveRegion < ZONES )
    {
        /* Disable DSI Wrapper */
        __HAL_DSI_WRAPPER_DISABLE(hdsi);
        /* Update LTDC configuaration */
        LTDC_LAYER(&hltdc, 0)->CFBAR  = LAYER0_ADDRESS + LCD_ActiveRegion  * HACT * sizeof(lv_color_t);
        __HAL_LTDC_RELOAD_CONFIG(&hltdc);
        __HAL_DSI_WRAPPER_ENABLE(hdsi);

        LCD_SetUpdateRegion(LCD_ActiveRegion++);
        /* Refresh the right part of the display */
        HAL_DSI_Refresh(hdsi);
    }
    else
    {
        __HAL_DSI_WRAPPER_DISABLE(hdsi);
        LTDC_LAYER(&hltdc, 0)->CFBAR  = LAYER0_ADDRESS;

        __HAL_LTDC_RELOAD_CONFIG(&hltdc);
        __HAL_DSI_WRAPPER_ENABLE(hdsi);

        LCD_ActiveRegion = 0;
        LCD_SetUpdateRegion(LCD_ActiveRegion);
       // if(disp_drv.draw_buf) /*end of refresh too*/
    }
}



static void DMA_TransferComplete(DMA_HandleTypeDef *han)
{
	y_flush_act++;

	if(y_flush_act > y2_flush)
	{
		if(lv_disp_flush_is_last(&disp_drv))
			{
			HAL_DSI_Refresh(&hdsi);
			}
		lv_disp_flush_ready(&disp_drv);
	}
	else
	{
	  buf_to_flush += (x2_flush - x1_flush + 1);

	  if(HAL_DMA_Start_IT(han,(uint32_t)buf_to_flush, (uint32_t)&my_fb[y_flush_act * TFT_HOR_RES + x1_flush],
						  (x2_flush - x1_flush + 1)) != HAL_OK)
	  {
	    while(1);	//Halt on error
	  }
	}
}

int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t* pData, uint16_t Size)
 {
     int32_t ret = DRV_ERR_NONE;

     if (Size <= 1U)
     {
         if (HAL_DSI_ShortWrite(&hdsi, ChannelNbr, DSI_DCS_SHORT_PKT_WRITE_P1, Reg, (uint32_t)pData[Size]) != HAL_OK)
         {
             ret = DRV_ERROR_BUS_FAILURE;
         }
     }
     else
     {
         if (HAL_DSI_LongWrite(&hdsi, ChannelNbr, DSI_DCS_LONG_PKT_WRITE, Size, (uint32_t)Reg, pData) != HAL_OK)
         {
             ret = DRV_ERROR_BUS_FAILURE;
         }
     }

     return ret;
 }


 int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t* pData, uint16_t Size)
 {
     int32_t ret = DRV_ERR_NONE;

     if (HAL_DSI_Read(&hdsi, ChannelNbr, pData, Size, DSI_DCS_SHORT_PKT_READ, Reg, pData) != HAL_OK)
     {
         ret = DRV_ERROR_BUS_FAILURE;
     }

     return ret;
 }


 void MX_GPIO_Init_LCD(void)
 {
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   /* GPIO Ports Clock Enable */
   __HAL_RCC_GPIOG_CLK_ENABLE();
   __HAL_RCC_GPIOE_CLK_ENABLE();
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOJ_CLK_ENABLE();
   __HAL_RCC_GPIOD_CLK_ENABLE();
   __HAL_RCC_GPIOF_CLK_ENABLE();
   __HAL_RCC_GPIOB_CLK_ENABLE();


   /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);

   /*Configure GPIO pin Output Level */
   HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);

   /*Configure GPIO pin : LCD_BL_Pin */
   GPIO_InitStruct.Pin = LCD_BL_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   HAL_GPIO_Init(LCD_BL_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pin : PA8 */
   GPIO_InitStruct.Pin = GPIO_PIN_8;
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   /*Configure GPIO pin : LCD_RESET_Pin */
   GPIO_InitStruct.Pin = LCD_RESET_Pin;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   HAL_GPIO_Init(LCD_RESET_GPIO_Port, &GPIO_InitStruct);

 }

