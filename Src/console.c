#include "console.h"
#include "usart.h"
#include "voltage_sensor.h"

uint16_t display_counter = 0;

void print_voltages() {
  uint8_t data[] = "MAIN: ______\tSTBY: ______\r\n";
  main_voltage_str(data+6);
  stby_voltage_str(data+19);
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
}

void display_loop() {
  if (display_counter++ == 2000) {
    print_voltages();
    display_counter = 0;
  }
}