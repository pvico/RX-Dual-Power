#include "s_port.h"
#include <stdlib.h>
#include <usart.h>
#include <stm32l0xx.h>
// #include <stm32l0xx_ll_usart.h>
// #include <stm32l0xx_ll_tim.h>
#include "usart.h"
#include <string.h>
#include "led.h"
#include "power_source.h"
#include "voltage_sensor.h"
#include "switching_logic.h"
#include "tim.h"

extern Power_Source main_power_source;
extern Power_Source stby_power_source;
extern switching_states switching_state;
extern uint8_t* transmit_buffer;
extern UART_HandleTypeDef huart2;
extern volatile bool requested_to_transmit_data ;


static uint8_t __compute_FrSky_CRC (uint8_t *packet) {
    uint16_t crc = 0;
    for (uint8_t i = 0; i < 8; i++) {
        crc += packet[i]; //0-1FF
        crc += crc >> 8;  //0-100
        crc &= 0x00ff;
        crc += crc >> 8;  //0-0FF
        crc &= 0x00ff;
    }
    return ~crc;
}

initialization_result init_s_port() {

    // LL_TIM_EnableIT_CC1(TIM21);

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
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);

    return INITIALIZE_OK;
}

void s_port_loop() {
    static s_port_data_type data_type = SPORT_DATA_BATT1_MILLIVOLTS;
    uint16_t milliV;
    uint16_t state;
    
    if (requested_to_transmit_data) {
        requested_to_transmit_data = false;

        memset(transmit_buffer, 0, S_PORT_TRANSMIT_BUFFER_SIZE);

        switch (data_type++) {
        case SPORT_DATA_BATT1_MILLIVOLTS:
            milliV = voltage_ADC_to_millivolts(main_power_source.last_16ms_average_voltage_ADC_value);
            transmit_buffer[0] = 0x10;       
            transmit_buffer[1] = 0x00;      // first ID
            transmit_buffer[2] = 0x0B;      // RBOX BATT1
            memcpy(transmit_buffer+3, &milliV, 2);  // bytes 3 & 4 are the voltage in mV
            // leave the bytes 5 & 6 at 0 (current bytes)
            break;
        case SPORT_DATA_BATT2_MILLIVOLTS:
            milliV = voltage_ADC_to_millivolts(stby_power_source.last_16ms_average_voltage_ADC_value);
            transmit_buffer[0] = 0x10;
            transmit_buffer[1] = 0x10;      // first ID
            transmit_buffer[2] = 0x0B;      // RBOX BATT2
            memcpy(transmit_buffer+3, &milliV, 2);
            break;
        case SPORT_DATA_STATE:
            // state = switching_state;
            state = 1 << switching_state;   // switching state appears as CH01..CH11 in OpenTX telemetry screen
            transmit_buffer[0] = 0x10;
            transmit_buffer[1] = 0x20;      // first ID
            transmit_buffer[2] = 0x0B;      // RBOX STATE
            memcpy(transmit_buffer+3, &state, 2);
            break;
        default:
            break;
        }
        transmit_buffer[7] = __compute_FrSky_CRC(transmit_buffer);

        if (data_type > SPORT_DATA_STATE) {
            data_type = SPORT_DATA_BATT1_MILLIVOLTS;
        }

        HAL_HalfDuplex_EnableTransmitter(&huart2);
        HAL_UART_Transmit(&huart2, transmit_buffer, S_PORT_TRANSMIT_BUFFER_SIZE, 10);
        HAL_HalfDuplex_EnableReceiver(&huart2);
    }
}

void s_port_uart_receive_byte_callback() {
    static bool poll_first_byte_detected = false;
    uint8_t byte;

    if (huart2.Instance->ISR & UART_FLAG_RXNE) {
        byte = huart2.Instance->RDR;

        if (byte == 0x7E) {
            poll_first_byte_detected = true;
        } else if (poll_first_byte_detected) {
            poll_first_byte_detected = false;
            if (byte == S_PORT_DEVICE_ID_WITH_CRC) {
                HAL_TIM_Base_Start_IT(&htim21);
                // LL_TIM_EnableCounter(TIM21);
            }
        }        
    }
}

// void s_port_response_timer_callback() {
//     // LL_TIM_DisableCounter(TIM21);
//     // LL_TIM_SetCounter(TIM21, 400);
//     requested_to_transmit_data = true;
//     // HAL_TIM_Base_Stop_IT(%htim21);
//     // HAL_TIM_
// }

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim21) {
        HAL_TIM_Base_Stop_IT(&htim21);
        requested_to_transmit_data = true;
    }
}
