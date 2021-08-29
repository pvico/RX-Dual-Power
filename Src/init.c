#include "init.h"
#include "led.h"
#include "adc.h"


extern ADC_HandleTypeDef hadc;


void init() {
  // Extinguish both diodes
  led1_extinguish();
  led2_extinguish();

  HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
  HAL_ADC_Start(&hadc);
}