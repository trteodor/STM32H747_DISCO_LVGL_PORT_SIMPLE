/*
 * TouchC_ft6x06.c
 *
 *  Created on: May 16, 2022
 *      Author: teodor
 */

#include "TouchC_ft6x06.h"
#include "main.h"

TS_Init_t hTS;
__IO uint32_t InterruptTsFlag = 0;
TS_Gesture_Config_t GestureConf;
TS_State_t  TS_State = {0};

typedef struct
{
  uint32_t freq;       /* Frequency in Hz */
  uint32_t freq_min;   /* Minimum frequency in Hz */
  uint32_t freq_max;   /* Maximum frequency in Hz */
  uint32_t hddat_min;  /* Minimum data hold time in ns */
  uint32_t vddat_max;  /* Maximum data valid time in ns */
  uint32_t sudat_min;  /* Minimum data setup time in ns */
  uint32_t lscl_min;   /* Minimum low period of the SCL clock in ns */
  uint32_t hscl_min;   /* Minimum high period of SCL clock in ns */
  uint32_t trise;      /* Rise time in ns */
  uint32_t tfall;      /* Fall time in ns */
  uint32_t dnf;        /* Digital noise filter coefficient */
} I2C_Charac_t;

typedef struct
{
  uint32_t presc;      /* Timing prescaler */
  uint32_t tscldel;    /* SCL delay */
  uint32_t tsdadel;    /* SDA delay */
  uint32_t sclh;       /* SCL high period */
  uint32_t scll;       /* SCL low period */
} I2C_Timings_t;


I2C_HandleTypeDef *hbus_i2c;
TS_Ctx_t           Ts_Ctx[TS_INSTANCES_NBR] = {0};
void               *Ts_CompObj[TS_INSTANCES_NBR] = {0};



int32_t BSP_I2C4_DeInit(void);
int32_t TS_GetTick(void);
static int32_t I2C4_WriteReg(uint16_t DevAddr, uint16_t MemAddSize, uint16_t Reg, uint8_t *pData, uint16_t Length);
static int32_t I2C4_ReadReg(uint16_t DevAddr, uint16_t MemAddSize, uint16_t Reg, uint8_t *pData, uint16_t Length);
static int32_t FT6X06_Probe(uint32_t Instance);
TouchStateFt6x06_t BSP_TS_GetTouchPointAndState(int16_t *Xread, int16_t *Yread);
TouchIRQ_StateFt6x06_t BSP_TS_GetIT_State_OTM8009a(void);
void BSP_TS_InitIT_OTM8009a(void);

static EXTI_HandleTypeDef hts_exti[TS_INSTANCES_NBR] = {0};
static TS_Drv_t           *Ts_Drv = NULL;


void BSP_TS_InitIT_OTM8009a(void) /*Gestures are not supported yet*/
{
    uint32_t x_size, y_size;

  DISP_LCD_GetXSize(0, &x_size);
  DISP_LCD_GetYSize(0, &y_size);

  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_XY | TS_SWAP_Y;
  hTS.Accuracy = 5;

//  GestureConf.Radian = 0x0A;
//  GestureConf.OffsetLeftRight = 0x19;
//  GestureConf.OffsetUpDown = 0x19;
//  GestureConf.DistanceLeftRight = 0x19;
//  GestureConf.DistanceUpDown = 0x19;
//  GestureConf.DistanceZoom = 0x32;

  /* Touchscreen initialization */
  BSP_TS_Init(0, &hTS);
//  BSP_TS_GestureConfig(0, &GestureConf);

  BSP_TS_Init(0, &hTS);
  BSP_TS_EnableIT(0);
}

/*
 * @brief BSP_TS_GetIT_State_OTM8009a(void)
 * /*IRQ TouchCntrl flag read and reset function
 */

TouchIRQ_StateFt6x06_t BSP_TS_GetIT_State_OTM8009a(void)
{
	if(InterruptTsFlag == Touch_IRQ_FlagSet_ft6x06 )
	{
		InterruptTsFlag = Touch_IRQ_FlagReset_ft6x06; /*After read reset the flag...*/
		return Touch_Touched_ft6x06;
	}
	return Touch_IRQ_FlagReset_ft6x06;
}


