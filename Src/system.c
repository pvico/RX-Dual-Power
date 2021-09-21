//######################################################################################
// system.c
// Determines power off activation and some timing functiopns
//
// Philippe Vico - 2021
//######################################################################################


#include "system.h"
#include "main.h"
#include "magnet.h"
#include "led.h"
#include "debug_console.h"
#include "output_control.h"
#include "uart.h"
#include "config.h"
#include "button.h"
#include "pin_config.h"


static uint16_t __first_32s__loop_counter = 0;


//################################## Helper functions ##################################

static void __system_enter_stop_mode() {
    // debug_console_print_entering_stop_mode();

    power_off();

    set_stop_mode_not_needed_gpio_pins_to_analog();
    
    LL_SYSTICK_DisableIT();

    // Disable ADC voltage regulator to reduce power in stop mode
    LL_ADC_Disable(ADC1);    // ADC must be disabled to disable internal regulator
    LL_ADC_DisableInternalRegulator(ADC1);

    // Disable GPIO clocks
    // Note: disabling GPIOA clock will not prevent the interruption
    // being fired on MAGNET and SW2 pins and disabling GPIOC clock 
    // will not change the state of the CTL pins
    LL_IOP_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOA |
                             LL_IOP_GRP1_PERIPH_GPIOB |
                             LL_IOP_GRP1_PERIPH_GPIOC);

    // Stop now
    LL_PWR_EnableUltraLowPower();
    LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
    LL_PWR_SetPowerMode(LL_PWR_MODE_STOP);
    LL_LPM_EnableDeepSleep();
    // Wait for interrupt
    __WFI();
    // in stop mode here, will resume below when EXTI interrupt

    // Out of stop mode, restart the system
    NVIC_SystemReset();
}

//######################################################################################


//################################ Interface functions #################################

void system_loop() {
    __first_32s__loop_counter++;
    if (__first_32s__loop_counter == 0xffff) {
        __first_32s__loop_counter = 0x8000;    // roll over the 32" value
    }

    if (is_magnet_double_activation_active()) {
        __system_enter_stop_mode();
    }

    if (is_button_dual_activation_active()) {
        __system_enter_stop_mode();
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

//######################################################################################
