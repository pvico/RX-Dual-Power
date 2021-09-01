#include "debug_console.h"
#include "usart.h"
#include "voltage_sensor.h"
#include "config.h"


void debug_console_print_voltages() {
  #ifdef DEBUG
  uint8_t data[] = "MAIN: ______\tSTBY: ______\r\n";
  main_voltage_str(data+6);
  stby_voltage_str(data+19);
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10); 
  #endif
}

#ifdef DEBUG
static uint16_t __display_counter = 0;
#endif

void debug_console_display_loop() {
  #ifdef DEBUG
  if (__display_counter++ == 10000) {
    debug_console_print_voltages();
    __display_counter = 0;
  }
  #endif
}

void debug_console_print_system_status() {
  #ifdef DEBUG
  uint8_t data[] = "SYSTEM ______\r\n";
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
  #endif
}

// void debug_console_print_magnet_present() {
//   #ifdef DEBUG
//   uint8_t data[] = "MAGNET PRESENT\r\n";
//   HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
//   #endif
// }

// void debug_console_print_single_activation() {
//   #ifdef DEBUG
//   uint8_t data[] = "SINGLE ACTIVATION\r\n";
//   HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
//   #endif
// }

// void debug_console_print_double_activation() {
//   #ifdef DEBUG
//   uint8_t data[] = "DOUBLE ACTIVATION\r\n";
//   HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
//   #endif
// }

void debug_console_print_entering_stop_mode() {
  #ifdef DEBUG
  uint8_t data[] = "ENTERING STOP MODE\r\n";
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
  #endif
}

void debug_console_print_exiting_stop_mode() {
  #ifdef DEBUG
  uint8_t data[] = "EXITING STOP MODE\r\n";
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
  #endif
}

void debug_console_print_sw1_depressed() {
  #ifdef DEBUG
  uint8_t data[] = "SW1 DEPRESSED\r\n";
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
  #endif
}

void debug_console_print_splash() {
  #ifdef DEBUG
  uint8_t data[] = "\r\nRX DUAL BATTERY SWITCH\r\nPhilippe Vico - 2021\r\n";
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
  #endif
}

