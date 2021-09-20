#include "configure.h"
#include "led.h"
#include <stdbool.h>
#include <string.h>
#include "eeprom.h"


static config_data *__stored_config = (config_data *)DATA_EEPROM_BASE;


static void __write_config(config_data *new_config) {
    for (uint16_t i = 0; i < NUM_WORDS_IN_CONFIG_DATA; i++) {
        uint32_t *psrc = (uint32_t*)new_config;
        eeprom_write_word(psrc[i], i);
    }
}

void __configure() {
    config_data data = {0};
    data.config_ok_code = CONFIG_OK_CODE;
    data.main_pwr_source_type = BATTERY;
    data.main_pwr_battery_type = LIPO;
    data.main_pwr_number_cells = _2S;
    data.stby_pwr_source_type = BATTERY;
    data.stby_pwr_battery_type = LIFE;
    data.stby_pwr_number_cells = _2S;
    __write_config(&data);
}

void init_configure() {
    // __configure();
}

inline config_data *get_stored_configuration() {
    return __stored_config;
}


bool is_config_valid() {
    return (__stored_config->config_ok_code == CONFIG_OK_CODE);
}