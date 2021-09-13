#include "s_port.h"
#include <stdlib.h>
#include <usart.h>
#include <string.h>


extern uint8_t* receive_buffer;
extern uint8_t* transmit_buffer;

initialization_result init_s_port() {
    receive_buffer = malloc(S_PORT_RECEIVE_BUFFER_SIZE);
    transmit_buffer = malloc(S_PORT_TRANSMIT_BUFFER_SIZE);
    if (receive_buffer == NULL || transmit_buffer == NULL) {
        return INITIALIZE_NOT_OK;
    }

    // s_port_receive();
    HAL_UART_Receive_DMA(&huart2, receive_buffer, S_PORT_RECEIVE_BUFFER_SIZE); 
    // HAL_UART_Receive_IT(&huart2, receive_buffer, 2);

    return INITIALIZE_OK;
}

void s_port_loop() {

}

// void s_port_receive() {
//     HAL_UART_Receive_DMA(&huart2, receive_buffer, S_PORT_RECEIVE_BUFFER_SIZE);     
// }