#include "voltage_sensor.h"
#include "fifo.h"
#include "adc.h"
#include <stdint.h>


extern uint32_t adc_values[2];
extern ADC_HandleTypeDef hadc;
uint32_t adc_values[2];

uint16_t adc_8ms_buffer[8] = {0};


initialization_result init_voltage_sensors() {
  HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
  HAL_ADC_Start_DMA(&hadc, adc_values, 2);

  // We are creating a 8 item fifo for the 8ms average
  // To have an average over ≂ 30s and to limit ram usage,
  // we create 2 64 item... TBD

  // TODO, init fifos
  
  return INITIALIZE_NOT_OK;
}

uint16_t main_voltage () {
    return adc_values[0] + CORRECTION_VALUE;
}

uint16_t stby_voltage () {
    return adc_values[1] + CORRECTION_VALUE;
}

void __voltage_to_str(uint32_t voltage, uint8_t *buffer) {
    uint8_t units;
    uint8_t decimals;

    // Real conversion ratio is 0.0198840816
    // Rounded to 0.02  = 1/50 because we don't want any floating point
    // calculation as this would significantly increase the hex file size
    // and flash memory use
    units = voltage / 50;
    decimals = (voltage % 50) * 2;

    buffer[0] = units < 10 ? ' ' : 48 + units / 10;
    buffer[1] = 48 + units % 10;
    buffer[2] = '.';
    buffer[3] = 48 + decimals / 10;
    buffer[4] = 48 + decimals % 10;
    buffer[5] = 'V';
}

// TODO
// use an average calculation instead
void main_voltage_str (uint8_t *buffer) {
    __voltage_to_str(main_voltage(), buffer);
}

void stby_voltage_str (uint8_t *buffer) {
    __voltage_to_str(stby_voltage(), buffer);
}

void voltage_sensor_loop() {
    //TODO
}