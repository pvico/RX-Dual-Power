//######################################################################################
// main.h
// Core headers includes and system wide defines
//
// Philippe Vico - 2021
//######################################################################################


#ifndef __MAIN_H
#define __MAIN_H


#include "stm32l0xx_ll_adc.h"
#include "stm32l0xx_ll_dma.h"
#include "stm32l0xx_ll_crs.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_wwdg.h"
#include "stm32l0xx_ll_gpio.h"


#define CTL1_Pin LL_GPIO_PIN_14
#define CTL1_GPIO_Port GPIOC
#define CTL2_Pin LL_GPIO_PIN_15
#define CTL2_GPIO_Port GPIOC
#define CTL_GPIO_Port GPIOC
#define MAGNET_Pin LL_GPIO_PIN_0
#define MAGNET_GPIO_Port GPIOA
#define MAGNET_EXTI_IRQn EXTI0_1_IRQn
#define V_MAIN_Pin LL_GPIO_PIN_1
#define V_MAIN_GPIO_Port GPIOA
#define V_STBY_Pin LL_GPIO_PIN_4
#define V_STBY_GPIO_Port GPIOA
#define V_MEASURE_GPIO_Port GPIOA
#define S_PORT_Pin LL_GPIO_PIN_9
#define S_PORT_GPIO_Port GPIOA
#define STAT_STBY_Pin LL_GPIO_PIN_10
#define STAT_STBY_GPIO_Port GPIOA
#define LED1_Pin LL_GPIO_PIN_13
#define LED1_GPIO_Port GPIOA
#define LED2_Pin LL_GPIO_PIN_7
#define LED2_GPIO_Port GPIOA
#define LEDS_GPIO_Port GPIOA
#define SW1_Pin LL_GPIO_PIN_9
#define SW1_GPIO_Port GPIOB
#define SW2_Pin LL_GPIO_PIN_14
#define SW2_GPIO_Port GPIOA


#endif /* __MAIN_H */

