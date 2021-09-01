#include "system.h"
#include "magnet.h"
#include "led.h"
#include "debug_console.h"
#include "main.h"
#include "output_control.h"


extern enum led_states led1_state;
extern enum led_states led2_state;
enum led_states led1_previous_state;
enum led_states led2_previous_state;

void __system_stop_mode() {
    power_off();

    led1_previous_state = led1_state;
    led2_previous_state = led2_state;
    led1_state = OFF;
    led2_state = OFF;
    leds_loop();
    
    debug_console_print_entering_stop_mode();

    HAL_SuspendTick();
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    // in stop mode here
    SystemClock_Config();
    HAL_ResumeTick();
    led1_state = led1_previous_state;
    led2_state = led2_previous_state;
    power_on();
    debug_console_print_exiting_stop_mode();
}

void system_loop() {
    if (is_magnet_double_activation_active()) {
        disable_magnet_double_activation();
        __system_stop_mode();
    }
}


