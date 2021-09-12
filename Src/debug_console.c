#include "debug_console.h"
#include "usart.h"
#include "voltage_sensor.h"
#include "power_source.h"
#include "config.h"
#include "system.h"


extern Power_Source main_power_source;
extern Power_Source stby_power_source;

void debug_console_print_voltages() {
  // #ifdef CONSOLE_OUTPUT
  uint8_t data[] = "MAIN: ______\tSTBY: ______\r\n";
  voltage_to_str(main_power_source.last_16ms_average_voltage_ADC_value, data+6);
  voltage_to_str(stby_power_source.last_16ms_average_voltage_ADC_value, data+19);
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10); 
  // #endif
}

void debug_console_display_loop() {
  #ifdef CONSOLE_OUTPUT
  // static uint16_t __display_counter = 0;
  if (rough_second_tick()) {
  // if (__display_counter++ == 1000) {
    debug_console_print_voltages();
    // __display_counter = 0;
  }
  #endif
}

void debug_console_print(char *data, uint16_t size) {
  #ifdef CONSOLE_OUTPUT
  HAL_UART_Transmit (&huart2, (uint8_t*)data, size, 10);    
  #endif
}

// void debug_console_print_magnet_present() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t data[] = "MAGNET PRESENT\r\n";
//   HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
//   #endif
// }

// void debug_console_print_single_activation() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t data[] = "SINGLE ACTIVATION\r\n";
//   HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
//   #endif
// }

// void debug_console_print_double_activation() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t data[] = "DOUBLE ACTIVATION\r\n";
//   HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
//   #endif
// }

// void debug_console_print_entering_stop_mode() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t data[] = "ENTERING STOP MODE\r\n";
//   HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
//   #endif
// }

// void debug_console_print_exiting_stop_mode() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t data[] = "EXITING STOP MODE\r\n";
//   HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
//   #endif
// }

// void debug_console_print_sw1_depressed() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t data[] = "SW1 DEPRESSED\r\n";
//   HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
//   #endif
// }

// void debug_console_print_initialization_error() {
//   #ifdef CONSOLE_OUTPUT
//   uint8_t data[] = "\r\nInitialization error. Stopped.\r\n";
//   HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
//   #endif
// }

void debug_console_print_splash() {
  #ifdef CONSOLE_OUTPUT
  uint8_t data[] = "\r\nRX Dual Power\r\nPhilippe Vico - 2021\r\n";
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
  #endif
}