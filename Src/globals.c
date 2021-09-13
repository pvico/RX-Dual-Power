#include "switching_logic.h"
#include "power_source.h"
#include "telemetry.h"
#include "stddef.h"

Power_Source main_power_source = {0};
Power_Source stby_power_source = {0};
switching_states switching_state;
volatile bool timer_flag = 0;
telemetry_types telemetry_type;
uint8_t* receive_buffer = NULL;
uint8_t* transmit_buffer = NULL;
volatile uint32_t IDLE_counter;