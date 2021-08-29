
#include "gpio.h"
#include "led.h"

void led1_illuminate() {
    HAL_GPIO_WritePin(LED1_GPIO, LED1_PIN, GPIO_PIN_RESET);
}

void led1_extinguish() {
    HAL_GPIO_WritePin(LED1_GPIO, LED1_PIN, GPIO_PIN_SET);
}

void led2_illuminate() {
    HAL_GPIO_WritePin(LED2_GPIO, LED2_PIN, GPIO_PIN_RESET);
}

void led2_extinguish() {
    HAL_GPIO_WritePin(LED2_GPIO, LED2_PIN, GPIO_PIN_SET);
}
