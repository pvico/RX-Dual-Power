
#include "gpio.h"
#include "led.h"
#include "config.h"
#include <stdbool.h>
#include <stdint.h>
#include "main.h"


typedef struct {
    GPIO_TypeDef *gpio_port;
    uint16_t pin;
    led_state state;
    bool illuminated;
} __Led_Hardware;

#ifndef DEBUG
static __Led_Hardware __leds[2];
#else
static __Led_Hardware __leds[1];
#endif


static void __illuminate_led(led the_led) {
    #ifdef DEBUG
    if (the_led != LED2) return;
    #endif
    HAL_GPIO_WritePin(__leds[the_led].gpio_port, __leds[the_led].pin, GPIO_PIN_RESET);
    __leds[the_led].illuminated = true;
}

static void __extinguish_led(led the_led) {
    #ifdef DEBUG
    if (the_led != LED2) return;
    #endif
    HAL_GPIO_WritePin(__leds[the_led].gpio_port, __leds[the_led].pin, GPIO_PIN_SET);
    __leds[the_led].illuminated = false;
}

initialization_result init_leds() {
    #ifndef DEBUG
    __leds[LED1].gpio_port = LED1_GPIO_Port;
    __leds[LED1].pin = LED1_Pin;
    __leds[LED1].state = OFF;
    __leds[LED1].illuminated = false;
    #endif
    __leds[LED2].gpio_port = LED2_GPIO_Port;
    __leds[LED2].pin = LED2_Pin;
    __leds[LED2].state = OFF;
    __leds[LED2].illuminated = false;

    return INITIALIZE_OK;
}

void leds_show_error() {
    __illuminate_led(LED1);
    __illuminate_led(LED2);
}

inline led_state get_led_state(led the_led) {
    return __leds[the_led].state;
}

inline void set_led_state(led the_led, led_state state) {
    __leds[the_led].state = state;
}


static uint16_t __led_counters[2] = {0};

void leds_loop() {
    #ifdef DEBUG
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
            if (!__leds[the_led].illuminated && __led_counters[the_led] > LED_BLINK_SLOW_PERIOD_MILLIS) {
                __illuminate_led(the_led);
                __led_counters[the_led] = 0;
            } else if (__leds[the_led].illuminated && __led_counters[the_led] > LED_BLINK_SLOW_PERIOD_MILLIS) {
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