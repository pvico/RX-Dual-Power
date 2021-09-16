#ifndef B3FBAB6A_2E5C_4234_8832_8C90EF85EFE3
#define B3FBAB6A_2E5C_4234_8832_8C90EF85EFE3

#include "initialize.h"
#include <stdint.h>

#define S_PORT_TRANSMIT_BUFFER_SIZE 8

// FrSky Redundancy Box id
#define S_PORT_DEVICE_ID_WITH_CRC 0x39

initialization_result init_s_port();

void s_port_loop();

void s_port_uart_receive_byte_callback();

#endif /* B3FBAB6A_2E5C_4234_8832_8C90EF85EFE3 */
