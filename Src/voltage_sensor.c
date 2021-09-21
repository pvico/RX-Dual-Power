//######################################################################################
// voltage_sensor.c
// Determines the voltage sensed in the power sources
//
// Philippe Vico - 2021
//######################################################################################


#include "voltage_sensor.h"
#include "main.h"
#include "power_source.h"
#include "led.h"
#include <stdint.h>


extern Power_Source main_power_source;
extern Power_Source stby_power_source;

extern uint32_t adc_values[];


//################################## Helper functions ##################################

static uint16_t __main_voltage() {
    return adc_values[0] + CORRECTION_VALUE;
}

static uint16_t __stby_voltage() {
    return adc_values[1] + CORRECTION_VALUE;
}

//######################################################################################


//################################ Interface functions #################################

void voltage_sensor_loop() {
    uint16_t main_voltage = __main_voltage();
    uint16_t stby_voltage = __stby_voltage();

    main_power_source.current_voltage_ADC_value = main_voltage;
    stby_power_source.current_voltage_ADC_value = stby_voltage;

    static uint16_t main_16ms_sum = 0;
    static uint8_t main_16ms_counter = 0;

    main_16ms_sum += main_voltage;
    main_16ms_counter++;
    if (main_16ms_counter == 16) {
        main_16ms_counter = 0;
        main_power_source.last_16ms_average_voltage_ADC_value = main_16ms_sum >> 4;    // divide by 16
        main_16ms_sum = 0;
    }

    static uint16_t stby_16ms_sum = 0;
    static uint8_t stby_16ms_counter = 0;

    stby_16ms_sum += stby_voltage;
    stby_16ms_counter++;
    if (stby_16ms_counter == 16) {
        stby_16ms_counter = 0;
        stby_power_source.last_16ms_average_voltage_ADC_value = stby_16ms_sum >> 4;
        stby_16ms_sum = 0;
    }
    
    static uint32_t main_16s_sum = 0;
    static uint16_t main_16s_counter = 0;

    main_16s_sum += main_voltage;
    main_16s_counter++;
    if (main_16s_counter == 0x4000) {       // = 16384 => 16.384 sec.
        main_16s_counter = 0;
        main_power_source.last_16s_average_voltage_ADC_value = main_16s_sum >> 14;      // divide by 16384
        main_16s_sum = 0;
    }

    static uint32_t stby_16s_sum = 0;
    static uint16_t stby_16s_counter = 0;
    
    stby_16s_sum += stby_voltage;
    stby_16s_counter++;
    if (stby_16s_counter == 0x4000) {
        stby_16s_counter = 0;
        stby_power_source.last_16s_average_voltage_ADC_value = stby_16s_sum >> 14;
        stby_16s_sum = 0;
    }
}

uint16_t voltage_ADC_to_millivolts(uint16_t adc_value) {
    return  ADC_TO_MILLIVOLTS * adc_value;
}

#ifdef CONSOLE_OUTPUT
void voltage_to_str(uint32_t voltage, uint8_t *buffer) {
    uint8_t units;
    uint8_t decimals;

    // Real conversion ratio is 0.0198840816
    // Rounded to 0.02  = 1/50 because we don't want any floating point
    // calculation as this would significantly increase the hex file size
    // and flash memory use
    units = voltage / VOLTAGE_INVERSE_MULTIPLY_FACTOR;
    decimals = (voltage % VOLTAGE_INVERSE_MULTIPLY_FACTOR) * 2;

    buffer[0] = units < 10 ? ' ' : 48 + units / 10;
    buffer[1] = 48 + units % 10;
    buffer[2] = '.';
    buffer[3] = 48 + decimals / 10;
    buffer[4] = 48 + decimals % 10;
    buffer[5] = 'V';
}
#endif

//######################################################################################
