//######################################################################################
// button.c
// Intitialization and loop code for buttons and managament of button state 
//
// Philippe Vico - 2021
//######################################################################################


#include "button.h"
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "led.h"
#include "debug_console.h"
#include <stdlib.h>


extern button *buttons;
static bool __dual_activation_state = false;


//################################## Helper functions ##################################

static bool __is_sw1_depressed() {
    return !(SW1_GPIO_Port->IDR & SW1_Pin);
}

static bool __is_sw2_depressed() {
#ifndef DEBUG_SWD_ENABLED
    return !(SW2_GPIO_Port->IDR & SW2_Pin);
#else
    return false;
#endif
}

//######################################################################################


//################################ Interface functions #################################

initialization_result init_buttons() {
    buttons = malloc(2 * sizeof(button));
    if (buttons == NULL) {
        return INITIALIZE_NOT_OK;
    }
    for (button_name name = SW1; name <= SW2; name++) {
        buttons[name].debounced_state = BUTTON_NOT_DEPRESSED;
        buttons[name].depressed_loop_counter = 0;
        buttons[name].not_depressed_loop_counter = 0;
    }
    buttons[SW1].is_depressed = __is_sw1_depressed;
    buttons[SW2].is_depressed = __is_sw2_depressed;
    
    return INITIALIZE_OK;
}

void button_loop() {
    for (button_name name = SW1; name <= SW2; name++) {
        if (buttons[name].is_depressed()) {
            buttons[name].not_depressed_loop_counter = 0;
            if (buttons[name].debounced_state == BUTTON_NOT_DEPRESSED) {
                buttons[name].depressed_loop_counter++;
                if (buttons[name].depressed_loop_counter > BUTTON_DEBOUNCE_DELAY_MILLIS) {
                    buttons[name].debounced_state = BUTTON_DEPRESSED;
                    buttons[name].depressed_loop_counter = 0;
                }
            }
        } else {                // not depressed            
            buttons[name].depressed_loop_counter = 0;
            if (buttons[name].debounced_state == BUTTON_DEPRESSED) {
                buttons[name].not_depressed_loop_counter++;
                if (buttons[name].not_depressed_loop_counter > BUTTON_DEBOUNCE_DELAY_MILLIS) {
                    buttons[name].debounced_state = BUTTON_NOT_DEPRESSED;
                    buttons[name].not_depressed_loop_counter = 0;
                }
            }
        }
    }

    static uint16_t __double_activation__loop_counter = 0;

    if (buttons[SW1].debounced_state == BUTTON_DEPRESSED &&
        buttons[SW2].debounced_state == BUTTON_DEPRESSED) {
        __double_activation__loop_counter++;
        if (__double_activation__loop_counter > BUTTON_DOUBLE_ACTIVATION_MILLIS) {
            __dual_activation_state = true;
        }
    } else {
        __double_activation__loop_counter = 0; 
        __dual_activation_state = false;       
    } 
}

// NOTE: does not work if programming clip is installed
bool is_button_dual_activation_active() {
    return __dual_activation_state;
}

//######################################################################################
