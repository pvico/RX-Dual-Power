#include "switching_logic.h"
#include "voltage_sensor.h"
#include "power_source.h"
#include "output_control.h"
#include "led.h"
#include <stdbool.h>
#include <stdint.h>
#include "gpio.h"


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

source_state  sources_state;

static bool __is_stby_powering_RX() {
    return HAL_GPIO_ReadPin(STAT_STBY_GPIO_Port, STAT_STBY_Pin) == GPIO_PIN_SET;
}

void switching_logic_loop() {
    if (main_power_source.state == OK) {
        use_main_power();
        set_led_state(LED1, STEADY_DIM);
        if (stby_power_source.state == OK) {
            sources_state = MAIN_PWR_ON_STBY_OK;
            set_led_state(LED2, OFF);
        } else {
            sources_state = MAIN_PWR_ON_STBY_NOT_OK;
            set_led_state(LED2, BLINK_FAST);
        }
    } else {        // MAIN PWR is not ok
        if (stby_power_source.state == OK) {
            use_stby_power();
            sources_state = STBY_PWR_ON;
            set_led_state(LED1, OFF);
            set_led_state(LED2, BLINK_SLOW);
        } else {    // CRITICAL situation
            let_LTCs_determine_power_source();
            sources_state = __is_stby_powering_RX() ? CRITICAL_STBY_POWERING : CRITICAL_MAIN_POWERING;
            switch (sources_state) {
            case CRITICAL_STBY_POWERING:
                set_led_state(LED1, BLINK_FAST);
                set_led_state(LED2, BLINK_SLOW);
                break;
            case CRITICAL_MAIN_POWERING:
                set_led_state(LED1, BLINK_SLOW);
                set_led_state(LED2, BLINK_FAST);
                break;            
            default:
                break;
            }
        }
    }
}