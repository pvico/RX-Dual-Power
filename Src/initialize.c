#include "initialize.h"
#include "led.h"
#include "voltage_sensor.h"
#include "tim.h"
#include "output_control.h"



void initialize() {

  init_voltage_sensors();

  HAL_TIM_Base_Start_IT(&htim21);

  void power_on();
}