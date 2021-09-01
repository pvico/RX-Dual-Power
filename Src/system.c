#include "system.h"
#include "magnet.h"
#include "led.h"
#include "debug_console.h"
#include "main.h"
#include "output_control.h"
#include "usart.h"
#include "config.h"


extern enum led_states led1_state;
extern enum led_states led2_state;
enum led_states led1_previous_state;
enum led_states led2_previous_state;

extern UART_HandleTypeDef huart2;

void __set_not_needed_gpio_pins_to_analog() {
    // TODO
    // SW1, LED2, STAT_STBY, S.PORT, PA13 & PA14
    // The the above code for leds can be suppressed
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    GPIO_InitStruct.Pin = SW1_Pin;
    HAL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);    

    GPIO_InitStruct.Pin = LED2_Pin;
    HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);    

    GPIO_InitStruct.Pin = STAT_STBY_Pin;
    HAL_GPIO_Init(STAT_STBY_GPIO_Port, &GPIO_InitStruct);    

    #ifndef DEBUG
    GPIO_InitStruct.Pin = LED1_Pin;
    HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);    

    GPIO_InitStruct.Pin = SW2_Pin;
    HAL_GPIO_Init(SW2_GPIO_Port, &GPIO_InitStruct);    
    #endif

    HAL_UART_MspDeInit(&huart2);
}

void __set_gpio_pins_back_to_normal() {
    // TODO: put back GPIOs their needed modes
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = SW1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct); 

    GPIO_InitStruct.Pin = STAT_STBY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(STAT_STBY_GPIO_Port, &GPIO_InitStruct);    

    #ifndef DEBUG
    GPIO_InitStruct.Pin = LED1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct); 

    GPIO_InitStruct.Pin = SW2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SW2_GPIO_Port, &GPIO_InitStruct);
    #endif

    HAL_UART_MspInit(&huart2);
}

void __system_stop_mode() {
    power_off();

    led1_previous_state = led1_state;
    led2_previous_state = led2_state;
    led1_state = OFF;
    led2_state = OFF;
    leds_loop();
    
    debug_console_print_entering_stop_mode();

    __set_not_needed_gpio_pins_to_analog();

    HAL_SuspendTick();
    // Stop now
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    // in stop mode here
    SystemClock_Config();
    HAL_ResumeTick();

    __set_gpio_pins_back_to_normal();

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


