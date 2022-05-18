
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "TouchC_ft6x06.h"

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  CIRCLE_RADIUS        40
#define  LINE_LENGHT          30
/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)       ((i * 800) / 5)
#define  CIRCLE_YPOS(i)       (480 - CIRCLE_RADIUS - 60)
#if (USE_TS_MULTI_TOUCH == 1)
static uint32_t touchscreen_color_idx = 0;
#endif /* USE_TS_MULTI_TOUCH */

/* Private Structures and Enumerations ------------------------------------------------------------*/
/* Possible allowed indexes of touchscreen demo */
typedef enum
{
  TOUCHSCREEN_DEMO_1 = 1,
  TOUCHSCREEN_DEMO_2 = 2,
  TOUCHSCREEN_DEMO_3 = 3,
  TOUCHSCREEN_DEMO_MAX = TOUCHSCREEN_DEMO_3,

} TouchScreenDemoTypeDef;

/* Table for touchscreen event information display on LCD : table indexed on enum @ref TS_TouchEventTypeDef information */
char * ts_event_string_tab[4] = { "None",
"Press down",
"Lift up",
"Contact"};

/* Table for touchscreen gesture Id information display on LCD : table indexed on enum @ref TS_GestureIdTypeDef information */
char * ts_gesture_id_string_tab[7] = { "No Gesture",
"Move Up",
"Move Right",
"Move Down",
"Move Left",
"Zoom In",
"Zoom Out"};

/* Global variables ---------------------------------------------------------*/
uint32_t TouchScreenDemo = 0;
uint16_t x_new_pos = 0, x_previous_pos = 0;
uint16_t y_new_pos = 0, y_previous_pos = 0;

TS_MultiTouch_State_t  TS_MTState = {0};
uint32_t colors[24] = {UTIL_LCD_COLOR_BLUE, UTIL_LCD_COLOR_GREEN, UTIL_LCD_COLOR_RED, UTIL_LCD_COLOR_CYAN, UTIL_LCD_COLOR_MAGENTA, UTIL_LCD_COLOR_YELLOW,
UTIL_LCD_COLOR_LIGHTBLUE, UTIL_LCD_COLOR_LIGHTGREEN, UTIL_LCD_COLOR_LIGHTRED, UTIL_LCD_COLOR_WHITE, UTIL_LCD_COLOR_LIGHTMAGENTA,
UTIL_LCD_COLOR_LIGHTYELLOW, UTIL_LCD_COLOR_DARKBLUE, UTIL_LCD_COLOR_DARKGREEN, UTIL_LCD_COLOR_DARKRED, UTIL_LCD_COLOR_DARKCYAN,
UTIL_LCD_COLOR_DARKMAGENTA, UTIL_LCD_COLOR_DARKYELLOW, UTIL_LCD_COLOR_LIGHTGRAY, UTIL_LCD_COLOR_GRAY, UTIL_LCD_COLOR_DARKGRAY,
UTIL_LCD_COLOR_BLACK, UTIL_LCD_COLOR_BROWN, UTIL_LCD_COLOR_ORANGE };

/* Private variables ---------------------------------------------------------*/
/* Static variable holding the current touch color index : used to change color at each touch */

__IO uint32_t ButtonState = 0;
__IO uint32_t TSInterruptTest=0;
extern __IO uint32_t InterruptTsFlag;
extern TS_Init_t hTS;
extern TS_State_t  TS_State;


extern TS_Gesture_Config_t GestureConf;
//extern __IO uint32_t ButtonState;
/* Private function prototypes -----------------------------------------------*/
static void     Touchscreen_SetHint_Demo(TouchScreenDemoTypeDef demoIndex);
static void TS_Update(void);
#if (USE_TS_MULTI_TOUCH == 1)
static uint32_t Touchscreen_Handle_NewTouch(void);
#endif /* USE_TS_MULTI_TOUCH */

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Touchscreen Demo1 : test touchscreen calibration and single touch in polling mode
  * @param  None
  * @retval None
  */





