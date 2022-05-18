/*
 * lvgl_HwPort.c
 *
 *  Created on: May 17, 2022
 *      Author: teodor
 */

//static lv_disp_drv_t *LastDriver;
//
//void OTM8009_flush(lv_disp_drv_t * drv, const lv_area_t * area,  lv_color_t * color_map)
//{
//	LastDriver=drv;
//
//	ILI9341_SetAddrWindowForLv((uint16_t)area->x1, (uint16_t)area->y1, (uint16_t)area->x2, (uint16_t)area->y2);
//	ILI9341_SendCommand(ILI9341_RAMWR);
//	uint32_t size = (area->x2 - area->x1 +1) * (area->y2 - area->y1 +1);
//	Send_DMA_Data16( (uint16_t *)color_map, size);
//}
//void DMA_ILI9341_SPI_TransmitComplete_Callback()
//{
//	lv_disp_flush_ready(LastDriver);
//}


//void TouchCntrlFt6x06_Read(lv_indev_drv_t * drv, lv_indev_data_t*data)
//{
//	if(TouchState==XPT2046_TOUCHED)
//	{
//
//	}
//}


