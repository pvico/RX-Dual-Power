//######################################################################################
// globals.c
// All global variables
//
// Philippe Vico - 2021
//######################################################################################
 
 
#include "switching_logic.h"
#include "power_source.h"
#include "telemetry.h"
#include "stddef.h"
#include "button.h"


Power_Source main_power_source = {0};
Power_Source stby_power_source = {0};
switching_states switching_state;
volatile bool timer_flag = 0;
volatile bool requested_to_transmit_data = false;
bool uart_in_config_mode = false;
telemetry_types telemetry_type;
uint8_t* receive_buffer = NULL;
uint8_t* transmit_buffer = NULL;
button *buttons;
uint32_t adc_values[2] = {0};
volatile uint32_t system_tick = 0;   // updated every 1ms, it will roll over after ± 50days