void Touchscreen_demo1(void)
{
  uint16_t x1, y1;
  uint8_t state = 0;
  uint32_t ts_status = DRV_ERR_NONE;
  uint32_t x_size, y_size;

  DISP_LCD_GetXSize(0, &x_size);
  DISP_LCD_GetYSize(0, &y_size);
//  ButtonState = 0;

  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_XY | TS_SWAP_Y;
  hTS.Accuracy = 0;

  /* Touchscreen initialization */
  ts_status = BSP_TS_Init(0, &hTS);

  if(ts_status == DRV_ERR_NONE)
  {
    /* Display touch screen demo description */
    Touchscreen_SetHint_Demo(TOUCHSCREEN_DEMO_1);
    Touchscreen_DrawBackground_Circles(state);

    while (1)
    {
      /* Check in polling mode in touch screen the touch status and coordinates */
      /* of touches if touch occurred                                           */
      ts_status = BSP_TS_GetState(0, &TS_State);
      if(TS_State.TouchDetected)
      {
        /* One or dual touch have been detected          */
        /* Only take into account the first touch so far */

        /* Get X and Y position of the first touch post calibrated */
        x1 = TS_State.TouchX;
        y1 = TS_State.TouchY;

        if ((y1 > (CIRCLE_YPOS(1) - CIRCLE_RADIUS)) &&
            (y1 < (CIRCLE_YPOS(1) + CIRCLE_RADIUS)))
        {
          if ((x1 > (CIRCLE_XPOS(1) - CIRCLE_RADIUS)) &&
              (x1 < (CIRCLE_XPOS(1) + CIRCLE_RADIUS)))
          {
            if ((state & 1) == 0)
            {
              Touchscreen_DrawBackground_Circles(state);
              UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);
              state = 1;
            }
          }
          if ((x1 > (CIRCLE_XPOS(2) - CIRCLE_RADIUS)) &&
              (x1 < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
          {
            if ((state & 2) == 0)
            {
              Touchscreen_DrawBackground_Circles(state);
              UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, UTIL_LCD_COLOR_RED);
              state = 2;
            }
          }

          if ((x1 > (CIRCLE_XPOS(3) - CIRCLE_RADIUS)) &&
              (x1 < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
          {
            if ((state & 4) == 0)
            {
              Touchscreen_DrawBackground_Circles(state);
              UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_YELLOW);
              state = 4;
            }
          }

          if ((x1 > (CIRCLE_XPOS(4) - CIRCLE_RADIUS)) &&
              (x1 < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
          {
            if ((state & 8) == 0)
            {
              Touchscreen_DrawBackground_Circles(state);
              UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_GREEN);
              state = 8;
            }
          }
        }

      }
      if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_SET)
      {
      	return;
      }

      HAL_Delay(20);
    }
  }
}

#if (USE_TS_MULTI_TOUCH == 1)
/**
  * @brief  Touchscreen Demo2 : test touchscreen single and dual touch in polling mode
  * @param  None
  * @retval None
  */
void Touchscreen_demo2(void)
{
  uint32_t ts_status = DRV_ERR_NONE;
  uint32_t x_size, y_size;

  DISP_LCD_GetXSize(0, &x_size);
  DISP_LCD_GetYSize(0, &y_size);

  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_XY | TS_SWAP_Y;
  hTS.Accuracy = 5;

  GestureConf.Radian = 0x0A;
  GestureConf.OffsetLeftRight = 0x19;
  GestureConf.OffsetUpDown = 0x19;
  GestureConf.DistanceLeftRight = 0x19;
  GestureConf.DistanceUpDown = 0x19;
  GestureConf.DistanceZoom = 0x32;

  /* Touchscreen initialization */
  ts_status = BSP_TS_Init(0, &hTS);
  ts_status = BSP_TS_GestureConfig(0, &GestureConf);

  if(ts_status == DRV_ERR_NONE)
  {
    /* Display touch screen demo2 description */
    Touchscreen_SetHint_Demo(TOUCHSCREEN_DEMO_2);
  } /* of if(status == DRV_ERR_NONE) */

  while (1)
  {
    Touchscreen_Handle_NewTouch();

    if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_SET)
    {
    	return;
    }

    HAL_Delay(100);
  }
}

