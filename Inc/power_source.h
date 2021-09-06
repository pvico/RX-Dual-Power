#ifndef A6A27D9F_3D7E_42DF_8B43_C4DB9E5F2B77
#define A6A27D9F_3D7E_42DF_8B43_C4DB9E5F2B77

// Power sources
#define NO_SOURCE       0          
#define BEC_5V          1
#define BEC_5_5V        2
#define BEC_6V          3
#define BEC_7_4V        4
#define BEC_8_4V        5
#define LIPO_2S         6
#define LIPO_3S         7
#define LIPO_4S         8
#define LIFE_2S         9
#define LIFE_3S         10
#define LIFE_4S         11
#define NIMH_4S         12
#define NIMH_5S         13

#define MINIMUM_ACCEPTABLE_VOLTAGE_ADC_VALUE    232 // 4.6V

enum source_type {BATTERY, BEC};

void power_source_loop();

#endif /* A6A27D9F_3D7E_42DF_8B43_C4DB9E5F2B77 */
