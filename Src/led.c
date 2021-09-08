
#include "gpio.h"
#include "led.h"
#include "config.h"
#include <stdbool.h>
#include "main.h"

led_state led1_state = OFF;
led_state led2_state = OFF;

static bool __led1_illuminated = false;
static bool __led2_illuminated = false;


void __led1_illuminate() {
    #ifndef DEBUG
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
    __led1_illuminated = true;
    #endif
}

void __led1_extinguish() {
    #ifndef DEBUG
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    __led1_illuminated = false;
    #endif
}

void __led2_illuminate() {
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    __led2_illuminated = true;
}

void __led2_extinguish() {
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    __led2_illuminated = false;
}

void leds_show_error() {
    __led1_illuminate();
    __led2_illuminate();
}

static uint16_t __led1_counter = 0;
static uint16_t __led2_counter = 0;

void leds_loop() {
    switch (led1_state)
    {
    case OFF:
        __led1_extinguish();
        __led1_counter = 0;
        break;
    case STEADY_DIM:
        if (!__led1_illuminated && __led1_counter > LED_DIM_FACTOR) {
            __led1_illuminate();
            __led1_counter = 0;
        } else {
            __led1_extinguish();
        }                
        break;
    case STEADY_BRIGHT:
        __led1_illuminate();
        __led1_counter = 0;        
        break;
    case BLINK_SLOW:
        if (!__led1_illuminated && __led1_counter > LED_BLINK_SLOW_PERIOD_MILLIS) {
            __led1_illuminate();
            __led1_counter = 0;
        } else if (__led1_illuminated && __led1_counter > LED_BLINK_SLOW_PERIOD_MILLIS) {
            __led1_extinguish();
            __led1_counter = 0;
        }                
        break;
    case BLINK_FAST:
        if (!__led1_illuminated && __led1_counter > LED_BLINK_FAST_PERIOD_MILLIS) {
            __led1_illuminate();
            __led1_counter = 0;
        } else if (__led1_illuminated && __led1_counter > LED_BLINK_FAST_PERIOD_MILLIS) {
            __led1_extinguish();
            __led1_counter = 0;
        }                        
        break;
    
    default:
        break;
    }
    __led1_counter++;

    switch (led2_state)
    {
    case OFF:
        __led2_extinguish();
        __led2_counter = 0;
        break;
    case STEADY_DIM:
        if (!__led2_illuminated && __led2_counter > LED_DIM_FACTOR) {
            __led2_illuminate();
            __led2_counter = 0;
        } else {
            __led2_extinguish();
        }   
        break;
    case STEADY_BRIGHT:
        __led2_illuminate();
        __led2_counter = 0;        
        break;
    case BLINK_SLOW:
        if (!__led2_illuminated && __led2_counter > LED_BLINK_SLOW_PERIOD_MILLIS) {
            __led2_illuminate();
            __led2_counter = 0;
        } else if (__led2_illuminated && __led2_counter > LED_BLINK_SLOW_PERIOD_MILLIS) {
            __led2_extinguish();
            __led2_counter = 0;
        }                
        break;
    case BLINK_FAST:
        if (!__led2_illuminated && __led2_counter > LED_BLINK_FAST_PERIOD_MILLIS) {
            __led2_illuminate();
            __led2_counter = 0;
        } else if (__led2_illuminated && __led2_counter > LED_BLINK_FAST_PERIOD_MILLIS) {
            __led2_extinguish();
            __led2_counter = 0;
        }                        
        break;
    
    default:
        break;
    }
    __led2_counter++;
}