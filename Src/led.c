
#include "gpio.h"
#include "led.h"
#include "config.h"

void led1_illuminate() {
    #ifndef DEBUG
    HAL_GPIO_WritePin(LED1_GPIO, LED1_PIN, GPIO_PIN_RESET);
    #endif
}

void led1_extinguish() {
    #ifndef DEBUG
    HAL_GPIO_WritePin(LED1_GPIO, LED1_PIN, GPIO_PIN_SET);
    #endif
}

void led2_illuminate() {
    HAL_GPIO_WritePin(LED2_GPIO, LED2_PIN, GPIO_PIN_RESET);
}

void led2_extinguish() {
    HAL_GPIO_WritePin(LED2_GPIO, LED2_PIN, GPIO_PIN_SET);
}
