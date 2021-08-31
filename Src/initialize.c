#include "initialize.h"
#include "led.h"
#include "voltage_sensor.h"
#include "tim.h"




void initialize() {

  init_voltage_sensors();

  HAL_TIM_Base_Start_IT(&htim21);
}