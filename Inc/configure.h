#ifndef B4D26B50_52A0_4F93_BC59_89DE8DDA842C
#define B4D26B50_52A0_4F93_BC59_89DE8DDA842C

#include "main.h"
#include "power_source.h"
#include <stdint.h>

// TODO

#define EEPROM_SIZE (DATA_EEPROM_END - DATA_EEPROM_BASE)

#define CONFIG_OK_CODE ((((uint32_t)'C') << 24UL) | (((uint32_t)'F') << 16UL) | (((uint32_t)'O') << 8UL) | ((uint32_t)'K'))


typedef struct {
    uint32_t config_ok_code;
    source_type main_pwr_source_type;
    battery_type main_pwr_battery_type;
    battery_number_cells main_pwr_number_cells;
    source_type stby_pwr_source_type;
    battery_type stby_pwr_battery_type;
    battery_number_cells stby_pwr_number_cells;
} config_data;

#define NUM_WORDS_IN_CONFIG_DATA (sizeof(config_data) / sizeof(uint32_t))


void init_configure();
bool is_config_valid();
config_data *get_stored_configuration();

#endif /* B4D26B50_52A0_4F93_BC59_89DE8DDA842C */
