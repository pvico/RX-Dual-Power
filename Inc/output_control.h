//######################################################################################
// output_control.h
// Interface functions for changing the switching of the power sources
// 
// Only the code in the associated .c file can manipulate the LTC4412 CTL pins
// to select the power source and/or switch on/off the output
// In particular, the ONLY way to remove power to the model is to call power_off()
// defined here
//
// Philippe Vico - 2021
//######################################################################################


#ifndef B1E80BD0_6433_4F2F_91C7_1D1C20E2D39F
#define B1E80BD0_6433_4F2F_91C7_1D1C20E2D39F


#include <stdbool.h>


//These functions are called by switching_logic_loop to set the "step" of power source selection (see README)
void use_main_power();
void use_stby_power();
void let_LTCs_determine_power_source();

// These functions are called when powering on or off with magnet or buttons
void power_off();
void power_on();


#endif /* B1E80BD0_6433_4F2F_91C7_1D1C20E2D39F */
