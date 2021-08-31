#include "main_loop.h"
#include "led.h"
#include "magnet.h"
#include "gpio.h"
#include <stm32l021xx.h>
#include "voltage_sensor.h"
#include <stdbool.h>
#include "console.h"

extern bool timer_flag;

extern enum led_states led1_state;
extern enum led_states led2_state;


void loop() {
  // test
  led1_state = OFF;
  // led2_state = OFF;
  led2_state = STEADY_DIM;
  // led2_state = BLINK_FAST;
  // led2_state = BLINK_SLOW;
  // led2_state = STEADY_BRIGHT;

  // timer_flag is set by tim21 interrupt every ms
  if (timer_flag) {

    // ############## 1ms loop #################

    leds_loop();

    // if (is_magnet_detected()) {
    //   led1_state = STEADY_BRIGHT;
    // } else {
    //   led1_state = OFF;
    // }

    display_loop();


    // ############ end 1ms loop ###############

    timer_flag = false;
  }
}