TouchStateFt6x06_t BSP_TS_GetTouchPointAndState(int16_t *Xread, int16_t *Yread)
{
	  BSP_TS_GetState(0, &TS_State);

	  if(TS_State.TouchDetected)
	  {
		*Xread = TS_State.TouchX;
		*Yread = TS_State.TouchY;
		return Touch_Touched_ft6x06;
	  }
	  return Touch_Released_ft6x06;
}



int32_t BSP_TS_Init(uint32_t Instance, TS_Init_t *TS_Init)
{

    FT6X06_Probe(Instance);

      TS_Capabilities_t Capabilities;
      Ts_Ctx[Instance].Width             = TS_Init->Width;
      Ts_Ctx[Instance].Height            = TS_Init->Height;
      Ts_Ctx[Instance].Orientation       = TS_Init->Orientation;
      Ts_Ctx[Instance].Accuracy          = TS_Init->Accuracy;

      Ts_Drv->GetCapabilities(Ts_CompObj[Instance], &Capabilities);


        /* Store maximum X and Y on context */
        Ts_Ctx[Instance].MaxX = Capabilities.MaxXl;
        Ts_Ctx[Instance].MaxY = Capabilities.MaxYl;
        /* Initialize previous position in order to always detect first touch */
        for(int i = 0; i < TS_TOUCH_NBR; i++)
        {
          Ts_Ctx[Instance].PreviousX[i] = TS_Init->Width + TS_Init->Accuracy + 1U;
          Ts_Ctx[Instance].PreviousY[i] = TS_Init->Height + TS_Init->Accuracy + 1U;
        }

 return 0;
}


int32_t BSP_TS_GetCapabilities(uint32_t Instance, TS_Capabilities_t *Capabilities)
{

 (void)Ts_Drv->GetCapabilities(Ts_CompObj[Instance], Capabilities);

  return 0;
}

int32_t BSP_TS_EnableIT(uint32_t Instance)
{
  int32_t ret = DRV_ERR_NONE;
  GPIO_InitTypeDef gpio_init_structure;
//  static const uint32_t TS_EXTI_LINE[TS_INSTANCES_NBR] = {TS_INT_LINE};
  if(Instance >=TS_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }
  else
  {

    TS_INT_GPIO_CLK_ENABLE();

  /* Configure Interrupt mode for TS_INT pin falling edge : when a new touch is available */
    /* TS_INT pin is active on low level on new touch available */
    gpio_init_structure.Pin = TS_INT_PIN;
    gpio_init_structure.Pull = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Mode = GPIO_MODE_IT_FALLING;
    HAL_GPIO_Init(TS_INT_GPIO_PORT, &gpio_init_structure);

    if(Ts_Drv->EnableIT(Ts_CompObj[Instance]) < 0)
    {
      ret = DRV_ERROR_COMPONENT_FAILURE;
    }
    else
    {

      HAL_NVIC_SetPriority((IRQn_Type)(TS_INT_EXTI_IRQn), 0x05, 0x00);
      HAL_NVIC_EnableIRQ((IRQn_Type)(TS_INT_EXTI_IRQn));
      ret = DRV_ERR_NONE;
    }


  }
  return ret;
}

