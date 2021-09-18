#include "telemetry.h"
#include "s_port.h"
#include "sbus2.h"
#include "x-bus.h"
#include "m-link.h"
#include "uart.h"
#include "led.h"


extern telemetry_types telemetry_type;

extern uint8_t* receive_buffer;
extern uint8_t* transmit_buffer;
extern volatile bool uart_rx_complete;


initialization_result init_telemetry() {
    // TODO: config

    switch (telemetry_type) {
    case S_PORT_TYPE:
        telemetry_type = S_PORT_TYPE;
        return init_s_port();
        break;
    // case SBUS2_TYPE:
    //     break;
    // case X_BUS_TYPE:
    //     break;
    // case M_LINK_TYPE:
    //     break;
    default:
        break;
    }
    return INITIALIZE_OK;
}

void telemetry_loop() {
    switch (telemetry_type) {
    case S_PORT_TYPE:
        s_port_loop();
        break;
    // case SBUS2_TYPE:
    //     break;
    // case X_BUS_TYPE:
    //     break;
    // case M_LINK_TYPE:
    //     break;
    default:
        break;
    }
}

void uart_receive_byte_callback() {
    switch (telemetry_type) {
    case S_PORT_TYPE:
        s_port_uart_receive_byte_callback();
        break;
    // case SBUS2_TYPE:
    //     break;
    // case X_BUS_TYPE:
    //     break;
    // case M_LINK_TYPE:
    //     break;
    default:
        break;
    }
}
