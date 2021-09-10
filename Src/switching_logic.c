#include "switching_logic.h"
#include "voltage_sensor.h"
#include "power_source.h"
#include "output_control.h"
#include "led.h"
#include <stdbool.h>
#include <stdint.h>


// Minimum voltage is the voltage at which, if the average MAIN PWR measured
// voltage was constantly below this value during 16 seconds, we will swicth
// to STBY PWR.
// The 16" are meant to average out any current spikes and thus voltage drops
// during temporary high aerodynamic loads or rapid movement of many servos.

// If, after swicthing to the STBY PWR source, MAIN PWR stays above the
// minimum voltage + HYSTERESIS_ADC_VALUE_FOR_REUSING_POWER_SOURCE for 16",
// MAIN PWR will be used again.

// If MAIN PWR measured voltage stays below the critical voltage for 16ms
// (16 loop iterations), we swicth to STBY PWR. Critical voltage is always lower
// than (or equal to for BEC) minimum voltage

// Critical condition exists when both sources are below minimum voltage.
// This includes critical voltage or disconnected status as the corresponding
// voltage levels are both below minimum voltage.

// See power_source.h for the values of minimum and critical voltage in
// function of power source type


extern Power_Source main_power_source;
extern Power_Source stby_power_source;

static uint16_t __loop_counter = 0;

inline static bool __is_in_16s_after_startup() {
    return __loop_counter <= 0x4000;
}

static bool __is_main_power_ok() {
    // MAIN PWR is ok if
    // - the 16s average voltage is above minimum voltage and the 16ms average is above critical voltage
    //                          or
    // - we are in the first 16 sec. after startup (the 16s average has not been computed yet) and
    //   the 16ms average is above critical voltage
    return !(is_power_source_below_minimum_voltage(&main_power_source, main_voltage_16s_average_adc_value()) ||
        is_power_source_below_critical_voltage(&main_power_source, main_voltage_16ms_average_adc_value())) ||
        (__is_in_16s_after_startup() && !is_power_source_below_critical_voltage(&main_power_source, main_voltage_16ms_average_adc_value()));
}

static bool __is_stby_power_ok() {
    // Same logic as for MAIN PWR
    return !(is_power_source_below_minimum_voltage(&stby_power_source, stby_voltage_16s_average_adc_value()) ||
        is_power_source_below_critical_voltage(&stby_power_source, stby_voltage_16ms_average_adc_value())) ||
        (__is_in_16s_after_startup() && !is_power_source_below_critical_voltage(&stby_power_source, main_voltage_16ms_average_adc_value()));
}

void switching_logic_loop() {
    __loop_counter++;
    if (__loop_counter == 0xffff) {
        __loop_counter = 0x4001;
    }
    // TODO HYSTERESIS

    // call functions in led.c instead of modifying global variables

    if (__is_main_power_ok()) {
        use_main_power();
        set_led_state(LED1, STEADY_DIM);
        if (__is_stby_power_ok()) {
            set_led_state(LED2, OFF);
        } else {
            set_led_state(LED2, BLINK_FAST);
        }
    } else {        // MAIN PWR is not ok
            set_led_state(LED1, BLINK_FAST);
        if (__is_stby_power_ok()) {
            use_stby_power();
            set_led_state(LED2, BLINK_SLOW);
        } else {
            let_LTCs_determine_power_source();
            set_led_state(LED2, BLINK_FAST);
        }
    }
}