/*Touch screen interrupt mode test*/
void Touchscreen_demo3(void)
{
  ButtonState = 0;
  uint32_t x_size, y_size;

  DISP_LCD_GetXSize(0, &x_size);
  DISP_LCD_GetYSize(0, &y_size);

  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_XY | TS_SWAP_Y;
  hTS.Accuracy = 5;

  GestureConf.Radian = 0x0A;
  GestureConf.OffsetLeftRight = 0x19;
  GestureConf.OffsetUpDown = 0x19;
  GestureConf.DistanceLeftRight = 0x19;
  GestureConf.DistanceUpDown = 0x19;
  GestureConf.DistanceZoom = 0x32;

  /* Touchscreen initialization */
  BSP_TS_Init(0, &hTS);
  BSP_TS_GestureConfig(0, &GestureConf);

  BSP_TS_Init(0, &hTS);
  BSP_TS_EnableIT(0);
  UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
  UTIL_LCD_SetFont(&Font12);
  UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Touch screen to mark touched point", CENTER_MODE);


  while (1)
  {
	  if(InterruptTsFlag == 1)
	  {
		  InterruptTsFlag = 0;
		  TS_Update();
	  }
	    if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin) == GPIO_PIN_SET)
	    {
	        TSInterruptTest = 0;
	    	return;
	    }
  }
}

/**
  * @brief  Touchscreen_Handle_NewTouch: factorization of touch management
  * @param  None
  * @retval DRV_ERR_NONE
  */
