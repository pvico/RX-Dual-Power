#include "power_source.h"


Power_Source main_power_source = {0};
Power_Source stby_power_source = {0};


initialization_result initialize_BEC_power_source(Power_Source *power_source, source_position position) {
    power_source->source_type = BEC;
    // battery_type and battery_number_cells = 0 (undefined)
    power_source->position = position;
    power_source->minimum_voltage_ADC_value = BEC_MINIMUM_VOLTAGE_ADC_VALUE;
    power_source->critical_voltage_ADC_value = BEC_CRITICAL_VOLTAGE_ADC_VALUE;
    power_source->valid = true;

    return INITIALIZE_OK;
}

initialization_result initialize_Battery_power_source(Power_Source *power_source, battery_type type, battery_number_cells numbers_cells, source_position position) {
    power_source->source_type = BATTERY;
    power_source->battery_type = type;
    power_source->battery_number_cells = numbers_cells;

    switch (type) {
    case LIPO:
        if (numbers_cells < _2S || numbers_cells > _4S) {
            power_source->valid = false;
            return INITIALIZE_NOT_OK;
        }
        power_source->minimum_voltage_ADC_value = numbers_cells * LIPO_SINGLE_CELL_MINIMUM_VOLTAGE_ADC_VALUE;
        power_source->critical_voltage_ADC_value = numbers_cells * LIPO_SINGLE_CELL_CRITICAL_VOLTAGE_ADC_VALUE;
        break;
    case LIFE:
        if (numbers_cells < _2S || numbers_cells > _4S) {
            power_source->valid = false;
            return INITIALIZE_NOT_OK;
        }
        power_source->minimum_voltage_ADC_value = numbers_cells * LIFE_SINGLE_CELL_MINIMUM_VOLTAGE_ADC_VALUE;
        power_source->critical_voltage_ADC_value = numbers_cells * LIFE_SINGLE_CELL_CRITICAL_VOLTAGE_ADC_VALUE;
        break;
    case NIMH:
        if (numbers_cells < _4S || numbers_cells > _5S) {
            power_source->valid = false;
            return INITIALIZE_NOT_OK;
        }
        power_source->minimum_voltage_ADC_value = numbers_cells * NIMH_SINGLE_CELL_MINIMUM_VOLTAGE_ADC_VALUE;
        power_source->critical_voltage_ADC_value = numbers_cells * NIMH_SINGLE_CELL_CRITICAL_VOLTAGE_ADC_VALUE;
        break;
    default:
        power_source->valid = false;
        return INITIALIZE_NOT_OK;
        break;
    }

    power_source->position = position;
    power_source->valid = true;

    return INITIALIZE_OK;
}

bool is_power_source_valid(Power_Source *power_source) {
    return power_source->valid;
}

bool is_power_source_below_minimum_voltage(Power_Source *power_source, uint16_t last_16s_average_ADC_value) {
    return last_16s_average_ADC_value < power_source->minimum_voltage_ADC_value;
}

bool is_power_source_below_critical_voltage(Power_Source *power_source, uint16_t last_16ms_ADC_value) {
    return last_16ms_ADC_value < power_source->critical_voltage_ADC_value;
}

bool is_power_source_disconnected_or_shorted(Power_Source *power_source, uint16_t last_16ms_ADC_value) {
    return last_16ms_ADC_value < DISCONNECTED_OR_SHORT_ADC_VALUE;
}

initialization_result init_power_sources() {
    // TODO

    // example to start with
    if (initialize_BEC_power_source(&main_power_source, MAIN) == INITIALIZE_OK &&
        initialize_Battery_power_source(&stby_power_source, LIFE, _2S, STBY) == INITIALIZE_OK) {
        return INITIALIZE_OK;
    } else {
        return INITIALIZE_NOT_OK;
    }

}