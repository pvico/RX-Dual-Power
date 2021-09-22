//######################################################################################
// uart.h
// UART initialization
//
// Philippe Vico - 2021
//######################################################################################


#ifndef C3C67899_BFD3_4074_B974_43B61C54F068
#define C3C67899_BFD3_4074_B974_43B61C54F068


#include <stdint.h>
#include <stdbool.h>


typedef enum {BYTE_FORMAT_8N1, BYTE_FORMAT_9E2} uart_byte_format;   // Number of bits includes parity bit if any
typedef enum {NO_INVERSION, RX_INVERTED_TX_INVERTED} uart_pin_inversion;


void init_uart();

void configure_uart(uint32_t baudrate, uart_byte_format data_type, uart_pin_inversion pin_inversion); 


#endif /* C3C67899_BFD3_4074_B974_43B61C54F068 */
