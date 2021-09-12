#include "switching_logic.h"
#include "voltage_sensor.h"
#include "power_source.h"
#include "output_control.h"
#include "led.h"
#include <stdbool.h>
#include <stdint.h>
#include "gpio.h"
#include "system.h"
#include "debug_console.h"
#include "config.h"


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

// Critical condition exists when both sources are below minimum voltage or
// if one is considered disconnected and the other is below minimum voltage

// See power_source.h for the values of minimum and critical voltage in
// function of power source type


extern Power_Source main_power_source;
extern Power_Source stby_power_source;

extern switching_states switching_state;

static bool __is_stby_powering_RX() {
    return HAL_GPIO_ReadPin(STAT_STBY_GPIO_Port, STAT_STBY_Pin) == GPIO_PIN_SET;
}

static void __set_critical_state() {
    switching_state = __is_stby_powering_RX() ? CRITICAL_STBY_POWERING : CRITICAL_MAIN_POWERING;
}

static void __print_source_state() {
    #ifdef CONSOLE_OUTPUT
    if(rough_quarter_second_tick()) {
        char text[] = "Main:       STBY:     \r\n";
        switch (main_power_source.state) {
        case OK:
            text[6] = 'O';
            text[7] = 'K';
            break;        
        case LOW:
            text[6] = 'L';
            text[7] = 'O';
            text[8] = 'W';
            break;        
        case DISCONNECTED_OR_SHORT:
            text[6] = 'D';
            text[7] = 'I';
            text[8] = 'S';
            text[9] = 'C';
            break;        
        default:
            break;
        }
        switch (stby_power_source.state) {
        case OK:
            text[18] = 'O';
            text[19] = 'K';
            break;        
        case LOW:
            text[18] = 'L';
            text[19] = 'O';
            text[20] = 'W';
            break;        
        case DISCONNECTED_OR_SHORT:
            text[18] = 'D';
            text[19] = 'I';
            text[20] = 'S';
            text[21] = 'C';
            break;        
        default:
            break;
        }
        debug_console_print(text, 24);
    }
    #endif
}

void switching_logic_loop() {

    __print_source_state();

    static bool main_power_disconnect_flag = false;
    static bool stby_power_disconnect_flag = false;

    // Set the disconnect flags
    if (is_in_first_32s_after_startup()) {
        // In the first 32" since startup, the flags are reset if power connected again
        // This is to give time to the user to connect a second battery
        // If the user fails to connect the second battery within 32" of power up,
        // he will have to cycle power off then on after connecting the second battery
        main_power_disconnect_flag = main_power_source.state == DISCONNECTED_OR_SHORT;
        stby_power_disconnect_flag = stby_power_source.state == DISCONNECTED_OR_SHORT;
    } else {    // 32" have elapsed since startup
        // When set after 32", the flags will remain so until next system reset or power on
        // So, if a 16ms glitch brought the voltage to the disconnect level, the associated
        // source is considered disconnected for the remainder of the flight even though it
        // can still provide power if it is an intermittent connection due to bad contact
        main_power_disconnect_flag = main_power_source.state == DISCONNECTED_OR_SHORT ? true : main_power_disconnect_flag;
        stby_power_disconnect_flag = stby_power_source.state == DISCONNECTED_OR_SHORT ? true : stby_power_disconnect_flag;
    }
    // Set the switching state and switch sources
    if (main_power_disconnect_flag || stby_power_disconnect_flag) {
        // Whenever a disconnect has occurred, we let the LTC4412's manage the situation to avoid
        // the MCU switching between MAIN PWR and STBY PWR repetitively in case of bad contact
        // as this would cause annoying and confucing audio messages on the transmitter
        let_LTCs_determine_power_source();

        if (main_power_disconnect_flag) {
            if (stby_power_disconnect_flag) {           // VERY BAD: one disconnected and the other had a bad contact ? 
                __set_critical_state(); 
            } else {                                    // MAIN PWR disconnected but not STBY PWR
                if (stby_power_source.state == OK) {    // MAIN PWR disconnected and STBY PWR ok
                    switching_state = STBY_PWR_ON_MAIN_DISCONNECTED;
                } else {                                // MAIN PWR disconnected and STBY PWR low
                    __set_critical_state(); 
                }
            }
        } else {        // STBY PWR disconnected but not MAIN PWR
            let_LTCs_determine_power_source();
            if (main_power_source.state == OK) {        // STBY PWR disconnected and MAIN PWR ok
                switching_state = MAIN_PWR_ON_STBY_DISCONNECTED;
            } else {                                    // STBY PWR disconnected and MAIN PWR low
                __set_critical_state();
            }
        }
    } else {                                        
        // No source disconnection occurred so both are OK or LOW              
        if (main_power_source.state == OK) {        // MAIN PWR is ok
            use_main_power();

            if (stby_power_source.state == OK) {    // And STBY too
                switching_state = MAIN_PWR_ON_STBY_OK;
            } else {                                // STBY PWR is low
                switching_state = MAIN_PWR_ON_STBY_LOW;
            }
        } else {                                    // MAIN PWR is low
            if (stby_power_source.state == OK) {    // But STBY is ok
                use_stby_power();
                switching_state = STBY_PWR_ON_MAIN_LOW;
            } else {                                // CRITICAL: MAIN PWR and STBY PWR are low
                let_LTCs_determine_power_source();
                __set_critical_state();
            }
        }
    }
}