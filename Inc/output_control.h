#ifndef B1E80BD0_6433_4F2F_91C7_1D1C20E2D39F
#define B1E80BD0_6433_4F2F_91C7_1D1C20E2D39F

#include <stdbool.h>

// bool is_main_power_forced_off();
// bool is_stby_power_forced_off();

//These functions are called by power_source_loop to set the "step" of power source selection (see README)
void main_power_on();
void stby_power_on();
void let_LTCs_determine_power_source();

// These functions are called when powering on or off with magnet or button
void power_off();
void power_on();

#endif /* B1E80BD0_6433_4F2F_91C7_1D1C20E2D39F */
