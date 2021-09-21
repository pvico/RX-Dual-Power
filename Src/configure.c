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


static uint32_t *__eeprom = (uint32_t *)DATA_EEPROM_BASE;
static config_data __stored_data = {0};
 
 
//################################## Helper functions ##################################

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
    data.main_pwr_source_type = BATTERY;
    data.main_pwr_battery_type = LIFE;
    data.main_pwr_number_cells = _2S;
    data.stby_pwr_source_type = BATTERY;
    data.stby_pwr_battery_type = LIFE;
    data.stby_pwr_number_cells = _2S;
    __write_config(&data);
}
 
//######################################################################################
 
 
//################################ Interface functions #################################

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
 
//######################################################################################
