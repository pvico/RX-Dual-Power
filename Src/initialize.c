#include "initialize.h"
#include "led.h"
#include "voltage_sensor.h"




void initialize() {

  led1_extinguish();
  led2_extinguish();

  init_voltage_sensors();
}