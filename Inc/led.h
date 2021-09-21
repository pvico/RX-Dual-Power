//######################################################################################
// led.h
//  Initialization and loop code for managing the LED's state
//
// Philippe Vico - 2021
//######################################################################################
 

#ifndef D86274A9_C095_49D9_9A72_98E31E11064A
#define D86274A9_C095_49D9_9A72_98E31E11064A


#include <stm32l021xx.h>
#include "initialize.h"
#include "config.h"


#define LED_BLINK_SLOW_PERIOD_MILLIS    1000
#define LED_BLINK_FAST_PERIOD_MILLIS    50
#define LED_DIM_FACTOR  10


typedef enum led_states {OFF, STEADY_DIM, STEADY_BRIGHT, BLINK_SLOW, BLINK_FAST} led_state;
#ifndef DEBUG_SWD_ENABLED
typedef enum leds {LED1, LED2} led;
#else
typedef enum leds {LED2} led;
#endif


initialization_result init_leds();
void leds_loop();

void set_led_state(led the_led, led_state state);
led_state get_led_state(led the_led);

void leds_show_error();
void leds_show_error_infinite_loop();


#endif /* D86274A9_C095_49D9_9A72_98E31E11064A */
