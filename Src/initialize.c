#include "initialize.h"
#include "led.h"
#include "button.h"
#include "voltage_sensor.h"
#include "output_control.h"
#include "debug_console.h"
#include "power_source.h"
#include "switching_logic.h"
#include "telemetry.h"
#include "config.h"
#include <gpio.h>
#include "adc_dma.h"
#include "watchdog.h"
#include "uart.h"

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


void initialize() {
init_watchdog();

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