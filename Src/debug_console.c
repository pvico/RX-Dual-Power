#include "debug_console.h"
#include "uart.h"
#include "voltage_sensor.h"
#include "power_source.h"
#include "config.h"
#include "system.h"


extern Power_Source main_power_source;
extern Power_Source stby_power_source;

// #ifdef CONSOLE_OUTPUT
// static void __transmit_string(uint8_t *text, uint16_t num_chars) {
//     uint16_t index = 0;

//     while (index < num_chars) {
//         while (!LL_USART_IsActiveFlag_TXE(USART2)); // wait for end of transmission of last byte

//         if (index == (num_chars-1)) {          // If last char to be sent, clear TC flag
//             LL_USART_ClearFlag_TC(USART2);
//         }
//         LL_USART_TransmitData8(USART2, text[index++]);
//     }

//     while (!LL_USART_IsActiveFlag_TC(USART2));      // Wait for TC flag to be raised for last char
// }
// #endif

// void debug_console_print_voltages() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t text[] = "MAIN: ______\tSTBY: ______\r\n";
//   voltage_to_str(main_power_source.last_16ms_average_voltage_ADC_value, text+6);
//   voltage_to_str(stby_power_source.last_16ms_average_voltage_ADC_value, text+19);
//   __transmit_string(text, 27);
//   #endif
// }

// void debug_console_print(char *text, uint16_t num_chars) {
//   #ifdef CONSOLE_OUTPUT 
//   __transmit_string((uint8_t*)text, num_chars);
//   #endif
// }

// void debug_console_print_magnet_present() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t text[] = "MAGNET PRESENT\r\n";
//   __transmit_string(text, sizeof(text)-1);   
//   #endif
// }

// void debug_console_print_single_activation() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t text[] = "SINGLE ACTIVATION\r\n";
//   __transmit_string(text, sizeof(text)-1);   
//   #endif
// }

// void debug_console_print_double_activation() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t text[] = "DOUBLE ACTIVATION\r\n";
//   __transmit_string(text, sizeof(text)-1);   
//   #endif
// }

// void debug_console_print_sw1_depressed() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t text[] = "SW1 DEPRESSED\r\n";
//   __transmit_string(text, sizeof(text)-1);   
//   #endif
// }

// void debug_console_print_initialization_error() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t text[] = "\r\nInitialization error. Stopped.\r\n";
//   __transmit_string(text, sizeof(text)-1);   
//   #endif
// }

// void debug_console_print_splash() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t text[] = "\r\nRX Dual Power\r\nPhilippe Vico - 2021\r\n";
// //   __transmit_string(text);   
//     __transmit_string(text, sizeof(text)-1);
//   #endif
// }