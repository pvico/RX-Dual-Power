//######################################################################################
// timer.c
// Timer(s) initializaton
//
// Philippe Vico - 2021
//######################################################################################


#include "timer.h"
#include "main.h"


//################################ Interface functions #################################

void init_timer() {
  LL_TIM_InitTypeDef TIM_InitStruct = {0};

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM21);

  NVIC_SetPriority(TIM21_IRQn, 1);
  NVIC_EnableIRQ(TIM21_IRQn);

  TIM_InitStruct.Prescaler = 15;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 220;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM21, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM21);
  LL_TIM_SetClockSource(TIM21, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIM21, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIM21);
}

//######################################################################################
