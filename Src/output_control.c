#include "output_control.h"
#include "main.h"
#include "gpio.h"
#include <stm32l021xx.h>
#include <stdint.h>

// Only the code in this file can manipulate the LTC4412 CTL pins
// to select the power source and/or switch on/off the output
// Extra attention required when modifying this code
// CTL pin of the LTC4412, when set to high, always swicthes off
// the associated MOSFET(s)


// ########################## Local Helper functions ##########################

static void __set_CTL_pin_analog(GPIO_TypeDef *gpio, uint16_t pin) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    // other struct members remain at 0:
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    // GPIO_InitStruct.Alternate
    HAL_GPIO_Init(gpio, &GPIO_InitStruct);
}

static void __set_CTL_pin_GPIO_output(GPIO_TypeDef *gpio, uint16_t pin) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(gpio, &GPIO_InitStruct);
}

static void __put_CTL1_pin_high_impedance() {    
    __set_CTL_pin_analog(CTL1_GPIO_Port, CTL1_Pin);
}

static void __put_CTL2_pin_high_impedance() {    
    __set_CTL_pin_analog(CTL2_GPIO_Port, CTL2_Pin);
}

static void __put_CTL1_pin_GPIO_output() {    
    __set_CTL_pin_GPIO_output(CTL1_GPIO_Port, CTL1_Pin);
}

static void __put_CTL2_pin_GPIO_output() {    
    __set_CTL_pin_GPIO_output(CTL2_GPIO_Port, CTL2_Pin);
}

static void __force_main_power_off() {
    __put_CTL1_pin_GPIO_output();
    HAL_GPIO_WritePin(CTL1_GPIO_Port, CTL1_Pin, GPIO_PIN_SET);
}

static void __force_stdby_power_off() {
    __put_CTL2_pin_GPIO_output();
    HAL_GPIO_WritePin(CTL2_GPIO_Port, CTL2_Pin, GPIO_PIN_SET);
}

// ########################## Public functions ##########################

// See README for electronic circuit explanation

void use_main_power() {
    __put_CTL1_pin_high_impedance();
    __force_stdby_power_off();
}

void use_stby_power() {
    __put_CTL2_pin_high_impedance();
    __force_main_power_off();
}

void let_LTCs_determine_power_source() {
    __put_CTL1_pin_high_impedance();
    __put_CTL2_pin_high_impedance();
}

void power_off() {
    __force_main_power_off();
    __force_stdby_power_off();
}

void power_on() {
    // Initially, let the LTCs do their magic 
    // power_source_loop() will further take
    // control of power source selection
    let_LTCs_determine_power_source();
}

