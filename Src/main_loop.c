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

extern bool timer_flag;


void main_loop() {

  if (timer_flag) {

    // ############## 1ms loop #################

    magnet_loop();
    button_loop();
    leds_loop();
    debug_console_display_loop();   // no debug_console display if CONSOLE_OUTPUT is not defined in config.h
    power_source_loop();
    system_loop();

    // ############ end 1ms loop ###############

    timer_flag = false;
  }
}