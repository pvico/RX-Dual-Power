#include "power_source.h"


Power_Source main_power_source;
Power_Source stby_power_source;


Power_Source create_BEC_power_source(source_position position) {
    Power_Source power_source = {0};
    power_source.source_type = BEC;
    // battery_type and battery_number_cells = 0 (undefined)
    power_source.position = position;
    power_source.minimum_voltage_ADC_value = BEC_MINIMUM_VOLTAGE_ADC_VALUE;
    power_source.critical_voltage_ADC_value = BEC_CRITICAL_VOLTAGE_ADC_VALUE;
    power_source.valid = true;
    return power_source;
}

// Power_Source create_Battery_power_source(battery_number_cells numbers_cells, source_position position) {
//     Power_Source power_source = {0};
//     power_source.source_type = LIPO;
//     power_source.battery_number_cells = numbers_cells;
//     power_source.position = position;
//     return power_source;
// }

bool is_power_source_valid(Power_Source *power_source) {
    return power_source->valid;
}

bool is_power_source_below_minimum_voltage(Power_Source *power_source, uint16_t average_ADC_value) {
    return average_ADC_value < power_source->minimum_voltage_ADC_value;
}

bool is_power_source_below_critical_voltage(Power_Source *power_source, uint16_t last_8ms_ADC_value) {
    return last_8ms_ADC_value < power_source->critical_voltage_ADC_value;
}

bool is_power_source_disconnected_or_shorted(Power_Source *power_source, uint16_t average_ADC_value) {
    // TODO
    return false;
}

initialization_result init_power_sources() {
    // TODO
    return INITIALIZE_OK;
}