#include "main_loop.h"
#include "main.h"
#include "led.h"
#include "magnet.h"
#include "voltage_sensor.h"
#include <stdbool.h>
#include "debug_console.h"
#include "system.h"
#include "power_source.h"
#include "button.h"
#include "switching_logic.h"
#include "telemetry.h"
#include "watchdog.h"


extern volatile bool timer_flag;


void main_loop() {
  // Performed every 1 ms
  if (timer_flag) {   // timer_flag is set by the Systick callback every 1ms     
    // refresh Watchdog
    LL_WWDG_SetCounter(WWDG, 0x7F);

    button_loop();
    magnet_loop();
    button_loop();
    leds_loop();
    voltage_sensor_loop();
    power_source_loop();
    switching_logic_loop();
    system_loop();

    timer_flag = false;
  }

// The telemetry_loop() call is outside of the 1ms loop because it has to reply to the RX
// polling after a delay (± 450µs, as is the case with the FrSky vario high prec.) set by
// Timer21 and telemetry_loop will do it when a flag is set by the Timer21 interrupt
#ifdef TELEMETRY_ENABLED
  telemetry_loop();
#endif
}