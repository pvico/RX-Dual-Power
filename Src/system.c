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


uint32_t SystemCoreClock = 2097152U; /* 32.768 kHz * 2^6 */

const uint8_t AHBPrescTable[16] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U};
const uint8_t APBPrescTable[8] = {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U};
const uint8_t PLLMulTable[9] = {3U, 4U, 6U, 8U, 12U, 16U, 24U, 32U, 48U};


static void __system_stop_mode() {
    // debug_console_print_entering_stop_mode();

    power_off();

    set_stop_mode_not_needed_gpio_pins_to_analog();
    
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



// This function must be there, it is called by the assembler bootup code
void SystemInit() {}