#ifndef B3FBAB6A_2E5C_4234_8832_8C90EF85EFE3
#define B3FBAB6A_2E5C_4234_8832_8C90EF85EFE3

#include "initialize.h"
#include <stdint.h>

#define S_PORT_RECEIVE_BUFFER_SIZE 2
#define S_PORT_TRANSMIT_BUFFER_SIZE 8

initialization_result init_s_port();

void s_port_loop();

// void s_port_receive();

#endif /* B3FBAB6A_2E5C_4234_8832_8C90EF85EFE3 */
