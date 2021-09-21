//######################################################################################
// s_port.h
// Telemetry interface for S.PORT protocol (FrSky)
//
// Philippe Vico - 2021
//######################################################################################


#ifndef B3FBAB6A_2E5C_4234_8832_8C90EF85EFE3
#define B3FBAB6A_2E5C_4234_8832_8C90EF85EFE3

#include "initialize.h"
#include <stdint.h>


// FrSky Redundancy Box id with CRC
#define S_PORT_DEVICE_ID_WITH_CRC 0x39      // = ID 25
#define S_PORT_TRANSMIT_BUFFER_SIZE 8


typedef enum {SPORT_DATA_BATT1_MILLIVOLTS, SPORT_DATA_BATT2_MILLIVOLTS, SPORT_DATA_STATE} s_port_data_type;


initialization_result init_s_port();
void s_port_loop();

void s_port_uart_receive_byte_callback();
void s_port_response_timer_callback();


#endif /* B3FBAB6A_2E5C_4234_8832_8C90EF85EFE3 */
