#include "power_source.h"


Power_Source main_power_source;
Power_Source stby_power_source;


Power_Source create_BEC_power_source(source_position position) {
    // TODO
}

Power_Source create_LiPo_power_source(battery_number_cells numbers_cells, source_position position) {
    // TODO
}

Power_Source create_LiFe_power_source(battery_number_cells numbers_cells, source_position position) {
    // TODO
}

Power_Source create_NiMh_power_source(battery_number_cells numbers_cells, source_position position) {
    // TODO
}

bool is_power_source_valid(Power_Source *power_source) {
    // TODO
}

bool is_power_source_below_minimum_voltage(Power_Source *power_source) {
    // TODO
}

bool is_power_source_disconnected_or_shorted(Power_Source *power_source) {
    // TODO
}

bool is_critical(Power_Source *main_power_source, Power_Source *stby_power_source) {
    // TODO
}


void power_source_loop() {
    // TODO
}