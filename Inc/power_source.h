#ifndef A6A27D9F_3D7E_42DF_8B43_C4DB9E5F2B77
#define A6A27D9F_3D7E_42DF_8B43_C4DB9E5F2B77

// Power sources
// #define NO_SOURCE       0          
// #define BEC_5V          1
// #define BEC_5_5V        2
// #define BEC_6V          3
// #define BEC_7_4V        4
// #define BEC_8_4V        5
// #define LIPO_2S         6
// #define LIPO_3S         7
// #define LIPO_4S         8
// #define LIFE_2S         9
// #define LIFE_3S         10
// #define LIFE_4S         11
// #define NIMH_4S         12
// #define NIMH_5S         13

#include <stdbool.h>
#include <stdint.h>


// TBD:
#define CRITICAL_VOLTAGE_ADC_VALUE    232 // 4.6V

typedef enum source_types {BATTERY, BEC} source_type;
typedef enum battery_types {LIPO, LIFE, NIMH} battery_type;
typedef enum battery_number_cellss {_2S, _3S, _4S, _5S} battery_number_cells;
typedef enum source_positions {MAIN, STBY} source_position;

typedef struct
{
    source_type source_type;
    battery_type battery_type;
    battery_number_cells battery_number_cells;
    source_position source_position;
    bool valid;
} Power_Source;

Power_Source create_BEC_power_source(source_position position);
Power_Source create_LiPo_power_source(battery_number_cells numbers_cells, source_position position);
Power_Source create_LiFe_power_source(battery_number_cells numbers_cells, source_position position);
Power_Source create_NiMh_power_source(battery_number_cells numbers_cells, source_position position);

bool is_power_source_valid(Power_Source *power_source);

bool is_power_source_below_minimum_voltage(Power_Source *power_source);

bool is_power_source_disconnected_or_shorted(Power_Source *power_source);

bool is_critical(Power_Source *main_power_source, Power_Source *stby_power_source);

void power_source_loop();

#endif /* A6A27D9F_3D7E_42DF_8B43_C4DB9E5F2B77 */
