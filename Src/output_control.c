//######################################################################################
// output_control.c
// Interface functions for changing the switching of the power sources
// 
// Only the code in this file can manipulate the LTC4412 CTL pins
// to select the power source and/or switch on/off the output
// In particular, the ONLY way to remove power to the model is to call power_off()
// defined here
// 
// Extra attention required when modifying this code!!!
// 
// CTL pin of the LTC4412, when set to high, always swicthes off
// the associated MOSFET(s)
// See README for electronic circuit details
//
// Philippe Vico - 2021
//######################################################################################
 

#include "output_control.h"
#include "main.h"
#include "pin_config.h"
#include <stdint.h>

 
//################################## Helper functions ##################################

static void __put_CTL1_pin_high_impedance() {    
    set_pin_to_analog(CTL1_GPIO_Port, CTL1_Pin);
}

static void __put_CTL2_pin_high_impedance() {    
    set_pin_to_analog(CTL2_GPIO_Port, CTL2_Pin);
}

static void __put_CTL1_pin_GPIO_output() {    
    set_pin_to_output_PP_no_pull(CTL1_GPIO_Port, CTL1_Pin);
}

static void __put_CTL2_pin_GPIO_output() {    
    set_pin_to_output_PP_no_pull(CTL2_GPIO_Port, CTL2_Pin);
}

static void __force_main_power_off() {
    __put_CTL1_pin_GPIO_output();
    LL_GPIO_SetOutputPin(CTL1_GPIO_Port, CTL1_Pin);
}

static void __force_stdby_power_off() {
    __put_CTL2_pin_GPIO_output();
    LL_GPIO_SetOutputPin(CTL2_GPIO_Port, CTL2_Pin);
}
 
//######################################################################################
 
 
//################################ Interface functions #################################

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
 
//######################################################################################


