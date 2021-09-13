#include "main_loop.h"
#include "led.h"
#include "magnet.h"
#include "gpio.h"
#include <stm32l021xx.h>
#include "voltage_sensor.h"
#include <stdbool.h>
#include "debug_console.h"
#include "system.h"
#include "power_source.h"
#include "button.h"
#include "switching_logic.h"
#include "telemetry.h"
#include <wwdg.h>

extern volatile bool timer_flag;


void main_loop() {

  if (timer_flag) {

    // ############## 1ms loop #################

    HAL_WWDG_Refresh(&hwwdg);

    magnet_loop();
    button_loop();
    leds_loop();
    voltage_sensor_loop();
    power_source_loop();
    switching_logic_loop();
    telemetry_loop();
    system_loop();

    // ############ end 1ms loop ###############

    timer_flag = false;
  }

  // debug_console display only if CONSOLE_OUTPUT is defined in config.h
  // if (rough_quarter_second_tick()) {
  //   debug_console_print_voltages();   
  // }
    
}