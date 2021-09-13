#include "button.h"
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "gpio.h"
#include "config.h"
#include "led.h"
#include "debug_console.h"


static bool __dual_activation_state = false;

static button_state __sw1_debounced_state = BUTTON_NOT_DEPRESSED;
static button_state __sw2_debounced_state = BUTTON_NOT_DEPRESSED;

static bool __is_sw1_depressed() {
    return HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET;
}

static bool __is_sw2_depressed() {
#ifndef DEBUG_SWD_ENABLED
    return HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_RESET;
#else
    return false;
#endif
}

static uint16_t __sw1_depressed__loop_counter = 0;
static uint16_t __sw1_not_depressed__loop_counter = 0;
static uint16_t __sw2_depressed__loop_counter = 0;
static uint16_t __sw2_not_depressed__loop_counter = 0;
static uint16_t __double_activation__loop_counter = 0;
void button_loop() {
    if (__is_sw1_depressed()) {
        __sw1_not_depressed__loop_counter = 0;
        if (__sw1_debounced_state == BUTTON_NOT_DEPRESSED) {
            __sw1_depressed__loop_counter++;
            if (__sw1_depressed__loop_counter > BUTTON_DEBOUNCE_DELAY_MILLIS) {
                __sw1_debounced_state = BUTTON_DEPRESSED;
                __sw1_depressed__loop_counter = 0;
            }
        }
    } else {
        // not depressed
        __sw1_depressed__loop_counter = 0;
        if (__sw1_debounced_state == BUTTON_DEPRESSED) {
            __sw1_not_depressed__loop_counter++;
            if (__sw1_not_depressed__loop_counter > BUTTON_DEBOUNCE_DELAY_MILLIS) {
                __sw1_debounced_state = BUTTON_NOT_DEPRESSED;
                __sw1_not_depressed__loop_counter = 0;
            }
        }
    }
    if (__is_sw2_depressed()) {
        __sw2_not_depressed__loop_counter = 0;
        if (__sw2_debounced_state == BUTTON_NOT_DEPRESSED) {
            __sw2_depressed__loop_counter++;
            if (__sw2_depressed__loop_counter > BUTTON_DEBOUNCE_DELAY_MILLIS) {
                __sw2_debounced_state = BUTTON_DEPRESSED;
                __sw2_depressed__loop_counter = 0;
            }
        }
    } else {
        // not depressed
        __sw2_depressed__loop_counter = 0;
        if (__sw2_debounced_state == BUTTON_DEPRESSED) {
            __sw2_not_depressed__loop_counter++;
            if (__sw2_not_depressed__loop_counter > BUTTON_DEBOUNCE_DELAY_MILLIS) {
                __sw2_debounced_state = BUTTON_NOT_DEPRESSED;
                __sw2_not_depressed__loop_counter = 0;
            }
        }
    }

    if (__sw1_debounced_state == BUTTON_DEPRESSED && 
            __sw2_debounced_state == BUTTON_DEPRESSED) {
        __double_activation__loop_counter++;
        if (__double_activation__loop_counter > BUTTON_DOUBLE_ACTIVATION_MILLIS) {
            __dual_activation_state = true;
        }
    } else {
        __double_activation__loop_counter = 0; 
        __dual_activation_state = false;       
    }
}


bool is_button_dual_activation_active() {
    return __dual_activation_state;
}
