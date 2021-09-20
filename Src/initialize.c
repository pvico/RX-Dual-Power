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
#include "configure.h"

extern Power_Source main_power_source;
extern Power_Source stby_power_source;
extern switching_states switching_state;


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
  init_pins();

  // Whatever event brings us here, normal power on, wathchdog reset or spurious reset due to low voltage,
  // we first put the LTC4412's in their default mode of selecting the highest voltage source to ensure
  // the model is powered up
  power_on();

#ifdef DEBUG_SWD_ENABLED
  // Disable watchdog timer for debugging
  SET_BIT(DBGMCU->APB1FZ, DBGMCU_APB1_FZ_DBG_WWDG_STOP);
#endif

  init_watchdog();
  
  // Enable the Systick interrupt
  LL_SYSTICK_EnableIT();

  main_power_source.state = MAIN_PWR_ON_STBY_OK;

  init_configure();
  init_adc_dma();
  init_uart();
  init_timer();
  
  // debug_console_print_splash();

  if (init_buttons() == INITIALIZE_NOT_OK ||
      init_power_sources() == INITIALIZE_NOT_OK ||
      init_leds() == INITIALIZE_NOT_OK
#ifdef TELEMETRY_ENABLED
      || init_telemetry() == INITIALIZE_NOT_OK
#endif                                          
  ) {
    leds_show_error_infinite_loop();
    // debug_console_print_initialization_error();
  }
}