#include "switching_logic.h"


// Minimum voltage is the voltage at which, if the average MAIN PWR measured
// voltage was constantly below this value during 30 seconds, we will swicth
// to STBY PWR.
// The 30" are meant to average out any current spikes and thus voltage drops
// during temporary high aerodynamic loads or rapid movement of many servos.

// If, after swicthing to the STBY PWR source, MAIN PWR stays above the
// minimum voltage + HYSTERESIS_ADC_VALUE_FOR_REUSING_POWER_SOURCE for 30",
// MAIN PWR will be used again.

// If MAIN PWR measured voltage stays below the critical voltage for 16ms
// (16 loop iterations), we swicth to STBY PWR. Critical voltage is always lower
// than (or equal to for BEC) minimum voltage

// Critical condition exists when both sources are below minimum voltage.
// This includes critical voltage or disconnected status as the corresponding
// voltage levels are both below minimum voltage.

// See power_source.h for the values of minimum and critical voltage in
// function of power source type

void switching_logic_loop() {
    // TODO
}