#include "initialize.h"
#include "led.h"
#include "voltage_sensor.h"
#include "tim.h"
#include "output_control.h"
#include "debug_console.h"
#include "power_source.h"
#include <wwdg.h>


void initialize() {

  #ifdef DEBUG
  __HAL_DBGMCU_FREEZE_WWDG();
  __HAL_DBGMCU_FREEZE_IWDG();
  #endif

  debug_console_print_splash();

  HAL_TIM_Base_Start_IT(&htim21);

  if (init_voltage_sensors() == INITIALIZE_OK && 
      init_power_sources() == INITIALIZE_OK &&
      init_leds() == INITIALIZE_OK) {
      power_on();
    } else {
      power_off();
      leds_show_error();
      debug_console_print_initialization_error();
      while(true) {}  // stop here
    }
}