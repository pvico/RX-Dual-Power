#include "main_loop.h"
#include "led.h"
#include "magnet.h"
#include "gpio.h"
#include <stm32l021xx.h>
#include "voltage_sensor.h"
#include <stdbool.h>
#include "console.h"
#include "system.h"

extern bool timer_flag;


void loop() {

  if (timer_flag) {

    // ############## 1ms loop #################

    magnet_loop();
    leds_loop();
    console_display_loop();   // no console display if DEBUG is not defined in config.h
    system_loop();    // must be last ?

    // ############ end 1ms loop ###############

    timer_flag = false;
  }
}