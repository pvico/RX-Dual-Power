#include "voltage_sensor.h"
#include "adc.h"

extern uint32_t adc_values[2];
extern ADC_HandleTypeDef hadc;
uint32_t adc_values[2];

void init_voltage_sensors() {
  HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
  HAL_ADC_Start_DMA(&hadc, adc_values, 2);
}

uint16_t main_voltage () {
    return adc_values[0];
}

uint16_t stby_voltage () {
    return adc_values[1];
}