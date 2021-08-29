#include "main_loop.h"
#include "led.h"
#include "magnet.h"
#include "gpio.h"
#include <stm32l021xx.h>
// #include "dma.h"
#include "adc.h"

// DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc;
uint32_t result;

void loop() {

  if (is_magnet_detected()) {
    led1_extinguish();
  } else {
    led1_illuminate();
  }

  
  if (HAL_ADC_PollForConversion(&hadc, 200) == HAL_OK) {
    result = HAL_ADC_GetValue(&hadc);
  }

  led2_illuminate();
  HAL_Delay(result);
  led2_extinguish();
  HAL_Delay(500);

  // HAL_ADC_Start_DMA(&hadc1, &dma_result, 1);
}