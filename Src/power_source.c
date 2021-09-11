#include "power_source.h"
#include "system.h"


extern Power_Source main_power_source;
extern Power_Source stby_power_source;

initialization_result initialize_BEC_power_source(Power_Source *power_source, source_position position) {
    power_source->source_type = BEC;
    // battery_type and battery_number_cells = 0 (undefined)
    power_source->position = position;
    power_source->minimum_voltage_ADC_value = BEC_MINIMUM_VOLTAGE_ADC_VALUE;
    power_source->critical_voltage_ADC_value = BEC_CRITICAL_VOLTAGE_ADC_VALUE;
    power_source->state = OK;
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
    power_source->state = OK;
    power_source->valid = true;

    return INITIALIZE_OK;
}

static bool __is_power_source_above_minimum_voltage(Power_Source *power_source) {
    // This uses the 16 seconds average (long integration)
    return !is_in_first_16s_after_startup() && power_source->last_16s_average_voltage_ADC_value > power_source->minimum_voltage_ADC_value;
}

static bool __is_power_source_above_critical_voltage(Power_Source *power_source) {
    // And that this uses the 16 milli-seconds average (short integration)
    return power_source->last_16ms_average_voltage_ADC_value > power_source->critical_voltage_ADC_value;
}

static bool __is_power_source_disconnected_or_shorted(Power_Source *power_source) {
    return power_source->last_16ms_average_voltage_ADC_value < DISCONNECTED_OR_SHORT_ADC_VALUE;
}

static bool __is_power_source_constantly_above_reinstate_voltage(Power_Source *power_source){
    return power_source->last_16s_average_voltage_ADC_value > (power_source->minimum_voltage_ADC_value + HYSTERESIS_ADC_VALUE_FOR_REUSING_POWER_SOURCE);
}

static bool __is_power_source_above_reinstate_voltage(Power_Source *power_source){
    // We can reinstate the power source if:
    // - after the first 16s after power up, the 16 seconds average is above the reinstate voltage (minimum voltage + hysteresis value)
    //                          or
    // - during the first 16s after power up, the 16 milli-seconds average is above the reinstate voltage (minimum voltage + hysteresis value)
    //
    // We do this so that if a fresh battery is connected just after power up, it is immediately indicated that the battery is present

    if (is_in_first_16s_after_startup()) {
        return power_source->last_16ms_average_voltage_ADC_value > (power_source->minimum_voltage_ADC_value + HYSTERESIS_ADC_VALUE_FOR_REUSING_POWER_SOURCE);
    } else {
        return __is_power_source_constantly_above_reinstate_voltage(power_source);
    }
}

void power_source_loop() {

    // Power source is ok if
    // - the 16 seconds average voltage is above minimum voltage and the 16 milli-seconds average is above critical voltage
    //                          or
    // - during the first 16 seconds after power up (the 16 seconds average has not been computed yet), the 16 milli-seconds average
    //   is above critical voltage
    //                          or
    // - the re-instate voltage conditions are met

    if ((__is_power_source_above_minimum_voltage(&main_power_source) && __is_power_source_above_critical_voltage(&main_power_source)) ||
        __is_power_source_above_reinstate_voltage(&main_power_source)) {
        main_power_source.state = OK;
    } else if (__is_power_source_disconnected_or_shorted(&main_power_source)) {
        main_power_source.state = DISCONNECTED_OR_SHORT;
    } else {
        main_power_source.state = LOW;
    }

    if ((__is_power_source_above_minimum_voltage(&stby_power_source) && __is_power_source_above_critical_voltage(&stby_power_source)) ||
        __is_power_source_above_reinstate_voltage(&stby_power_source)) {
        stby_power_source.state = OK;
    } else if (__is_power_source_disconnected_or_shorted(&stby_power_source)) {
        stby_power_source.state = DISCONNECTED_OR_SHORT;
    } else {
        stby_power_source.state = LOW;
    }
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