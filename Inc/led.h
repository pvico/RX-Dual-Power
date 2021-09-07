#ifndef D86274A9_C095_49D9_9A72_98E31E11064A
#define D86274A9_C095_49D9_9A72_98E31E11064A

#include <stm32l021xx.h>

#define LED_BLINK_SLOW_PERIOD_MILLIS    1000
#define LED_BLINK_FAST_PERIOD_MILLIS    50

#define LED_DIM_FACTOR  10

typedef enum led_states {OFF, STEADY_DIM, STEADY_BRIGHT, BLINK_SLOW, BLINK_FAST} led_state;

void leds_loop();

#endif /* D86274A9_C095_49D9_9A72_98E31E11064A */
