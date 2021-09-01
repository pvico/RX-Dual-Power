#include "magnet.h"
#include "led.h"
#include <stdint.h>
#include "config.h"
#include "console.h"


static enum magnet_states magnet_debounced_state = MAGNET_NOT_PRESENT;


static bool __is_magnet_detected() {
    return HAL_GPIO_ReadPin(MAGNET_GPIO, MAGNET_PIN) == MAGNET_PRESENT_PIN_STATE;
}

static void __show_magnet_present();


static uint16_t __loop_counter = 0;
static uint16_t __magnet_not_present_loop_counter = 0;
void magnet_loop() {
    if (__is_magnet_detected()) {
      if (magnet_debounced_state == MAGNET_NOT_PRESENT) { 
        // magnet is detected and magnet_debounced_state is MAGNET_NOT_PRESENT
        // we are counting...
        if (__loop_counter > MAGNET_DEBOUNCE_DELAY_MILLIS) {      // > 1 sec.
          magnet_debounced_state = MAGNET_PRESENT;
        }
      } else {
        // magnet is detected and magnet_debounced_state is MAGNET_PRESENT
        __loop_counter = 0;
      }
    } else {
      // magnet is not detected
      if (magnet_debounced_state == MAGNET_PRESENT) {
        // magnet is not detected and magnet_debounced_state is MAGNET_PRESENT
        // we are counting... 
        if (__loop_counter > MAGNET_DEBOUNCE_DELAY_MILLIS) {      // > 1 sec.
          magnet_debounced_state = MAGNET_NOT_PRESENT;
        }               
      } else {
        // magnet is not detected and magnet_debounced_state is MAGNET_NOT_PRESENT
        __loop_counter = 0;
      }
    }    
    __show_magnet_present();

    __loop_counter++;
}


extern enum led_states led2_state;
static enum led_states __led2_previous_state;

void __show_magnet_present() {
  if (magnet_debounced_state == MAGNET_PRESENT) {
    if (led2_state != BLINK_FAST) {
      __led2_previous_state = led2_state;
      console_print_magnet_present();
    }
    led2_state = BLINK_FAST;
  } else {
    led2_state = __led2_previous_state;
  }
}