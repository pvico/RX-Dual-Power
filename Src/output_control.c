#include "output_control.h"
#include "main.h"
#include "gpio.h"

// Only the code in this file can manipulate the LTC4412 CTL pins
// to select the power source and/or switch on/off the output
// Extra attention required when modifying
// CTL pin of the LTC4412, when set to high, always swicthes off
// the associated MOSFET(s)


static void __force_main_power_off() {
    HAL_GPIO_WritePin(CTL1_GPIO_Port, CTL1_Pin, GPIO_PIN_SET);
}

static void __force_stdby_power_off() {
    HAL_GPIO_WritePin(CTL2_GPIO_Port, CTL2_Pin, GPIO_PIN_SET);
}

// static bool __is_main_power_forced_off() {
//     return HAL_GPIO_ReadPin(CTL1_GPIO_Port, CTL1_Pin) == GPIO_PIN_SET;
// }

// static bool __is_stby_power_forced_off() {
//     return HAL_GPIO_ReadPin(CTL2_GPIO_Port, CTL2_Pin) == GPIO_PIN_SET;
// }

void let_LTCs_determine_power_source() {
    HAL_GPIO_WritePin(CTL1_GPIO_Port, CTL1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CTL2_GPIO_Port, CTL2_Pin, GPIO_PIN_RESET);
}

void main_power_on() {
    HAL_GPIO_WritePin(CTL1_GPIO_Port, CTL1_Pin, GPIO_PIN_RESET);
    __force_stdby_power_off();
}

void stby_power_on() {
    HAL_GPIO_WritePin(CTL2_GPIO_Port, CTL2_Pin, GPIO_PIN_RESET);
    __force_main_power_off();
}

void power_off() {
    __force_main_power_off();
    __force_stdby_power_off();
}

void power_on() {
    // initially, let the LTCs do their magic 
    // power_source_loop() will further 
    // take control of power sources
    let_LTCs_determine_power_source();
}