int32_t BSP_TS_GetState(uint32_t Instance, TS_State_t *TS_State)
{
  int32_t ret = DRV_ERR_NONE;
  UNUSED(ret);
  uint32_t x_oriented, y_oriented;
  uint32_t x_diff, y_diff;


    FT6X06_State_t state;

    /* Get each touch coordinates */
    if(Ts_Drv->GetState(Ts_CompObj[Instance], &state) < 0)
    {
      ret = DRV_ERROR_COMPONENT_FAILURE;
    }/* Check and update the number of touches active detected */
    else if(state.TouchDetected != 0U)
    {
      x_oriented = state.TouchX;
      y_oriented = state.TouchY;

      if((Ts_Ctx[Instance].Orientation & TS_SWAP_XY) == TS_SWAP_XY)
      {
        x_oriented = state.TouchY;
        y_oriented = state.TouchX;
      }

      if((Ts_Ctx[Instance].Orientation & TS_SWAP_X) == TS_SWAP_X)
      {
        x_oriented = Ts_Ctx[Instance].MaxX - x_oriented - 1UL;
      }

      if((Ts_Ctx[Instance].Orientation & TS_SWAP_Y) == TS_SWAP_Y)
      {
        y_oriented = Ts_Ctx[Instance].MaxY - y_oriented - 1UL;
      }

      /* Apply boundary */
      TS_State->TouchX = (x_oriented * Ts_Ctx[Instance].Width) / Ts_Ctx[Instance].MaxX;
      TS_State->TouchY = (y_oriented * Ts_Ctx[Instance].Height) / Ts_Ctx[Instance].MaxY;
      /* Store Current TS state */
      TS_State->TouchDetected = state.TouchDetected;

      /* Check accuracy */
      x_diff = (TS_State->TouchX > Ts_Ctx[Instance].PreviousX[0])?
               (TS_State->TouchX - Ts_Ctx[Instance].PreviousX[0]):
               (Ts_Ctx[Instance].PreviousX[0] - TS_State->TouchX);

      y_diff = (TS_State->TouchY > Ts_Ctx[Instance].PreviousY[0])?
               (TS_State->TouchY - Ts_Ctx[Instance].PreviousY[0]):
               (Ts_Ctx[Instance].PreviousY[0] - TS_State->TouchY);


      if ((x_diff > Ts_Ctx[Instance].Accuracy) || (y_diff > Ts_Ctx[Instance].Accuracy))
      {
        /* New touch detected */
        Ts_Ctx[Instance].PreviousX[0] = TS_State->TouchX;
        Ts_Ctx[Instance].PreviousY[0] = TS_State->TouchY;
      }
      else
      {
        TS_State->TouchX = Ts_Ctx[Instance].PreviousX[0];
        TS_State->TouchY = Ts_Ctx[Instance].PreviousY[0];
      }
    }
    else
    {
      TS_State->TouchDetected = 0U;
      TS_State->TouchX = Ts_Ctx[Instance].PreviousX[0];
      TS_State->TouchY = Ts_Ctx[Instance].PreviousY[0];
    }

  return ret;
}

#if (USE_TS_MULTI_TOUCH > 0)
int32_t BSP_TS_Get_MultiTouchState(uint32_t Instance, TS_MultiTouch_State_t *TS_State)
{
  uint32_t x_oriented[2], y_oriented[2];
  uint32_t x_diff, y_diff;
  uint32_t index;

    TS_MultiTouch_State_t state;
    /* Get each touch coordinates */
    if(Ts_Drv->GetMultiTouchState(Ts_CompObj[Instance], &state) < 0)
    {

    }/* Check and update the number of touches active detected */
    else if(state.TouchDetected != 0U)
    {
      for(index = 0; index < state.TouchDetected; index++)
      {
        x_oriented[index] = state.TouchX[index];
        y_oriented[index] = state.TouchY[index];

        if((Ts_Ctx[Instance].Orientation & TS_SWAP_XY) == TS_SWAP_XY)
        {
          x_oriented[index] = state.TouchY[index];
          y_oriented[index] = state.TouchX[index];
        }

        if((Ts_Ctx[Instance].Orientation & TS_SWAP_X) == TS_SWAP_X)
        {
          x_oriented[index] = Ts_Ctx[Instance].MaxX - x_oriented[index] - 1UL;
        }

        if((Ts_Ctx[Instance].Orientation & TS_SWAP_Y) == TS_SWAP_Y)
        {
          y_oriented[index] = Ts_Ctx[Instance].MaxY - y_oriented[index] - 1UL;
        }

       /* Apply boundary */
       TS_State->TouchX[index] = (x_oriented[index] * Ts_Ctx[Instance].Width) / Ts_Ctx[Instance].MaxX;
       TS_State->TouchY[index] = (y_oriented[index] * Ts_Ctx[Instance].Height) / Ts_Ctx[Instance].MaxY;
       /* Store Current TS state */
       TS_State->TouchDetected = state.TouchDetected;

        /* Check accuracy */
        x_diff = (TS_State->TouchX[index] > Ts_Ctx[Instance].PreviousX[0])?
          (TS_State->TouchX[index] - Ts_Ctx[Instance].PreviousX[0]):
          (Ts_Ctx[Instance].PreviousX[0] - TS_State->TouchX[index]);

        y_diff = (TS_State->TouchY[index] > Ts_Ctx[Instance].PreviousY[0])?
          (TS_State->TouchY[index] - Ts_Ctx[Instance].PreviousY[0]):
          (Ts_Ctx[Instance].PreviousY[0] - TS_State->TouchY[index]);

        if ((x_diff > Ts_Ctx[Instance].Accuracy) || (y_diff > Ts_Ctx[Instance].Accuracy))
        {
          /* New touch detected */
          Ts_Ctx[Instance].PreviousX[index] = TS_State->TouchX[index];
          Ts_Ctx[Instance].PreviousY[index] = TS_State->TouchY[index];
        }
        else
        {
          TS_State->TouchX[index] = Ts_Ctx[Instance].PreviousX[index];
          TS_State->TouchY[index] = Ts_Ctx[Instance].PreviousY[index];
        }
      }
    }
    else
    {
      TS_State->TouchDetected = 0U;
      for(index = 0; index < TS_TOUCH_NBR; index++)
      {
        TS_State->TouchX[index] = Ts_Ctx[Instance].PreviousX[index];
        TS_State->TouchY[index] = Ts_Ctx[Instance].PreviousY[index];
      }
    }

  return 0;
}
#endif /* USE_TS_MULTI_TOUCH > 1 */

