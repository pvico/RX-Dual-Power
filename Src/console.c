#include "console.h"
#include "usart.h"
#include "voltage_sensor.h"
#include "config.h"


void console_print_voltages() {
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

void console_display_loop() {
  #ifdef DEBUG
  if (__display_counter++ == 10000) {
    console_print_voltages();
    __display_counter = 0;
  }
  #endif
}

void console_print_system_status() {
  #ifdef DEBUG
  uint8_t data[] = "SYSTEM ______\r\n";
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
  #endif
}

void console_print_magnet_present() {
  #ifdef DEBUG
  uint8_t data[] = "MAGNET PRESENT\r\n";
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
  #endif
}