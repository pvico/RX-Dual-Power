//######################################################################################
// telemetry.h
// Generic telemetry initialization, loop and callbacks. Switches to the appropriate
// protocol functions
//
// Philippe Vico - 2021
//######################################################################################


#ifndef AC7A9D5F_7A56_4238_9A65_B68D910343AB
#define AC7A9D5F_7A56_4238_9A65_B68D910343AB


#include "initialize.h"


typedef enum { S_PORT_TYPE, SBUS2_TYPE, X_BUS_TYPE, M_LINK_TYPE, EX_BUS_TYPE, IBUS_TYPE } telemetry_types;


initialization_result init_telemetry();
void telemetry_loop();
void uart_receive_byte_callback();


#endif /* AC7A9D5F_7A56_4238_9A65_B68D910343AB */
