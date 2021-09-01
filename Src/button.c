#include "button.h"
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "gpio.h"
#include "config.h"
#include "led.h"
#include "debug_console.h"

extern enum led_states led1_state;

static enum button_states sw1_debounced_state = BUTTON_NOT_DEPRESSED;
static enum button_states sw2_debounced_state = BUTTON_NOT_DEPRESSED;

static bool __is_sw1_depressed() {
    return HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET;
}

static bool __is_sw2_depressed() {
#ifndef DEBUG
    return HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_RESET;
#else
    return false;
#endif
}

static uint16_t __sw1_depressed__loop_counter = 0;
static uint16_t __sw1_not_depressed__loop_counter = 0;
static uint16_t __sw2_depressed__loop_counter = 0;
static uint16_t __sw2_not_depressed__loop_counter = 0;
void button_loop() {
    if (__is_sw1_depressed()) {
        // NVIC_SystemReset();
        if (sw1_debounced_state == BUTTON_NOT_DEPRESSED) {
            __sw1_depressed__loop_counter++;
            if (__sw1_depressed__loop_counter > BUTTON_DEBOUNCE_DELAY_MILLIS) {
                sw1_debounced_state = BUTTON_DEPRESSED;
                __sw1_depressed__loop_counter = 0;
            }
        }
    } else {
        // not depressed
        if (sw1_debounced_state == BUTTON_DEPRESSED) {
            __sw1_not_depressed__loop_counter++;
            if (__sw1_not_depressed__loop_counter > BUTTON_DEBOUNCE_DELAY_MILLIS) {
                sw1_debounced_state = BUTTON_NOT_DEPRESSED;
                __sw1_not_depressed__loop_counter = 0;
            }
        }
    }
    if (__is_sw2_depressed()) {
        if (sw2_debounced_state == BUTTON_NOT_DEPRESSED) {
            __sw2_depressed__loop_counter++;
            if (__sw2_depressed__loop_counter > BUTTON_DEBOUNCE_DELAY_MILLIS) {
                sw2_debounced_state = BUTTON_DEPRESSED;
                __sw2_depressed__loop_counter = 0;
            }
        }
    } else {
        // not depressed
        if (sw2_debounced_state == BUTTON_DEPRESSED) {
            __sw2_not_depressed__loop_counter++;
            if (__sw2_not_depressed__loop_counter > BUTTON_DEBOUNCE_DELAY_MILLIS) {
                sw2_debounced_state = BUTTON_NOT_DEPRESSED;
                __sw2_not_depressed__loop_counter = 0;
            }
        }
    }

    // if (sw1_debounced_state == BUTTON_DEPRESSED) {
    //     led1_state = STEADY_BRIGHT;
    //     leds_loop();
    //     // debug_console_print_system_status();
    // } else {
    //     led1_state = STEADY_DIM;
    // }
}