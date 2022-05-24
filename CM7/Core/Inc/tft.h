/**
 * @file disp.h
 * 
 */

#ifndef DISP_H
#define DISP_H

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include "lvgl/lvgl.h"

#include "../Components/otm8009a/otm8009a.h"

/*********************
 *      DEFINES
 *********************/
#define TFT_HOR_RES     800
#define TFT_VER_RES     480
#define TFT_NO_TEARING  1    /*1: no tearing but slower*/

#define LCD_BL_Pin GPIO_PIN_12
#define LCD_BL_GPIO_Port GPIOJ
#define LCD_RESET_Pin GPIO_PIN_3
#define LCD_RESET_GPIO_Port GPIOG

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void tft_init_1(void);
void tft_init_2(void);

/**********************
 *      MACROS
 **********************/

#endif
