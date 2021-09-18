#include "initialize.h"
#include "main.h"
#include "led.h"
#include "button.h"
#include "voltage_sensor.h"
#include "output_control.h"
#include "debug_console.h"
#include "power_source.h"
#include "switching_logic.h"
#include "telemetry.h"
#include "config.h"
#include "adc_dma.h"
#include "watchdog.h"
#include "uart.h"
#include "timer.h"
#include "pin_config.h"

extern Power_Source main_power_source;
extern Power_Source stby_power_source;
extern switching_states switching_state;


  // Re-configure SWD pins if not in debug mode
static void __normal_SWD_pins_GPIO_init() {
#ifndef DEBUG_SWD_ENABLED

  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE14);
  LL_GPIO_SetPinPull(SW2_GPIO_Port, SW2_Pin, LL_GPIO_PULL_UP);
  LL_GPIO_SetPinMode(SW2_GPIO_Port, SW2_Pin, LL_GPIO_MODE_INPUT);

  // Set interrupt for SW2 pin
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_14;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);
  NVIC_SetPriority(EXTI4_15_IRQn, 0);
  NVIC_EnableIRQ(EXTI4_15_IRQn);

#endif // DEBUG_SWD_ENABLED
}


static void __init_system_clock() {
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0) {}
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1) {}
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_3, LL_RCC_PLL_DIV_3);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1) {}
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {}

  LL_Init1msTick(16000000);

  LL_SetSystemCoreClock(16000000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
}


void initialize() {
  // Reset of all peripherals, Initializes the Flash interface and the Systick.
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  __init_system_clock();
  init_watchdog();
  init_pins();

#ifdef DEBUG_SWD_ENABLED
  // Disable watchdog timer for debugging
  SET_BIT(DBGMCU->APB1FZ, DBGMCU_APB1_FZ_DBG_WWDG_STOP);
#endif

  // Enable the Systick interrupt
  LL_SYSTICK_EnableIT();

  main_power_source.state = MAIN_PWR_ON_STBY_OK;

  // Whatever event brings us here, normal power on, wathchdog reset or spurious reset due to low voltage,
  // we first put the LTC4412's in their default mode of selecting the highest voltage source to ensure
  // the model is powered up
  power_on();
  init_adc_dma();
  init_uart();
  init_timer();
  
  // Configure the SWD GPIO pin in function of debug state
  __normal_SWD_pins_GPIO_init();
  
  // debug_console_print_splash();

  if (init_buttons() == INITIALIZE_NOT_OK ||
      init_power_sources() == INITIALIZE_NOT_OK ||
      init_leds() == INITIALIZE_NOT_OK
#ifdef TELEMETRY_ENABLED
      || init_telemetry() == INITIALIZE_NOT_OK
#endif                                          
      )
  {
    leds_show_error();
    // debug_console_print_initialization_error();
    while(true) {}  // stop here
  }
  
  // debug_console_print("Initialize\r\n", 12);
}