#include "system.h"
#include "main.h"
#include "magnet.h"
#include "led.h"
#include "debug_console.h"
#include "main.h"
#include "output_control.h"
#include "uart.h"
#include "config.h"
#include "button.h"


// extern UART_HandleTypeDef huart2;

static void __set_not_needed_gpio_pins_to_analog() {
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

    GPIO_InitStruct.Pin = SW1_Pin;
    LL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = LED2_Pin;
    LL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = STAT_STBY_Pin;
    LL_GPIO_Init(STAT_STBY_GPIO_Port, &GPIO_InitStruct);



#ifndef DEBUG_SWD_ENABLED
    GPIO_InitStruct.Pin = LED1_Pin;
    LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);         
#endif

    // HAL_UART_MspDeInit(&huart2);
    // TODO in LL ?
}

static void __system_stop_mode() {
    
    // debug_console_print_entering_stop_mode();

    power_off();

    __set_not_needed_gpio_pins_to_analog();
    
    LL_SYSTICK_DisableIT();

    // Stop now
    LL_PWR_EnableUltraLowPower();
    LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP);
    LL_LPM_EnableDeepSleep(); 
    __WFI();
    // in stop mode here, will resume below when EXTI interrupt

    // Out of stop mode, restart the system
    NVIC_SystemReset();
}

static uint16_t __first_32s__loop_counter = 0;

void system_loop() {
    __first_32s__loop_counter++;
    if (__first_32s__loop_counter == 0xffff) {
        __first_32s__loop_counter = 0x8000;    // roll over the 32" value
    }

    if (is_magnet_double_activation_active()) {
        __system_stop_mode();
    }

    if (is_button_dual_activation_active()) {
        __system_stop_mode();
    }
}

bool is_in_first_16s_after_startup() {
    return __first_32s__loop_counter < 0x4000;
}

bool is_in_first_32s_after_startup() {
    return __first_32s__loop_counter < 0x8000;
}

bool rough_second_tick() {
    return (__first_32s__loop_counter & 0x3FF) == 0;
}

bool rough_quarter_second_tick() {
    return (__first_32s__loop_counter & 0xFF) == 0;
}


