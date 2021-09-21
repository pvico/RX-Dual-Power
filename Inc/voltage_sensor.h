//######################################################################################
// voltage_sensor.h
// Determines the voltage sensed in the power sources
//
// Philippe Vico - 2021
//######################################################################################


#ifndef B99D9472_C491_40AD_BE50_51BB818EAF15
#define B99D9472_C491_40AD_BE50_51BB818EAF15


#include <stdint.h>
#include "initialize.h"


// The ratio of source voltage to the voltage sensed at ADC input =
//      resistor divider ratio = (560 + 110) / 110
//
// (Corrected) sensed voltage per ADC unit for 10 bit ADC resolution =
//      (3.3 / 1024.0) * correction coefficient
// The correction coefficient is due to adc non linearity or imprecise voltage reference
// and was measured to be 1.013
//
// The voltage of the source is the ADC number multiplied by some factor:
//      source voltage = ADC value * factor
//
// Factor = 
//      (correccted sensed voltage per ADC unit) * (ratio of source to ADC input voltage)
//      = ((3.3 / 1024) * 1.013) * ((560 + 110) / 110)
//      = 0.01988
// 0.02 or 1/50 is close enough and avoids using floating point calculation

// This define is the inverse of the multiply factor and will be used for voltage
// conversion to string
#define VOLTAGE_INVERSE_MULTIPLY_FACTOR 50

// This define is the multiply factor for millivolts and will be used for voltage
// calculation in millivolts as required by the S.PORT prorocol
#define ADC_TO_MILLIVOLTS 20        // 0.02 * 1000

// We define an additive correction value to the ADC value to approximate the correction
// coefficient and simplify calculation
#define CORRECTION_VALUE -2;


// ADC frequency is 4Mhz (HSI 16MHz, prescaler /4), period 0.25µs
// ADC conversion time: 43µs
//  - Sampling time: 160.5 cycles ≂ 40µs
//  - conversion time: 12.5 cycles ≂ 3µs
// In continuous conversion with DMA, we will have multiple conversions performed
// between each 1ms loop iteration

void voltage_sensor_loop();
uint16_t voltage_ADC_to_millivolts(uint16_t adc_value);
#ifdef CONSOLE_OUTPUT
void voltage_to_str(uint32_t voltage, uint8_t *buffer);
#endif


#endif /* B99D9472_C491_40AD_BE50_51BB818EAF15 */

