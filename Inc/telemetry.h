#ifndef AC7A9D5F_7A56_4238_9A65_B68D910343AB
#define AC7A9D5F_7A56_4238_9A65_B68D910343AB

#include "initialize.h"

typedef enum {S_PORT, SBUS2, X_BUS, M_LINK} telemetry_types;

initialization_result init_telemetry();

void telemetry_loop();

// void telemetry_receive();

#endif /* AC7A9D5F_7A56_4238_9A65_B68D910343AB */
