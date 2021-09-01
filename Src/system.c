#include "system.h"
#include "magnet.h"
#include "led.h"
#include "console.h"
#include "main.h"


extern enum led_states led1_state;
extern enum led_states led2_state;
enum led_states led1_previous_state;
enum led_states led2_previous_state;

void __system_stop_mode() {

    led1_previous_state = led1_state;
    led2_previous_state = led2_state;
    led1_state = OFF;
    led2_state = OFF;
    leds_loop();
    
    console_print_entering_stop_mode();

    HAL_SuspendTick();
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    SystemClock_Config();
    HAL_ResumeTick();
    led1_state = led1_previous_state;
    led2_state = led2_previous_state;

    console_print_exiting_stop_mode();
}

void system_loop() {
    led2_state = STEADY_DIM;
    if (is_magnet_double_activation_active()) {
        disable_magnet_double_activation();
        __system_stop_mode();
    }
}

void power_off() {

}

void power_on() {
    
}

