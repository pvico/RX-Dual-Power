#include "s_port.h"
#include "main.h"
#include <stdlib.h>
#include "uart.h"
#include <string.h>
#include "led.h"
#include "power_source.h"
#include "voltage_sensor.h"
#include "switching_logic.h"
#include "timer.h"


extern Power_Source main_power_source;
extern Power_Source stby_power_source;
extern switching_states switching_state;
extern uint8_t* transmit_buffer;
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

    transmit_buffer = malloc(S_PORT_TRANSMIT_BUFFER_SIZE);
    if (transmit_buffer == NULL) {
        return INITIALIZE_NOT_OK;
    }

    LL_USART_EnableHalfDuplex(USART2);
    LL_USART_EnableIT_RXNE(USART2);

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

        LL_USART_DisableDirectionRx(USART2);
        LL_USART_EnableDirectionTx(USART2);
        
        for (size_t i = 0; i < S_PORT_TRANSMIT_BUFFER_SIZE; i++) {
            LL_USART_TransmitData8(USART2, transmit_buffer[i]);
            while (!LL_USART_IsActiveFlag_TXE(USART2));
        }

        LL_USART_DisableDirectionTx(USART2);
        LL_USART_EnableDirectionRx(USART2);
    }
}

void s_port_uart_receive_byte_callback() {
    static bool poll_first_byte_detected = false;
    uint8_t byte;

    if (LL_USART_IsActiveFlag_RXNE(USART2)) {
        byte = LL_USART_ReceiveData8(USART2);
        if (byte == 0x7E) {
            poll_first_byte_detected = true;
        } else if (poll_first_byte_detected) {
            poll_first_byte_detected = false;
            if (byte == S_PORT_DEVICE_ID_WITH_CRC) {
                LL_TIM_ClearFlag_UPDATE(TIM21);
                LL_TIM_EnableIT_UPDATE(TIM21);
                LL_TIM_SetCounter(TIM21, 0);
                LL_TIM_EnableCounter(TIM21);

            }
        }        
    }
}

void s_port_response_timer_callback() {
    requested_to_transmit_data = true;
    LL_TIM_DisableIT_UPDATE(TIM21);
    LL_TIM_DisableCounter(TIM21);
}

