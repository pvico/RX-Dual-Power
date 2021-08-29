#include "main_loop.h"
#include "led.h"
#include "magnet.h"
#include "gpio.h"
#include <stm32l021xx.h>

void loop() {

    if (is_magnet_detected()) {
      led1_extinguish();
    } else {
      led1_illuminate();
    }

    led2_illuminate();
    HAL_Delay(200);
    led2_extinguish();
    HAL_Delay(200);
}