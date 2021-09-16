#ifndef AC7A9D5F_7A56_4238_9A65_B68D910343AB
#define AC7A9D5F_7A56_4238_9A65_B68D910343AB

#include "initialize.h"

typedef enum { S_PORT_TYPE, SBUS2_TYPE, X_BUS_TYPE, M_LINK_TYPE } telemetry_types;

initialization_result init_telemetry();

void telemetry_loop();

void Error_Callback();

// void telemetry_receive();

void uart_receive_byte_callback();

#endif /* AC7A9D5F_7A56_4238_9A65_B68D910343AB */
