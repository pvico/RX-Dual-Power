/**
  ******************************************************************************
  * @file    wwdg.c
  * @brief   This file provides code for the configuration
  *          of the WWDG instances.
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

/* Includes ------------------------------------------------------------------*/
#include "wwdg.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* WWDG init function */
void MX_WWDG_Init(void)
{

  /* USER CODE BEGIN WWDG_Init 0 */

  /* USER CODE END WWDG_Init 0 */

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_WWDG);

  /* USER CODE BEGIN WWDG_Init 1 */

  /* USER CODE END WWDG_Init 1 */
  LL_WWDG_SetCounter(WWDG, 0x7F);
  LL_WWDG_Enable(WWDG);
  LL_WWDG_SetPrescaler(WWDG, LL_WWDG_PRESCALER_4);
  LL_WWDG_SetWindow(WWDG, 0x7F);
  /* USER CODE BEGIN WWDG_Init 2 */

  /* USER CODE END WWDG_Init 2 */

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