#if (USE_TS_GESTURE > 0)

int32_t BSP_TS_GestureConfig(uint32_t Instance, TS_Gesture_Config_t *GestureConfig)
{
	Ts_Drv->GestureConfig(Ts_CompObj[Instance], GestureConfig);
	return 0;
}

int32_t BSP_TS_GetGestureId(uint32_t Instance, uint32_t *GestureId)
{
  int32_t ret = DRV_ERR_NONE;
  uint8_t tmp = 0;

  if(Instance >=TS_INSTANCES_NBR)
  {
    ret = DRV_ERROR_WRONG_PARAM;
  }/* Get gesture Id */
  else if(Ts_Drv->GetGesture(Ts_CompObj[Instance], &tmp)  < 0)
  {
    ret = DRV_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Remap gesture Id to a TS_Gesture_Id_t value */
    switch(tmp)
    {
    case FT6X06_GEST_ID_NO_GESTURE :
      *GestureId = GESTURE_ID_NO_GESTURE;
      break;
    case FT6X06_GEST_ID_MOVE_UP :
      *GestureId = GESTURE_ID_MOVE_UP;
      break;
    case FT6X06_GEST_ID_MOVE_RIGHT :
      *GestureId = GESTURE_ID_MOVE_RIGHT;
      break;
    case FT6X06_GEST_ID_MOVE_DOWN :
      *GestureId = GESTURE_ID_MOVE_DOWN;
      break;
    case FT6X06_GEST_ID_MOVE_LEFT :
      *GestureId = GESTURE_ID_MOVE_LEFT;
      break;
    case FT6X06_GEST_ID_ZOOM_IN :
      *GestureId = GESTURE_ID_ZOOM_IN;
      break;
    case FT6X06_GEST_ID_ZOOM_OUT :
      *GestureId = GESTURE_ID_ZOOM_OUT;
      break;
    default :
      *GestureId = GESTURE_ID_NO_GESTURE;
      break;
    }

    ret = DRV_ERR_NONE;
  }

  return ret;
}
#endif /* USE_TS_GESTURE > 0 */

int32_t BSP_TS_Set_Orientation(uint32_t Instance, uint32_t Orientation)
{
  Ts_Ctx[Instance].Orientation = Orientation;
  return DRV_ERR_NONE;
}

int32_t BSP_TS_Get_Orientation(uint32_t Instance, uint32_t *Orientation)
{
  *Orientation = Ts_Ctx[Instance].Orientation;
  return DRV_ERR_NONE;
}

void BSP_TS_IRQHandler(uint32_t Instance)
{
  HAL_EXTI_IRQHandler(&hts_exti[Instance]);
}

