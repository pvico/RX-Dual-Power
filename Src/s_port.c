#include "s_port.h"
#include <stdlib.h>
#include <usart.h>
#include <stm32l0xx.h>
// #include <stm32l0xx_ll_usart.h>
#include "usart.h"
#include <string.h>
#include "led.h"


// extern uint8_t* receive_buffer;
extern uint8_t* transmit_buffer;
extern UART_HandleTypeDef huart2;
extern volatile bool requested_to_transmit_data ;

initialization_result init_s_port() {
    // receive_buffer = malloc(S_PORT_RECEIVE_BUFFER_SIZE);
    transmit_buffer = malloc(S_PORT_TRANSMIT_BUFFER_SIZE);
    // if (receive_buffer == NULL || transmit_buffer == NULL) {
    if (transmit_buffer == NULL) {
        return INITIALIZE_NOT_OK;
    }
    // Invert signal level§
    // LL_USART_Disable(USART2);
    // LL_USART_SetRXPinLevel(USART2, LL_USART_RXPIN_LEVEL_INVERTED);
    // LL_USART_SetTXPinLevel(USART2, LL_USART_TXPIN_LEVEL_INVERTED);
    // LL_USART_Enable(USART2);
    // LL_USART_EnableHalfDuplex(USART2);

    // while((!(LL_USART_IsActiveFlag_TEACK(USART2))) || (!(LL_USART_IsActiveFlag_REACK(USART2))));

    // LL_USART_EnableIT_RXNE(USART2);
    // LL_USART_EnableIT_ERROR(USART2);

    // huart2.AdvancedInit.RxPinLevelInvert = UART_ADVFEATURE_RXINV_ENABLE;
    // huart2.AdvancedInit.TxPinLevelInvert = UART_ADVFEATURE_TXINV_ENABLE;
    // if (HAL_HalfDuplex_Init(&huart2) != HAL_OK) {
    //     Error_Handler();
    // }
    HAL_HalfDuplex_EnableReceiver(&huart2);
    // s_port_receive();
    // TODO: in LL
    // HAL_UART_Receive_DMA(&huart2, receive_buffer, S_PORT_RECEIVE_BUFFER_SIZE); 
    // HAL_UART_Receive_IT(&huart2, receive_buffer, 2);
    // __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);

    return INITIALIZE_OK;
}

static void __set_empty_reply() {
    memset(transmit_buffer, 0, S_PORT_TRANSMIT_BUFFER_SIZE-1);
    transmit_buffer[S_PORT_TRANSMIT_BUFFER_SIZE-1] = 0xFF;
}

void s_port_loop() {
    // if (uart_rx_complete) {
    //     uart_rx_complete = false;
    //     if ((receive_buffer[0] == 0x7e) || (receive_buffer[1] == 0x7E)) {
    //     // if ((receive_buffer[1] == 0x7E)) {
    //         leds_show_error;
    //         for (size_t i = 0; i < 100000L; i++)
    //         {
    //             ;
    //         }
            
    //     }
    //     HAL_UART_Receive_IT(&huart2, receive_buffer, 2);
    // }
    
    __set_empty_reply();


    // if (requested_to_transmit_data) {
    //     requested_to_transmit_data = false;
    //     // transmit LSB first
    //     // 
    //     HAL_HalfDuplex_EnableTransmitter(&huart2);
    //     HAL_UART_Transmit(&huart2, transmit_buffer, S_PORT_TRANSMIT_BUFFER_SIZE, 10);
    //     HAL_HalfDuplex_EnableReceiver(&huart2);
    //     __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    // }
}

// void s_port_receive() {
//     HAL_UART_Receive_DMA(&huart2, receive_buffer, S_PORT_RECEIVE_BUFFER_SIZE);     
// }

void s_port_uart_receive_byte_callback() {
    // static bool poll_first_byte_detected = false;
    // uint8_t byte;

    // if (huart2.Instance->ISR & UART_FLAG_RXNE) {
    //     byte = huart2.Instance->RDR;

    //     if (byte == 0x7E) {
    //         poll_first_byte_detected = true;
    //     } else if (poll_first_byte_detected) {
    //         poll_first_byte_detected = false;
    //         if (byte == S_PORT_DEVICE_ID_WITH_CRC) {
    //             requested_to_transmit_data = true;
    //         }
    //     }        
    // }
}