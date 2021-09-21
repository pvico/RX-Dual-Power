//######################################################################################
// led.c
//  Initialization and code for managing the LED's state
//
// Philippe Vico - 2021
//######################################################################################
 
 
#include "led.h"
#include "main.h"
#include "system.h"
#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "debug_console.h"
#include "switching_logic.h"
#include "magnet.h"
#include "configure.h"


extern switching_states switching_state;


typedef struct {
    GPIO_TypeDef *gpio_port;
    uint32_t pin;
    led_state state;
    bool illuminated;
} __Led_Hardware;


// LED1 is on the SYS_SWDIO pin and is not included in the code if debugging through SWD
#ifndef DEBUG_SWD_ENABLED
static __Led_Hardware __leds[2];
#else
static __Led_Hardware __leds[1];
#endif
 
 
//################################## Helper functions ##################################

static void __illuminate_led(led the_led) {
#ifdef DEBUG_SWD_ENABLED
    if (the_led != LED2) return;
#endif
    CLEAR_BIT((__leds[the_led].gpio_port->ODR), (__leds[the_led].pin));
    __leds[the_led].illuminated = true;
}

static void __extinguish_led(led the_led) {
#ifdef DEBUG_SWD_ENABLED
    if (the_led != LED2) return;
#endif
    SET_BIT(__leds[the_led].gpio_port->ODR, __leds[the_led].pin);
    __leds[the_led].illuminated = false;
}

static uint16_t __led_counters[2] = {0};

static void __set_leds_slow_alternate_blink_initial() {
#ifndef DEBUG_SWD_ENABLED
    __leds[LED1].illuminated = true; 
    __leds[LED2].illuminated = false;
    __led_counters[LED1] = LED_BLINK_SLOW_PERIOD_MILLIS +1; 
    __led_counters[LED2] = LED_BLINK_SLOW_PERIOD_MILLIS+1; 
#endif
}

static void __set_leds_fast_alternate_blink_initial() {
#ifndef DEBUG_SWD_ENABLED
    __leds[LED1].illuminated = true; 
    __leds[LED2].illuminated = false;
    __led_counters[LED1] = LED_BLINK_FAST_PERIOD_MILLIS +1; 
    __led_counters[LED2] = LED_BLINK_FAST_PERIOD_MILLIS+1; 
#endif
}

static void __set_leds_state_in_function_of_switching_state() {
    static  bool in_critical_state = false;
    switch (switching_state) {
    // Non critical cases
    case MAIN_PWR_ON_STBY_OK:
        in_critical_state = false;
#ifndef DEBUG_SWD_ENABLED
        set_led_state(LED1, STEADY_DIM);
#endif
        set_led_state(LED2, OFF);
        break;
    case MAIN_PWR_ON_STBY_LOW:
        in_critical_state = false;
#ifndef DEBUG_SWD_ENABLED
        set_led_state(LED1, STEADY_DIM);
#endif
        set_led_state(LED2, BLINK_SLOW);
        break;
    case STBY_PWR_ON_MAIN_LOW:
        in_critical_state = false;
#ifndef DEBUG_SWD_ENABLED
        set_led_state(LED1, BLINK_SLOW);
#endif
        set_led_state(LED2, STEADY_DIM);
        break;
    case MAIN_PWR_ON_STBY_DISCONNECT_OR_BAD_CONTACT:
        in_critical_state = false;
#ifndef DEBUG_SWD_ENABLED
        set_led_state(LED1, STEADY_DIM);
#endif
        set_led_state(LED2, BLINK_FAST);
        break;
    case STBY_PWR_ON_MAIN_DISCONNECTED_OR_BAD_CONTACT:
        in_critical_state = false;
#ifndef DEBUG_SWD_ENABLED
        set_led_state(LED1, BLINK_FAST);
#endif
        set_led_state(LED2, STEADY_DIM);
        break;
    case MAIN_PWR_ON_MAIN_BAD_CONTACT:
        in_critical_state = false;
#ifndef DEBUG_SWD_ENABLED
        set_led_state(LED1, BLINK_FAST);
#endif
        set_led_state(LED2, OFF);
        break;
    case STBY_PWR_ON_STBY_BAD_CONTACT:
        in_critical_state = false;
#ifndef DEBUG_SWD_ENABLED
        set_led_state(LED1, OFF);
#endif
        set_led_state(LED2, BLINK_FAST);
        break;
    // CRITICAL cases
    case CRITICAL_MAIN_LOW_STBY_LOW:
        if (!in_critical_state) {
            __set_leds_slow_alternate_blink_initial();
            in_critical_state = true;
        }
#ifndef DEBUG_SWD_ENABLED
        set_led_state(LED1, BLINK_SLOW);
#endif
        set_led_state(LED2, BLINK_SLOW);
        break;
    case CRITICAL_MAIN_LOW_STBY_DISCONNECT_OR_BAD_CONTACT:
        in_critical_state = true;
#ifndef DEBUG_SWD_ENABLED
        set_led_state(LED1, BLINK_SLOW);
#endif
        set_led_state(LED2, BLINK_FAST);
        break;
    case CRITICAL_STBY_LOW_MAIN_DISCONNECT_OR_BAD_CONTACT:
        in_critical_state = true;
#ifndef DEBUG_SWD_ENABLED
        set_led_state(LED1, BLINK_FAST);
#endif
        set_led_state(LED2, BLINK_SLOW);
        break;
    case CRITICAL_MAIN_DISCONNECT_OR_BAD_CONTACT_STBY_DISCONNECT_OR_BAD_CONTACT:
        if (!in_critical_state) {
            __set_leds_fast_alternate_blink_initial();
            in_critical_state = true;
        }
#ifndef DEBUG_SWD_ENABLED
        set_led_state(LED1, BLINK_FAST);
#endif
        set_led_state(LED2, BLINK_FAST);
        break;
    default:
        break;
    }

    // This one overrides the above cases
    if (is_magnet_present()) {
        set_led_state(LED2, STEADY_BRIGHT);
    }
}

