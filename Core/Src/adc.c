#include "adc.h"


// void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
// {

//   if(adcHandle->Instance==ADC1)
//   {
//   /* USER CODE BEGIN ADC1_MspDeInit 0 */

//   /* USER CODE END ADC1_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_ADC1_CLK_DISABLE();

//     /**ADC GPIO Configuration
//     PA1     ------> ADC_IN1
//     PA4     ------> ADC_IN4
//     */
//     HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_4);

//     /* ADC1 DMA DeInit */
//     HAL_DMA_DeInit(adcHandle->DMA_Handle);

//     /* ADC1 interrupt Deinit */
//     HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);
//   /* USER CODE BEGIN ADC1_MspDeInit 1 */

//   /* USER CODE END ADC1_MspDeInit 1 */
//   }
// }
