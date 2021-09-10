#ifndef A6A27D9F_3D7E_42DF_8B43_C4DB9E5F2B77
#define A6A27D9F_3D7E_42DF_8B43_C4DB9E5F2B77

#include <stdbool.h>
#include <stdint.h>
#include "initialize.h"


// See switching_logic.c for a description of the use of the following #defines

// See the ADC values corresponding to measured voltage in voltage_sensor.h

#define DISCONNECTED_OR_SHORT_ADC_VALUE     25          // 0.5V

#define LIPO_SINGLE_CELL_MINIMUM_VOLTAGE_ADC_VALUE 186       // 3.7V, 15% capacity remaining at relatively low average discharge currents 
#define LIPO_SINGLE_CELL_CRITICAL_VOLTAGE_ADC_VALUE 171       // 3.4V

#define LIFE_SINGLE_CELL_MINIMUM_VOLTAGE_ADC_VALUE 159       // 3.15V, 15% capacity remaining at relatively low average discharge currents 
#define LIFE_SINGLE_CELL_CRITICAL_VOLTAGE_ADC_VALUE 146       // 2.9V

#define NIMH_SINGLE_CELL_MINIMUM_VOLTAGE_ADC_VALUE 63       // 1.25V TO BE TESTED IN CONTROLLED DISCHARGE
#define NIMH_SINGLE_CELL_CRITICAL_VOLTAGE_ADC_VALUE 55       // 1.1V TO BE CHECKED

// BEC
// Although other types exist, most BEC's are buck converters, a type of switching
// mode power supply (SMPS) providing DC-DC step-down voltage conversion with high
// efficiency (80-90%). Buck converters can only output a voltage lower than the
// battery connected to it.

// Buck converters have a feedback loop, ajusting the duty cycle of the switching
// MOSFET (typically) to keep the output voltage constant with a varying battery
// voltage.

// Usually, when the connected battery voltage falls below output voltage + about
// 1V, the output voltage will start to reduce below the BEC's normal output
// voltage. The MOSFET duty cycle becomes 100% (it is conducting all the time) and
// the output voltage becomes the input voltage less the resistive loss and the
// diode forward drop.

// This is typical. However, this 1V value depends on the components used in the BEC
// and can be quite less than 1V in the case of a synchronous buck converter
// (2 MOSFET's and no diode).

// The voltage measured at the output of the BEC gives no real indication of the
// discharge level of the connected battery. If you use a 6S LiPo connected to the
// BEC, the BEC will perfectly regulate at 5V even though the battery is at a very
// deep discharge level and incurring irreversible damage.

// To keep the configuration very simple, without asking the end user to enter the BEC
// voltage (which can have many different values), the type of battery connected and
// the type of BEC construction (of which 99.9% of users have no idea), we will only
// consider that the BEC normal output voltage is 5V and that falling below 4.6V
// is below critical voltage. Anyway, if we enter critical condition, the LTC4412's
// will determine the selected power source and will use the highest voltage one
// regardless of its voltage so that the receiver and servos will remain powered
// until the last bit of available electrical power.

#define BEC_MINIMUM_VOLTAGE_ADC_VALUE 232       // 4.6V
#define BEC_CRITICAL_VOLTAGE_ADC_VALUE 232       // 4.6V

#define HYSTERESIS_ADC_VALUE_FOR_REUSING_POWER_SOURCE  15   // 0.3V

// typedef's
typedef enum {BEC, BATTERY} source_type;
typedef enum {BATT_TYPE_UNDEFINED=0, LIPO, LIFE, NIMH} battery_type;
typedef enum {BATT_CELLS_UNDEFINED=0, _2S = 2, _3S, _4S, _5S} battery_number_cells;
typedef enum {MAIN, STBY} source_position;
typedef enum {OK, LOW, DISCONNECTED_OR_SHORT} power_state;

typedef struct {
    source_type source_type;
    battery_type battery_type;
    battery_number_cells battery_number_cells;
    source_position position;
    uint16_t minimum_voltage_ADC_value;
    uint16_t critical_voltage_ADC_value;
    uint16_t current_voltage_ADC_value;
    uint16_t last_16ms_average_voltage_ADC_value;
    uint16_t last_16s_average_voltage_ADC_value;
    power_state state;
    bool valid;
} Power_Source;

// prototypes

initialization_result initialize_BEC_power_source(Power_Source *power_source, source_position position);
initialization_result initialize_Battery_power_source(Power_Source *power_source, battery_type type, battery_number_cells numbers_cells, source_position position);

// // if source average voltage for the last 16" is below this value, will switch to the other source
// bool is_power_source_below_minimum_voltage(Power_Source *power_source, uint16_t last_16s_average_ADC_value);

// // if source average voltage for the last 16ms is below this value, will immediately switch to the other source
// bool is_power_source_below_critical_voltage(Power_Source *power_source, uint16_t last_16ms_ADC_value);

// bool is_power_source_disconnected_or_shorted(Power_Source *power_source, uint16_t last_16ms_ADC_value);

// bool is_power_source_above_reinstate_voltage(Power_Source *power_source, uint16_t last_16s_average_ADC_value);

initialization_result init_power_sources();

void power_source_loop();


#endif /* A6A27D9F_3D7E_42DF_8B43_C4DB9E5F2B77 */
