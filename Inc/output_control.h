#ifndef B1E80BD0_6433_4F2F_91C7_1D1C20E2D39F
#define B1E80BD0_6433_4F2F_91C7_1D1C20E2D39F

#include <stdbool.h>

bool is_main_power_forced_off();
bool is_stby_power_forced_off();

void let_LTCs_determine_power_source();
void force_main_power_on();
void force_stby_power_on();

void power_off();
void power_on();

#endif /* B1E80BD0_6433_4F2F_91C7_1D1C20E2D39F */
