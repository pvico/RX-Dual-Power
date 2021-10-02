//######################################################################################
// initialize.c
// All initialization before entering the main loop
//
// Philippe Vico - 2021
//######################################################################################


#include "initialize.h"
#include "main.h"
#include "system_clock.h"
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
#include "system.h"


extern Power_Source main_power_source;
extern Power_Source stby_power_source;
extern switching_states switching_state;


//################################## Public functions ##################################

void initialize() {
  // Reset of all peripherals, Initializes the Flash interface and the Systick.
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  init_system_clock();
  init_pins();

  // Whatever event brings us here, normal power on, wathchdog reset or spurious reset due to low voltage,
  // we first put the LTC4412's in their default mode of selecting the highest voltage source to ensure
  // the model is powered up
  power_on();


  init_uart();    // UART is now half-duplex 9600bds 8N1
  // Enable the Systick interrupt
  LL_SYSTICK_EnableIT();

  check_serial_configuration(); 
  init_configure();     // Not sure this is needed

  init_watchdog();
#ifdef DEBUG_SWD_ENABLED
  // Disable watchdog timer for debugging
  SET_BIT(DBGMCU->APB1FZ, DBGMCU_APB1_FZ_DBG_WWDG_STOP);
#endif
  
  init_adc_dma();
  init_timer();
  
  #ifdef CONSOLE_OUTPUT
  debug_console_print_splash();
  #endif

  if (init_buttons() == INITIALIZE_NOT_OK ||
      init_power_sources() == INITIALIZE_NOT_OK ||
      init_leds() == INITIALIZE_NOT_OK
#ifdef TELEMETRY_ENABLED
      || init_telemetry() == INITIALIZE_NOT_OK
#endif                                          
  ) {
    leds_show_error_infinite_loop();
    #ifdef CONSOLE_OUTPUT
    debug_console_print_initialization_error();
    #endif
  } else {

  }
}

//######################################################################################
