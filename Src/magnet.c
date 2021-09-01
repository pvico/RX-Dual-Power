#include "magnet.h"
#include "led.h"
#include <stdint.h>
#include "config.h"
#include "console.h"


static bool double_activation_state = false;
static bool single_activation_state = false;

static enum magnet_states magnet_debounced_state = MAGNET_NOT_PRESENT;

static bool __is_magnet_detected() {
    return HAL_GPIO_ReadPin(MAGNET_GPIO, MAGNET_PIN) == MAGNET_PRESENT_PIN_STATE;
}


extern enum led_states led2_state;
static enum led_states __led2_previous_state;
void __show_magnet_presence() {
  if (magnet_debounced_state == MAGNET_PRESENT) {
    if (led2_state != BLINK_FAST) {
      __led2_previous_state = led2_state;
    }
    led2_state = BLINK_FAST;
  } else {
    led2_state = __led2_previous_state;
  }
}

static uint16_t __magnet_detected_loop_counter = 0;
static uint16_t __magnet_not_detected_loop_counter = 0;
static uint16_t __after_activation_loop_counter = 0;
void magnet_loop() {
    if (__is_magnet_detected()) {
      if (magnet_debounced_state == MAGNET_NOT_PRESENT) { 
        // magnet is detected and magnet_debounced_state is MAGNET_NOT_PRESENT
        
        __magnet_detected_loop_counter++;
        if (__magnet_detected_loop_counter > MAGNET_DEBOUNCE_DELAY_MILLIS) {      // > 1 sec.
          magnet_debounced_state = MAGNET_PRESENT;
          if (__after_activation_loop_counter > MAGNET_SECOND_DETECTION_MINIMUM_INTERVAL_MILLIS &&
            __after_activation_loop_counter < MAGNET_SECOND_DETECTION_TIMEOUT_MILLIS) {
            double_activation_state = true;
            single_activation_state = false;
            console_print_double_activation();
            __after_activation_loop_counter = 0;
          } else {
            double_activation_state = false;
            single_activation_state = true;
            console_print_single_activation();
            __after_activation_loop_counter = 0;
          }
          __magnet_detected_loop_counter = 0;
        }
      } else {
        // magnet is detected and magnet_debounced_state is MAGNET_PRESENT
      }
    } else {
      // magnet is not detected
      if (magnet_debounced_state == MAGNET_PRESENT) {
        // magnet is not detected and magnet_debounced_state is MAGNET_PRESENT
        __magnet_not_detected_loop_counter++;
        if (__magnet_not_detected_loop_counter > MAGNET_DEBOUNCE_DELAY_MILLIS) {      // > 1 sec.
          magnet_debounced_state = MAGNET_NOT_PRESENT;
          __magnet_not_detected_loop_counter = 0;
        }               
      } else {
        // magnet is not detected and magnet_debounced_state is MAGNET_NOT_PRESENT
      }
    }    
    __show_magnet_presence();

    if (single_activation_state) {      
      if (__after_activation_loop_counter++ == 0xFFFF) {
        // roll over to above MAGNET_SECOND_DETECTION_TIMEOUT_MILLIS
        __after_activation_loop_counter = MAGNET_SECOND_DETECTION_TIMEOUT_MILLIS;
      }
    }
}
