#ifndef C2381FDA_B024_449A_9DE0_A68DD172B266
#define C2381FDA_B024_449A_9DE0_A68DD172B266

#include "power_source.h"

// Change definition of PA13 & PA14 in STM32CubeMX when you change this
// DEBUG defined:
//      Set PA13 & PA14 to SYS_SWCLK and SYS_SWDIO
// DEBUG not defined:
//      Set PA13 (label LED1) as GPIO output open drain no pullup no pulldown
//      Set PA14 (label SW2) as GPIO input pullup
// When DEBUG is defined, all code related to LED1 is deactivated
// #define DEBUG

// Undefine this one to activate the telemetry transmission
// #define CONSOLE_OUTPUT

#ifndef CONSOLE_OUTPUT
#define TELEMETRY_ENABLED
#endif // 


// The following #defines are used if no config is stored in EEPROM
// Both power sources being configured as BEC, it will work in all cases
// but it could deep discharge connected batteries

#define DEFAULT_MAIN_SOURCE_TYPE BEC
// These two are not needed for BEC
// #define DEFAULT_MAIN_BATTERY_TYPE LIFE
// #define DEFAULT_MAIN_BATTERY_NUMBER_CELLS _2S

#define DEFAULT_STBY_SOURCE_TYPE BEC
// These two are not needed for BEC
// #define DEFAULT_STBY_BATTERY_TYPE LIFE
// #define DEFAULT_STBY_BATTERY_NUMBER_CELLS _2S

#define S_PORT
// #define SBUS2
// #define X_BUS
// #define M_LINK


#endif /* C2381FDA_B024_449A_9DE0_A68DD172B266 */
