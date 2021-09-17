#include "adc.h"


ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

static void __DMA_Init(DMA_HandleTypeDef *hdma) {
  uint32_t tmp;
  /* Compute the channel index */
  /* Only one DMA: DMA1 */
  // hdma->ChannelIndex = (((uint32_t)DMA1_Channel1 - (uint32_t)DMA1_Channel1) / ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1)) << 2;
  hdma->ChannelIndex = 0;
  hdma->DmaBaseAddress = DMA1;
  // /* Change DMA peripheral state */
  // hdma->State = HAL_DMA_STATE_BUSY;
  /* Get the CR register value */
  // tmp = hdma->Instance->CCR;
  tmp = DMA1_Channel1->CCR;
  /* Clear PL, MSIZE, PSIZE, MINC, PINC, CIRC, DIR and MEM2MEM bits */
  tmp &= ((uint32_t)~(DMA_CCR_PL    | DMA_CCR_MSIZE  | DMA_CCR_PSIZE  |
                      DMA_CCR_MINC  | DMA_CCR_PINC   | DMA_CCR_CIRC   |
                      DMA_CCR_DIR   | DMA_CCR_MEM2MEM));
  /* Prepare the DMA Channel configuration */
  tmp |=  DMA_PERIPH_TO_MEMORY |
          DMA_PINC_DISABLE     |
          DMA_MINC_ENABLE      |
          DMA_PDATAALIGN_WORD  | 
          DMA_MDATAALIGN_WORD  |
          DMA_CIRCULAR         |
          DMA_PRIORITY_LOW;

  /* Write to DMA Channel CR register */
  DMA1_Channel1->CCR = tmp;
  /* Set request selection */
  if(hdma->Init.Direction != DMA_MEMORY_TO_MEMORY) {
    /* Write to DMA channel selection register */
    /* Reset request selection for DMA1 Channelx */
    DMA1_CSELR->CSELR &= ~(DMA_CSELR_C1S << (0 & 0x1cU));
    /* Configure request selection for DMA1 Channelx */
    DMA1_CSELR->CSELR |= (uint32_t) (DMA_REQUEST_0 << (0 & 0x1cU));
  }
}

static void __HAL_DMA_DeInit(DMA_HandleTypeDef *hdma)
{

  /* Disable the selected DMA Channelx */
  ((hdma)->Instance->CCR &= ~DMA_CCR_EN);

  /* Compute the channel index */
  /* DMA1 */
  hdma->ChannelIndex = (((uint32_t)hdma->Instance - (uint32_t)DMA1_Channel1) / ((uint32_t)DMA1_Channel2 - (uint32_t)DMA1_Channel1)) << 2;
  hdma->DmaBaseAddress = DMA1;

  /* Reset DMA Channel control register */
  hdma->Instance->CCR  = 0U;

  /* Clear all flags */
  hdma->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << (hdma->ChannelIndex & 0x1cU));

  /* Reset DMA channel selection register */
  /* DMA1 */
  DMA1_CSELR->CSELR &= ~(DMA_CSELR_C1S << (hdma->ChannelIndex & 0x1cU));

}


// void __HAL_ADC_MspInit() {

//   GPIO_InitTypeDef GPIO_InitStruct = {0};

//     /* ADC1 clock enable */
//     __HAL_RCC_ADC1_CLK_ENABLE();

//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     /**ADC GPIO Configuration
//     PA1     ------> ADC_IN1
//     PA4     ------> ADC_IN4
//     */
//     GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
//     GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//     /* ADC1 DMA Init */
//     /* ADC Init */
//     hdma_adc.Instance = DMA1_Channel1;
//     hdma_adc.Init.Request = DMA_REQUEST_0;
//     hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
//     hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
//     hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
//     hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//     hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//     hdma_adc.Init.Mode = DMA_CIRCULAR;
//     hdma_adc.Init.Priority = DMA_PRIORITY_LOW;
//     if (HAL_DMA_Init(&hdma_adc) != HAL_OK) {
//       Error_Handler();
//     }

//     __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc);

//     /* ADC1 interrupt Init */
//     HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0, 0);
//     HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);

// }


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


void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle) {

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_ADC1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* ADC1 DMA Init */
  /* ADC Init */
  hdma_adc.Instance = DMA1_Channel1;
  hdma_adc.Init.Request = DMA_REQUEST_0;
  hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_adc.Init.Mode = DMA_CIRCULAR;
  hdma_adc.Init.Priority = DMA_PRIORITY_LOW;
  __DMA_Init(&hdma_adc);

  (&hadc)->DMA_Handle = &(hdma_adc);
  (hdma_adc).Parent = (&hadc);

  NVIC_SetPriority(ADC1_COMP_IRQn, 0);
  NVIC_EnableIRQ(ADC1_COMP_IRQn);
}


void MX_ADC_Init(void) {
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc.Init.Resolution = ADC_RESOLUTION_10B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_160CYCLES_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;     // ENABLE ?

  HAL_ADC_MspInit(&hadc);

  // Prescaler
  ADC1->CFGR2 &= ~(ADC_CFGR2_CKMODE); 
  ADC1->CFGR2 |= ADC_CLOCK_SYNC_PCLK_DIV4; 

  ADC1->CFGR1 &= ~( ADC_CFGR1_RES);
  ADC1->CFGR1 |= ADC_RESOLUTION_10B;    
  ADC->CCR &= (uint32_t)~ADC_CCR_LFMEN;
  ADC->CCR |= 0;                          // LowPowerFrequencyMode ENABLE? then  1 << 25U
  ADC1->CR |= ADC_CR_ADVREGEN;
  ADC1->CFGR1 &= ~( ADC_CFGR1_ALIGN   |
                    ADC_CFGR1_SCANDIR |
                    ADC_CFGR1_EXTSEL  |
                    ADC_CFGR1_EXTEN   |
                    ADC_CFGR1_CONT    |
                    ADC_CFGR1_DMACFG  |
                    ADC_CFGR1_OVRMOD  |
                    ADC_CFGR1_AUTDLY  |
                    ADC_CFGR1_AUTOFF  |
                    ADC_CFGR1_DISCEN );

  ADC1->CFGR1 |= (ADC_DATAALIGN_RIGHT                     |
                  0                                       |   // ScanConvMode ADC_SCAN_DIRECTION_BACKWARD ?  then 1UL << 2U
                  (1U << 13U)                             |   // ContinuousConvMode DISABLE ? then 0
                  (1U << 1U)                              |   // DMAContinuousRequests DISABLE ? 0
                  ADC_OVR_DATA_PRESERVED                  |
                  0                                       |   // AutoDelay		ENABLE ?  then 1 << 14U
                  0);                                          // LowPowerAutoPowerOff ENABLE ?  then 1 << 15U

  // Disable OverSampling mode
  ADC1->CFGR2 &= ~ADC_CFGR2_OVSE;
  /* Clear the old sampling time */
  ADC1->SMPR &= (uint32_t)(~ADC_SMPR_SMPR);
  /* Set the new sample time */
  ADC1->SMPR |= ADC_SAMPLETIME_160CYCLES_5;

  ADC1->CHSELR |= (uint32_t)(ADC_CHANNEL_1 & ADC_CHANNEL_MASK);
  ADC1->CHSELR |= (uint32_t)(ADC_CHANNEL_4 & ADC_CHANNEL_MASK);

}