static void __set_leds_in_function_of_leds_state() {
#ifdef DEBUG_SWD_ENABLED
    for (led the_led = LED2; the_led <= LED2; the_led++) {
#else
    for (led the_led = LED1; the_led <= LED2; the_led++) {
#endif
        switch (__leds[the_led].state) {
        case OFF:
            __extinguish_led(the_led);
            __led_counters[the_led] = 0;
            break;
        case STEADY_DIM:
            if (!__leds[the_led].illuminated && __led_counters[the_led] > LED_DIM_FACTOR) {
                __illuminate_led(the_led);
                __led_counters[the_led] = 0;
            } else {
                __extinguish_led(the_led);
            }                
            break;
        case STEADY_BRIGHT:
            __illuminate_led(the_led);
            __led_counters[the_led] = 0;        
            break;
        case BLINK_SLOW:
            if (!(__leds[the_led].illuminated) && (__led_counters[the_led] > LED_BLINK_SLOW_PERIOD_MILLIS)) {
                __illuminate_led(the_led);
                __led_counters[the_led] = 0;
            } else if (__leds[the_led].illuminated && (__led_counters[the_led] > LED_BLINK_SLOW_PERIOD_MILLIS)) {
                __extinguish_led(the_led);
                __led_counters[the_led] = 0;
            }                
            break;
        case BLINK_FAST:
            if (!__leds[the_led].illuminated && __led_counters[the_led] > LED_BLINK_FAST_PERIOD_MILLIS) {
                __illuminate_led(the_led);
                __led_counters[the_led] = 0;
            } else if (__leds[the_led].illuminated && __led_counters[the_led] > LED_BLINK_FAST_PERIOD_MILLIS) {
                __extinguish_led(the_led);
                __led_counters[the_led] = 0;
            }                        
            break;        
        default:
            break;
        }
        __led_counters[the_led]++;
    }
}
 
//######################################################################################
 
 
//################################ Interface functions #################################
 
initialization_result init_leds() {
#ifndef DEBUG_SWD_ENABLED
    __leds[LED1].gpio_port = LED1_GPIO_Port;
    __leds[LED1].pin = LED1_Pin;
    __leds[LED1].state = OFF;
    __leds[LED1].illuminated = false;
    __extinguish_led(LED1);
#endif
    __leds[LED2].gpio_port = LED2_GPIO_Port;
    __leds[LED2].pin = LED2_Pin;
    __leds[LED2].state = OFF;
    __leds[LED2].illuminated = false;
    __extinguish_led(LED2);

    return INITIALIZE_OK;
}

void leds_loop() {
    if (is_config_valid()) {
        __set_leds_state_in_function_of_switching_state();
        __set_leds_in_function_of_leds_state();
    } else {
        leds_show_error();
    }

    // if (rough_quarter_second_tick()) {
    //     char text[] = "   \r\n";
    //     text[0] = 48 + __leds[LED1].illuminated;
    //     text[2] = 48 + __leds[LED2].illuminated;;
    //     debug_console_print(text, sizeof(text));
    // }
}

led_state get_led_state(led the_led) {
    return __leds[the_led].state;
}

void set_led_state(led the_led, led_state state) {
    __leds[the_led].state = state;
}

void leds_show_error() {
    #ifndef DEBUG_SWD_ENABLED
    __illuminate_led(LED1);
    #endif
    __illuminate_led(LED2);
}

void leds_show_error_infinite_loop() {
    leds_show_error();
    while(1);
}

//######################################################################################