#include "voltage_sensor.h"
#include "adc.h"
#include <stdint.h>


extern uint32_t adc_values[2];
extern ADC_HandleTypeDef hadc;
uint32_t adc_values[2];

initialization_result init_voltage_sensors() {
  HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
  HAL_ADC_Start_DMA(&hadc, adc_values, 2);
  
  return INITIALIZE_OK;
}

static uint16_t __main_voltage () {
    return adc_values[0] + CORRECTION_VALUE;
}

static uint16_t __stby_voltage () {
    return adc_values[1] + CORRECTION_VALUE;
}

static void __voltage_to_str(uint32_t voltage, uint8_t *buffer) {
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

void main_voltage_str (uint8_t *buffer) {
    __voltage_to_str(main_voltage_16s_average_adc_value(), buffer);
}

void stby_voltage_str (uint8_t *buffer) {
    __voltage_to_str(stby_voltage_16s_average_adc_value(), buffer);
}

static uint16_t main_16ms_sum = 0;
static uint8_t main_16ms_counter = 0;
static uint16_t main_last_16ms_average = 0;

static uint16_t stby_16ms_sum = 0;
static uint8_t stby_16ms_counter = 0;
static uint16_t stby_last_16ms_average = 0;

static uint32_t main_16s_sum = 0;
static uint16_t main_16s_counter = 0;
static uint16_t main_last_16s_average = 0;

static uint32_t stby_16s_sum = 0;
static uint16_t stby_16s_counter = 0;
static uint16_t stby_last_16s_average = 0;

void voltage_sensor_loop() {
    main_16ms_sum += __main_voltage();
    main_16ms_counter++;
    if (main_16ms_counter == 16) {
        main_16ms_counter = 0;
        main_last_16ms_average = main_16ms_sum >> 4;    // divide by 16
        main_16ms_sum = 0;
    }
    
    stby_16ms_sum += __stby_voltage();
    stby_16ms_counter++;
    if (stby_16ms_counter == 16) {
        stby_16ms_counter = 0;
        stby_last_16ms_average = stby_16ms_sum >> 4;
        stby_16ms_sum = 0;
    }
    main_16s_sum += __main_voltage();
    main_16s_counter++;
    if (main_16s_counter == 0x4000) {       // = 16384 => 16.384 sec.
        main_16s_counter = 0;
        main_last_16s_average = main_16s_sum >> 14;      // divide by 16384
        main_16s_sum = 0;
    }
    
    stby_16s_sum += __stby_voltage();
    stby_16s_counter++;
    if (stby_16s_counter == 0x4000) {
        stby_16s_counter = 0;
        stby_last_16s_average = stby_16s_sum >> 14;
        stby_16s_sum = 0;
    }
}

inline uint16_t main_voltage_16ms_average_adc_value() {
    return main_last_16ms_average;
}

inline uint16_t main_voltage_16s_average_adc_value() {
    return main_last_16s_average;
}

inline uint16_t stby_voltage_16ms_average_adc_value() {
    return stby_last_16ms_average;
}

inline uint16_t stby_voltage_16s_average_adc_value() {
    return stby_last_16s_average;
}