static int32_t FT6X06_Probe(uint32_t Instance)
{
  int32_t ret;
  FT6X06_IO_t              IOCtx;
  static FT6X06_Object_t   FT6X06Obj;
  FT6X06_Capabilities_t    Cap;
  uint32_t id, i;
  uint32_t const i2c_address[] = {TS_I2C_ADDRESS, TS_I2C_ADDRESS_A02};

  /* Configure the touch screen driver */
  IOCtx.Init        = BSP_I2C4_Init;
  IOCtx.DeInit      = BSP_I2C4_DeInit;
  IOCtx.ReadReg     = BSP_I2C4_ReadReg;
  IOCtx.WriteReg    = BSP_I2C4_WriteReg;
  IOCtx.GetTick     = TS_GetTick;

  for(i = 0; i < 2UL; i++)
  {
    IOCtx.Address     = (uint16_t)i2c_address[i];

    if(FT6X06_RegisterBusIO (&FT6X06Obj, &IOCtx) != FT6X06_OK)
    {
      ret = DRV_ERROR_BUS_FAILURE;
    }
    else if(FT6X06_ReadID(&FT6X06Obj, &id) != FT6X06_OK)
    {
      ret = DRV_ERROR_COMPONENT_FAILURE;
    }
    else if(id != FT6X06_ID)
    {
      ret = DRV_ERROR_UNKNOWN_COMPONENT;
    }
    else
    {

      (void)FT6X06_GetCapabilities(&FT6X06Obj, &Cap);
      Ts_CompObj[Instance] = &FT6X06Obj;
      Ts_Drv = (TS_Drv_t *) &FT6X06_TS_Driver;

      if(Ts_Drv->Init(Ts_CompObj[Instance]) != FT6X06_OK)
      {
        ret = DRV_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = DRV_ERR_NONE;
        break;
      }
    }
  }

  return ret;
}

int32_t TS_GetTick(void)
{
  return (int32_t)HAL_GetTick();
}

/*
 ****************************************************
 ****************************************************
 ****************************************************
 ****************************************************
 I2C4 Section
 */

int32_t BSP_I2C4_Init(void)
{
  int32_t ret = DRV_ERR_NONE;

  hbus_i2c = &hi2c4;

  hbus_i2c->Instance = I2C4;


    if (HAL_I2C_GetState(hbus_i2c) == HAL_I2C_STATE_RESET)
    {

        MX_I2C4_Init();
    }


  return ret;
}

int32_t BSP_I2C4_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(I2C4_WriteReg(DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length) == 0)
  {
    ret = DRV_ERR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(hbus_i2c) == HAL_I2C_ERROR_AF)
    {
    	ret =  DRV_ERROR_PERIPH_FAILURE;
    }
    else
    {
      ret =  DRV_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

int32_t BSP_I2C4_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(I2C4_ReadReg(DevAddr, Reg, I2C_MEMADD_SIZE_8BIT, pData, Length) == 0)
  {
    ret = DRV_ERR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(hbus_i2c) == HAL_I2C_ERROR_AF)
    {
    	ret =  DRV_ERROR_PERIPH_FAILURE;
    }
    else
    {
    	ret =  DRV_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

static int32_t I2C4_WriteReg(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length)
{
  if(HAL_I2C_Mem_Write(hbus_i2c, DevAddr, Reg, MemAddSize, pData, Length, 1000) == HAL_OK)
  {
    return DRV_ERR_NONE;
  }

  return DRV_ERROR_BUS_FAILURE;
}

static int32_t I2C4_ReadReg(uint16_t DevAddr, uint16_t Reg, uint16_t MemAddSize, uint8_t *pData, uint16_t Length)
{
  if (HAL_I2C_Mem_Read(hbus_i2c, DevAddr, Reg, MemAddSize, pData, Length, 1000) == HAL_OK)
  {
    return DRV_ERR_NONE;
  }

  return DRV_ERROR_BUS_FAILURE;
}


int32_t BSP_I2C4_DeInit(void)
{
  int32_t ret  = DRV_ERR_NONE;

    if (HAL_I2C_DeInit(hbus_i2c) != HAL_OK)
    {
      ret = DRV_ERROR_BUS_FAILURE;
    }

  return ret;
}

/*Touch Screen interrupt handler*/
void Touch_HandlerIRQ()
{
		  InterruptTsFlag = 1;
}






