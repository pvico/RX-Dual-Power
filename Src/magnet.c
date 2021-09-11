#include "magnet.h"
#include <stdint.h>
#include "config.h"
#include "debug_console.h"
#include "main.h"


static bool double_activation_state = false;
static bool single_activation_state = false;

static magnet_state __magnet_debounced_state = MAGNET_NOT_PRESENT;

static bool __is_magnet_detected() {
    return HAL_GPIO_ReadPin(MAGNET_GPIO_Port, MAGNET_Pin) == MAGNET_PRESENT_PIN_STATE;
}

static uint16_t __magnet_detected_loop_counter = 0;
static uint16_t __magnet_not_detected_loop_counter = 0;
static uint16_t __after_activation_loop_counter = 0;
void magnet_loop() {
    if (__is_magnet_detected()) {
      if (__magnet_debounced_state == MAGNET_NOT_PRESENT) { 
        __magnet_detected_loop_counter++;
        if (__magnet_detected_loop_counter > MAGNET_DEBOUNCE_DELAY_MILLIS) {      // > 1 sec.
          __magnet_debounced_state = MAGNET_PRESENT;
          __magnet_detected_loop_counter = 0;

          if (__after_activation_loop_counter > MAGNET_SECOND_DETECTION_MINIMUM_INTERVAL_MILLIS &&
            __after_activation_loop_counter < MAGNET_SECOND_DETECTION_TIMEOUT_MILLIS) {
            double_activation_state = true;
            single_activation_state = false;
            // debug_console_print_double_activation();
            __after_activation_loop_counter = 0;
          } else {
            double_activation_state = false;
            single_activation_state = true;
            // debug_console_print_single_activation();
            __after_activation_loop_counter = 0;
          }
        }
      }
    } else {
      // magnet is not detected
      if (__magnet_debounced_state == MAGNET_PRESENT) {
        __magnet_not_detected_loop_counter++;
        if (__magnet_not_detected_loop_counter > MAGNET_DEBOUNCE_DELAY_MILLIS) {      // > 1 sec.
          __magnet_debounced_state = MAGNET_NOT_PRESENT;
          __magnet_not_detected_loop_counter = 0;
        }               
      }
    }    

    if (single_activation_state) {      
      if (__after_activation_loop_counter++ == 0xFFFF) {
        // roll over to above MAGNET_SECOND_DETECTION_TIMEOUT_MILLIS
        __after_activation_loop_counter = MAGNET_SECOND_DETECTION_TIMEOUT_MILLIS;
      }
    }
}


inline bool is_magnet_double_activation_active() {
  return double_activation_state;
}

bool is_magnet_present() {
  return __magnet_debounced_state == MAGNET_PRESENT;
}
