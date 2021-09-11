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

  // Whatever event brings us here, normal power on, wathchdog reset or spurious reset due to low voltage,
  // we first put the LTC4412's in their default mode of selecting the highest voltage source to ensure
  // the model is powered up
  power_on();

  debug_console_print_splash();

  HAL_TIM_Base_Start_IT(&htim21);

  if (init_voltage_sensors() == INITIALIZE_NOT_OK || 
      init_power_sources() == INITIALIZE_NOT_OK ||
      init_leds() == INITIALIZE_NOT_OK)
  {
    leds_show_error();
    debug_console_print_initialization_error();
    while(true) {}  // stop here
  }
}