static uint32_t Touchscreen_Handle_NewTouch(void)
{
  uint32_t GestureId = GESTURE_ID_NO_GESTURE;
#define TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS 15
#define TOUCH_INFO_STRING_SIZE                70
  uint16_t x1 = 0;
  uint16_t y1 = 0;
  uint16_t x2 = 0;
  uint16_t y2 = 0;
  uint32_t drawTouch1 = 0; /* activate/deactivate draw of footprint of touch 1 */
  uint32_t drawTouch2 = 0; /* activate/deactivate draw of footprint of touch 2 */
  uint32_t colors[24] = {UTIL_LCD_COLOR_BLUE, UTIL_LCD_COLOR_GREEN, UTIL_LCD_COLOR_RED, UTIL_LCD_COLOR_CYAN, UTIL_LCD_COLOR_MAGENTA, UTIL_LCD_COLOR_YELLOW,
  UTIL_LCD_COLOR_LIGHTBLUE, UTIL_LCD_COLOR_LIGHTGREEN, UTIL_LCD_COLOR_LIGHTRED, UTIL_LCD_COLOR_LIGHTCYAN, UTIL_LCD_COLOR_LIGHTMAGENTA,
  UTIL_LCD_COLOR_LIGHTYELLOW, UTIL_LCD_COLOR_DARKBLUE, UTIL_LCD_COLOR_DARKGREEN, UTIL_LCD_COLOR_DARKRED, UTIL_LCD_COLOR_DARKCYAN,
  UTIL_LCD_COLOR_DARKMAGENTA, UTIL_LCD_COLOR_DARKYELLOW, UTIL_LCD_COLOR_LIGHTGRAY, UTIL_LCD_COLOR_GRAY, UTIL_LCD_COLOR_DARKGRAY,
  UTIL_LCD_COLOR_BLACK, UTIL_LCD_COLOR_BROWN, UTIL_LCD_COLOR_ORANGE };
  uint32_t ts_status = DRV_ERR_NONE;
  uint8_t lcd_string[TOUCH_INFO_STRING_SIZE] = "";
  uint32_t x_size, y_size;

  DISP_LCD_GetXSize(0, &x_size);
  DISP_LCD_GetYSize(0, &y_size);

  /* Check in polling mode in touch screen the touch status and coordinates */
  /* of touches if touch occurred                                           */
  ts_status = BSP_TS_Get_MultiTouchState(0, &TS_MTState);
  if(TS_MTState.TouchDetected)
  {
    /* One or dual touch have been detected  */

    /* Erase previous information on touchscreen play pad area */
    UTIL_LCD_FillRect(0, 80, x_size, y_size - 160, UTIL_LCD_COLOR_WHITE);

    /* Re-Draw touch screen play area on LCD */
    UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size - 180, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size - 182, UTIL_LCD_COLOR_BLUE);

    /* Erase previous information on bottom text bar */
    UTIL_LCD_FillRect(0, y_size - 80, x_size, 80, UTIL_LCD_COLOR_BLUE);

    /* Desactivate drawing footprint of touch 1 and touch 2 until validated against boundaries of touch pad values */
    drawTouch1 = drawTouch2 = 0;

    /* Get X and Y position of the first touch post calibrated */
    x1 = TS_MTState.TouchX[0];
    y1 = TS_MTState.TouchY[0];

    if((y1 > (90 + TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
       (y1 < (y_size - 90 - TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)))
    {
      drawTouch1 = 1;
    }

    /* If valid touch 1 position : inside the reserved area for the use case : draw the touch */
    if(drawTouch1 == 1)
    {
      /* Draw circle of first touch : turn on colors[] table */
      UTIL_LCD_FillCircle(x1, y1, TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS, colors[(touchscreen_color_idx++ % 24)]);

      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_SetFont(&Font16);
      UTIL_LCD_DisplayStringAt(0, y_size - 70, (uint8_t *)"TOUCH INFO : ", CENTER_MODE);

      UTIL_LCD_SetFont(&Font12);
      sprintf((char*)lcd_string, "x1 = %u, y1 = %u, Event = %s, Weight = %lu",
              x1,
              y1,
              ts_event_string_tab[TS_MTState.TouchEvent[0]],
              TS_MTState.TouchWeight[0]);
      UTIL_LCD_DisplayStringAt(0, y_size - 45, lcd_string, CENTER_MODE);
    } /* of if(drawTouch1 == 1) */

    if(TS_MTState.TouchDetected > 1)
    {
      /* Get X and Y position of the second touch post calibrated */
      x2 = TS_MTState.TouchX[1];
      y2 = TS_MTState.TouchY[1];

      if((y2 > (90 + TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
         (y2 < (y_size - 90 - TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)))
      {
        drawTouch2 = 1;
      }

      /* If valid touch 2 position : inside the reserved area for the use case : draw the touch */
      if(drawTouch2 == 1)
      {
        sprintf((char*)lcd_string, "x2 = %u, y2 = %u, Event = %s, Weight = %lu",
                x2,
                y2,
                ts_event_string_tab[TS_MTState.TouchEvent[1]],
                TS_MTState.TouchWeight[1]);
        UTIL_LCD_DisplayStringAt(0, y_size - 35, lcd_string, CENTER_MODE);

        /* Draw circle of second touch : turn on color[] table */
        UTIL_LCD_FillCircle(x2, y2, TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS, colors[(touchscreen_color_idx++ % 24)]);
      } /* of if(drawTouch2 == 1) */

    } /* of if(TS_MTState.TouchDetected > 1) */

    if((drawTouch1 == 1) || (drawTouch2 == 1))
    {
      /* Get updated gesture Id in global variable 'TS_State' */
      ts_status = BSP_TS_GetGestureId(0, &GestureId);

      sprintf((char*)lcd_string, "Gesture Id = %s", ts_gesture_id_string_tab[GestureId]);
    }
    else
    {
      /* Invalid touch position */
      UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
      UTIL_LCD_DisplayStringAt(0, y_size - 70, (uint8_t *)"Invalid touch position : use drawn touch area : ", CENTER_MODE);
    }
  } /* of if(TS_MTState.TouchDetected) */

  return(ts_status);
}

#endif /* USE_TS_MULTI_TOUCH == 1 */

/**
  * @brief  Display TS Demo Hint for all touchscreen demos depending on passed
  *         demoIndex in parameter.
  * @param  demoIndex : parameter of type @ref TouchScreenDemoTypeDef
  * @retval None
  */
static void Touchscreen_SetHint_Demo(TouchScreenDemoTypeDef demoIndex)
{
  uint32_t x_size, y_size;

  DISP_LCD_GetXSize(0, &x_size);
  DISP_LCD_GetYSize(0, &y_size);

  if(demoIndex <= TOUCHSCREEN_DEMO_MAX)
  {
    /* Clear the LCD */
    UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

    /* Set Touchscreen Demo1 description */
    UTIL_LCD_FillRect(0, 0, x_size, 80, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_SetFont(&Font24);

    if(demoIndex == TOUCHSCREEN_DEMO_1)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen basic polling", CENTER_MODE);
      UTIL_LCD_SetFont(&Font12);
      UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please use the Touchscreen to", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"activate the colored circle", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"inside the rectangle. Then press User button", CENTER_MODE);
    }
    else if (demoIndex == TOUCHSCREEN_DEMO_2)
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen dual touch polling", CENTER_MODE);
      UTIL_LCD_SetFont(&Font12);
      UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please press the Touchscreen to", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"activate single and", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"dual touch", CENTER_MODE);

    }
    else /* demoIndex == TOUCHSCREEN_DEMO_3 */
    {
      UTIL_LCD_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen dual touch interrupt", CENTER_MODE);
      UTIL_LCD_SetFont(&Font12);
      UTIL_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please press the Touchscreen to", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)"activate single and", CENTER_MODE);
      UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)"dual touch", CENTER_MODE);
    }

    UTIL_LCD_DrawRect(10, 90, x_size - 20, y_size - 100, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DrawRect(11, 91, x_size - 22, y_size - 102, UTIL_LCD_COLOR_BLUE);

  } /* of if(demoIndex <= TOUCHSCREEN_DEMO_MAX) */
}

/**
  * @brief  Draw Touchscreen Background
  * @param  state : touch zone state
  * @retval None
  */
void Touchscreen_DrawBackground_Circles(uint8_t state)
{
  uint16_t x, y;
  switch (state)
  {
  case 0:
    UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);

    UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, UTIL_LCD_COLOR_RED);

    UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_YELLOW);


    UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_GREEN);

    UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    break;

  case 1:
    UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    break;

  case 2:
    UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, UTIL_LCD_COLOR_RED);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    break;

  case 4:
    UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_YELLOW);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    break;

  case 8:
    UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS, UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    break;

  case 16:
    UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLUE);

    UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);

    UTIL_LCD_DrawHLine(CIRCLE_XPOS(1)-LINE_LENGHT, CIRCLE_YPOS(1), 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DrawHLine(CIRCLE_XPOS(2)-LINE_LENGHT, CIRCLE_YPOS(2), 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DrawVLine(CIRCLE_XPOS(2), CIRCLE_YPOS(2)-LINE_LENGHT, 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DrawHLine(CIRCLE_XPOS(3)-LINE_LENGHT, CIRCLE_YPOS(3), 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DrawHLine(CIRCLE_XPOS(4)-LINE_LENGHT, CIRCLE_YPOS(4), 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_DrawVLine(CIRCLE_XPOS(4), CIRCLE_YPOS(4)-LINE_LENGHT, 2*LINE_LENGHT, UTIL_LCD_COLOR_BLUE);

    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font24);
    x = CIRCLE_XPOS(1);
    y = CIRCLE_YPOS(1) - CIRCLE_RADIUS - UTIL_LCD_GetFont()->Height;
    UTIL_LCD_DisplayStringAt(x, y, (uint8_t *)"Volume", CENTER_MODE);
    x = CIRCLE_XPOS(4);
    y = CIRCLE_YPOS(4) - CIRCLE_RADIUS - UTIL_LCD_GetFont()->Height;
    UTIL_LCD_DisplayStringAt(x, y, (uint8_t *)"Frequency", CENTER_MODE);

    break;

  case 32:
    UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS, UTIL_LCD_COLOR_BLACK);

    UTIL_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);


    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetFont(&Font20);
    x = CIRCLE_XPOS(1) - 10;
    y = CIRCLE_YPOS(1) - (UTIL_LCD_GetFont()->Height)/2;
    UTIL_LCD_DisplayStringAt(x, y, (uint8_t *)"Up", LEFT_MODE);
    x = CIRCLE_XPOS(2) - 10;
    y = CIRCLE_YPOS(3)  - (UTIL_LCD_GetFont()->Height)/2;
    UTIL_LCD_DisplayStringAt(x, y, (uint8_t *)"Dw", LEFT_MODE);
    UTIL_LCD_SetFont(&Font12);

    break;
  }
}

