//######################################################################################
// main_loop.h
// Configuration defines, to enable debugging through SWD or debug console output and
// to define default configuration if no configuration is stored in EEPROM
//
// Philippe Vico - 2021
//######################################################################################


#ifndef C2381FDA_B024_449A_9DE0_A68DD172B266
#define C2381FDA_B024_449A_9DE0_A68DD172B266

#include "power_source.h"


// Debugging defines

// When DEBUG_SWD_ENABLED is defined, all code related to LED1 and SW2 is deactivated
// as LED1 and SW2 are on the SWD pins
// #define DEBUG_SWD_ENABLED

// Undefine CONSOLE_OUTPUT to activate the telemetry transmission
// #define CONSOLE_OUTPUT

#ifndef CONSOLE_OUTPUT
#define TELEMETRY_ENABLED
#endif // 

// These defines should disappear if the protocol is set in stored configuration
// or if it can be determined automatically from the telemetry tramsmission (TBD)
#define S_PORT
// #define SBUS2
// #define X_BUS
// #define M_LINK


// The following #defines are used if no config is stored in EEPROM
// Both power sources being configured as BEC, it will work in all cases
// but it could deep discharge connected batteries

#define DEFAULT_MAIN_SOURCE_TYPE BEC
// These two are not needed for BEC
#define DEFAULT_MAIN_BATTERY_TYPE LIFE
#define DEFAULT_MAIN_BATTERY_NUMBER_CELLS _2S

#define DEFAULT_STBY_SOURCE_TYPE BEC
// These two are not needed for BEC
#define DEFAULT_STBY_BATTERY_TYPE LIFE
#define DEFAULT_STBY_BATTERY_NUMBER_CELLS _2S


#endif /* C2381FDA_B024_449A_9DE0_A68DD172B266 */
