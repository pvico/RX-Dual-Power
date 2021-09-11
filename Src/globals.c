#include "switching_logic.h"
#include "power_source.h"

Power_Source main_power_source = {0};
Power_Source stby_power_source = {0};
switching_states switching_state;
volatile bool timer_flag = 0;