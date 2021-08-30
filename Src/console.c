#include "console.h"
#include "usart.h"

void print_voltages() {
  uint8_t data[] = "MAIN: ______\tSTBY: ______\r\n";
  main_voltage_str(data+6);
  stby_voltage_str(data+19);
  HAL_UART_Transmit (&huart2, data, sizeof (data) -1, 10);    
}