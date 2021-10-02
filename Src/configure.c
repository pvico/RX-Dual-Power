//######################################################################################
// configure.c
// Configuration of power sources stored in EEPROM
//
// Philippe Vico - 2021
//######################################################################################
 
 
#include "configure.h"
#include "led.h"
#include <stdbool.h>
#include <string.h>
#include "eeprom.h"
#include "uart.h"


extern bool uart_in_config_mode;
extern uint32_t system_tick;


static uint32_t *__eeprom = (uint32_t *)DATA_EEPROM_BASE;
static config_data __stored_data = {0};
 
 
//############################### Local helper functions ###############################

static void __write_config(config_data *new_config) {
    // for (uint16_t i = 0; i < NUM_WORDS_IN_CONFIG_DATA; i++) {
    //     uint32_t *psrc = (uint32_t*)new_config;
    //     eeprom_write_word(psrc[i], i);
    // }

    // We write two 32 bit words
    // The first one contains the OK code
    // The second one contains the configuration data as follows:
    // Bit  [0:1]: main pwr source type (BEC or battery)
    // Bits [2:4]: main pwr battery type
    // Bits [5:8]: main pwr number of battery cells
    // Bits [9:31]: reserved
    // For the stby pwr, we do the same shifted by 16 to the left
    // and or'ed with the main pwr data
    uint32_t config_word = 0L;
    config_word = (new_config->main_pwr_source_type         | 
                  (new_config->main_pwr_battery_type << 2L) |
                  (new_config->main_pwr_number_cells << 5L)   )
                  |
                  ((new_config->stby_pwr_source_type        | 
                  (new_config->stby_pwr_battery_type << 2L) |
                  (new_config->stby_pwr_number_cells << 5L)) << 16);

    eeprom_write_word(CONFIG_OK_CODE, 0);
    eeprom_write_word(config_word, 1);
}

void __configure() {
    config_data data = {0};
    data.main_pwr_source_type = BEC;
    data.main_pwr_battery_type = BATT_TYPE_UNDEFINED;
    data.main_pwr_number_cells = BATT_CELLS_UNDEFINED;
    data.stby_pwr_source_type = BATTERY;
    data.stby_pwr_battery_type = LIFE;
    data.stby_pwr_number_cells = _2S;
    __write_config(&data);
}
 
//######################################################################################
 
 
//################################## Public functions ##################################

void init_configure() {
    // __configure();
}

config_data *get_stored_configuration() {
    uint32_t data = __eeprom[1];

    __stored_data.main_pwr_source_type =  (data & 0b000000011L);
    __stored_data.main_pwr_battery_type = (data & 0b000011100L) >> 2;
    __stored_data.main_pwr_number_cells = (data & 0b111100000L) >> 5;

    // stby pwr source
    data = (data >> 16) & 0xffff;
    __stored_data.stby_pwr_source_type =  (data & 0b000000011L);
    __stored_data.stby_pwr_battery_type = (data & 0b000011100L) >> 2;
    __stored_data.stby_pwr_number_cells = (data & 0b111100000L) >> 5;
    return &__stored_data;
}


bool is_config_valid() {
    return (__eeprom[0] == CONFIG_OK_CODE);
}

void check_serial_configuration() {

    // Process:
    // The serial programmer repetitively sends a distinctive message to manifest its presence: "PROG"
    // We listen for that message during 500ms
    // If we get that message, we enter configuration mode with no limit in time
    // The serial programmer sends a message to read our current configuration
    // When we get that message, we send the current configuration
    // That configuration will be displayed on the programmer UI
    // The programmer allows the user to customize the configuration
    // When ready, the programmer sends to us a message containing the new configuration
    // The new configuration is written in EEPROM
    // When ready, we send an OK message
    // The programmer sends a message to read our (new) current configuration
    // The programmer will then display on its interface this success
    // The user can create and send a modified configuration and the process repeats
    // The user terminates the configuration by disconnecting the programmer which is 
    // also powering the RX Dual Power. When the RX Dual Power is powered up again, it will not 
    // see the programmer during the first 500ms and will thus proceed for normal start

    // Serial configuration
    // uart_in_config_mode = true;
    // leds_on();        // Both bright illuminated. They will have their normal behavior once the led_loop() runs

    // LL_USART_Enable(USART2);  
    // LL_USART_EnableHalfDuplex(USART2);
    // LL_USART_EnableIT_RXNE(USART2);
    // LL_USART_DisableDirectionRx(USART2);
    // LL_USART_EnableDirectionTx(USART2);
    // // for (size_t i = 0; i < S_PORT_TRANSMIT_BUFFER_SIZE; i++) {
    // //     LL_USART_TransmitData8(USART2, transmit_buffer[i]);
    // //     while (!LL_USART_IsActiveFlag_TXE(USART2));
    // // }
    // while (system_tick < 500) {}
    // uart_in_config_mode = false;

    // LL_USART_DisableDirectionTx(USART2);
    // LL_USART_EnableDirectionRx(USART2);
    // LL_USART_DisableIT_RXNE(USART2);
    // LL_USART_DisableHalfDuplex(USART2);
  // LL_USART_Disable(USART2); 
}
 
//######################################################################################
