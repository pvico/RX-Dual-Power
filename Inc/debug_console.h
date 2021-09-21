//######################################################################################
// debug_console.h
// Console output on serial telemetry port for debugging purpose
// CONSOLE_OUTPUT must be defined in config.h include the functions in the code
//
// Philippe Vico - 2021
//######################################################################################
 

#ifndef A152A863_A8B8_49EB_9B77_0984DE3C3643
#define A152A863_A8B8_49EB_9B77_0984DE3C3643


#include <stdint.h>


#ifdef CONSOLE_OUTPUT
void debug_console_print_splash();
void debug_console_print_voltages();
void debug_console_print(char *data, uint16_t num_chars);
void debug_console_print_system_status();
void debug_console_print_magnet_present();
void debug_console_print_initialization_error();
void debug_console_print_sw1_depressed();
void debug_console_print_single_activation();
void debug_console_print_double_activation();
#endif


#endif /* A152A863_A8B8_49EB_9B77_0984DE3C3643 */