/**
  * @brief  TouchScreen get touch position
  * @param  None
  * @retval None
  */
uint8_t TouchScreen_GetTouchPosition(void)
{
  uint16_t x1, y1;
  uint8_t circleNr = 0;

  /* Check in polling mode in touch screen the touch status and coordinates */
  /* of touches if touch occurred                                           */
  BSP_TS_GetState(0, &TS_State);
  if(TS_State.TouchDetected)
  {
    /* One or dual touch have been detected          */
    /* Only take into account the first touch so far */

    /* Get X and Y position of the first */
    x1 = TS_State.TouchX;
    y1 = TS_State.TouchY;

    if ((y1 > (CIRCLE_YPOS(1) - CIRCLE_RADIUS)) &&
        (y1 < (CIRCLE_YPOS(1) + CIRCLE_RADIUS)))
    {
      if ((x1 > (CIRCLE_XPOS(1) - CIRCLE_RADIUS)) &&
          (x1 < (CIRCLE_XPOS(1) + CIRCLE_RADIUS)))
      {
        circleNr = 1;
      }
      if ((x1 > (CIRCLE_XPOS(2) - CIRCLE_RADIUS)) &&
          (x1 < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
      {
        circleNr = 2;
      }

      if ((x1 > (CIRCLE_XPOS(3) - CIRCLE_RADIUS)) &&
          (x1 < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
      {
        circleNr = 3;
      }

      if ((x1 > (CIRCLE_XPOS(4) - CIRCLE_RADIUS)) &&
          (x1 < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
      {
        circleNr = 4;
      }
    }
    else
    {
      if (((y1 < 220) && (y1 > 140)) &&
          ((x1 < 160) && (x1 > 100)))
      {
        circleNr = 0xFE;   /* top part of the screen */
      }
      else
      {
        circleNr = 0xFF;
      }
    }
  } /* of if(TS_State.TouchDetected) */
  return circleNr;
}


static void TS_Update(void)
{
  BSP_TS_GetState(0, &TS_State);
  if(TS_State.TouchDetected)
  {
    /* One or dual touch have been detected          */
    /* Only take into account the first touch so far */

    /* Get X and Y position of the first touch post calibrated */
    x_new_pos = TS_State.TouchX;
    y_new_pos = TS_State.TouchY;

	if(x_new_pos )
	{
	  UTIL_LCD_FillRect(x_new_pos, y_new_pos,10,10, UTIL_LCD_COLOR_CYAN);

	  x_previous_pos = x_new_pos;
	  y_previous_pos = y_new_pos;
	}
  }
}



