/*
 * DispTest.h
 *
 *  Created on: May 19, 2022
 *      Author: teodor
 */

#ifndef INC_DISPTEST_H_
#define INC_DISPTEST_H_

void LCD_Task(void);

void LCD_OTM8009a_InitFull(void);



void LvglFlushBuffer(uint32_t *pixelMap, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize,void(*DMAtrEndCb)(void));

#endif /* INC_DISPTEST_H_ */
