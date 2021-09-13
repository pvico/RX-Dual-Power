#include "telemetry.h"
#include "s_port.h"
#include "sbus2.h"
#include "x-bus.h"
#include "m-link.h"
#include <usart.h>


extern telemetry_types telemetry_type;

extern uint8_t* receive_buffer;
extern uint8_t* transmit_buffer;

initialization_result init_telemetry() {
    // TODO: config

    switch (telemetry_type) {
    case S_PORT:
        telemetry_type = S_PORT;
        return init_s_port();
        break;
    // case SBUS2:
    //     break;
    // case X_BUS:
    //     break;
    // case M_LINK:
    //     break;
    default:
        break;
    }
    return INITIALIZE_NOT_OK;
}

void telemetry_loop() {
    switch (telemetry_type) {
    case S_PORT:
        s_port_loop();
        break;
    // case SBUS2:
    //     break;
    // case X_BUS:
    //     break;
    // case M_LINK:
    //     break;
    default:
        break;
    }
}

// void telemetry_receive() {
//     switch (telemetry_type) {
//     case S_PORT:
//         s_port_receive();
//         break;
//     // case SBUS2:
//     //     break;
//     // case X_BUS:
//     //     break;
//     // case M_LINK:
//     //     break;
//     default:
//         break;
//     }
// }