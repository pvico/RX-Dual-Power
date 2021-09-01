#include "system.h"
#include "magnet.h"
#include "led.h"
#include "debug_console.h"
#include "main.h"
#include "output_control.h"
#include "usart.h"
#include "config.h"
#include "button.h"


extern UART_HandleTypeDef huart2;

void __set_not_needed_gpio_pins_to_analog() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    GPIO_InitStruct.Pin = SW1_Pin;
    HAL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);    

    GPIO_InitStruct.Pin = LED2_Pin;
    HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);    

    GPIO_InitStruct.Pin = STAT_STBY_Pin;
    HAL_GPIO_Init(STAT_STBY_GPIO_Port, &GPIO_InitStruct);    

    GPIO_InitStruct.Pin = LED1_Pin;
    HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);      

    HAL_UART_MspDeInit(&huart2);
}

void __system_stop_mode() {
    power_off();
    
    debug_console_print_entering_stop_mode();

    __set_not_needed_gpio_pins_to_analog();

    HAL_SuspendTick();
    // Stop now
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    // in stop mode here, will resume below when EXTI interrupt

    // Out of stop mode, restart the system
    NVIC_SystemReset();
}

void system_loop() {
    if (is_magnet_double_activation_active()) {
        __system_stop_mode();
    }

    if (is_button_double_activation_active()) {
        __system_stop_mode();
    }
}


