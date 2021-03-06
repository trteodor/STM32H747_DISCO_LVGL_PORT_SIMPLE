/*
 * lvglAppMain.c
 *
 *  Created on: May 17, 2022
 *      Author: teodor
 */

#include "main.h"
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
//#include "Display_OTM8009A.h"
#include "DispTest.h"


#include "lv_demos.h"

static lv_disp_draw_buf_t disp_buf;

#define DISCOH747_DISP_WIDTH 800
#define DISCOH747_DISP_HIGH 480
#define BufferDivider 1


#define SDRAM_DEVICE_ADDR         0xD0000000U

void BuffTransmitCpltCb(void);
//#ifndef TODO
/*todo: it maybe should be stored in external ram... */
static volatile lv_color_t *buf_1 = (lv_color_t*)SDRAM_DEVICE_ADDR + (2*0x240000);
static volatile lv_color_t *buf_2 = (lv_color_t*)SDRAM_DEVICE_ADDR + (3*0x240000);
//static lv_color_t buf_1[ (DISCOH747_DISP_WIDTH * DISCOH747_DISP_HIGH) /BufferDivider ] ;
//static lv_color_t buf_2[ (DISCOH747_DISP_WIDTH * DISCOH747_DISP_HIGH) /BufferDivider ] ;


static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;


/*Drivers and all necessary peripherials are initialized in main.c file*/

static lv_disp_drv_t *LastDriver;

static lv_color_t *lasColorMapPointer;

void OTM8009_flush(lv_disp_drv_t * drv, const lv_area_t * area,  lv_color_t * color_map)
{
//	LTDC_Layer1->CFBAR = (uint32_t)color_map;
//	LTDC->SRCR = LTDC_SRCR_VBR;
//	lv_disp_flush_ready(drv);


	LastDriver = drv;
	lasColorMapPointer = color_map;


	SCB_CleanDCache_by_Addr((uint32_t*)color_map, 480*800*4);
	LvglFlushBuffer((void*)color_map, area->x1, area->y1, area->x2 - area->x1 +1, area->y2 - area->y1 +1,BuffTransmitCpltCb);



}


void BuffTransmitCpltCb(void)
{
	SCB_CleanInvalidateDCache();


	if(lv_disp_flush_is_last(LastDriver)) HAL_DSI_Refresh(&hdsi);
	lv_disp_flush_ready(LastDriver);

//	lv_disp_flush_ready(LastDriver);
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



void LvglInitApp(void)
{
	  lv_init();
	  lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, (DISCOH747_DISP_WIDTH * DISCOH747_DISP_HIGH)/ BufferDivider);
	  lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
	  disp_drv.draw_buf = &disp_buf;          /*Set an initialized buffer*/
	  disp_drv.flush_cb = OTM8009_flush;        /*Set a flush callback to draw to the display*/
	  disp_drv.hor_res = LCD_DEFAULT_WIDTH;                 /*Set the horizontal resolution in pixels*/ //800
	  disp_drv.ver_res = LCD_DEFAULT_HEIGHT;                 /*Set the vertical resolution in pixels*/ //480
      lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/



      lv_indev_drv_init(&indev_drv);
      indev_drv.type =LV_INDEV_TYPE_POINTER;
      indev_drv.read_cb = TouchCntrlFt6x06_Read;
      lv_indev_drv_register(&indev_drv);
      HAL_Delay(10);

//      lv_example_get_started_1();
//      ui_init();
      lv_demo_benchmark();
//      lv_example_chart_5();
}




/*Called from main.c with period 5ms*/
void LvglProcesTask(void)
{
    lv_tick_inc(5);
    lv_task_handler();
}
