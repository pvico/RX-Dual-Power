#include "initialize.h"
#include "led.h"
#include "voltage_sensor.h"
#include "tim.h"
#include "output_control.h"
#include "debug_console.h"


extern led_state led1_state;
extern led_state led2_state;

void initialize() {

  init_voltage_sensors();

  HAL_TIM_Base_Start_IT(&htim21);

  power_on();

  debug_console_print_splash();

  // // TEST
  // led1_state = STEADY_DIM;
}