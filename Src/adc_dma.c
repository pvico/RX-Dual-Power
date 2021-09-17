#include "adc_dma.h"
#include "main.h"
#include "stm32l021xx.h"


extern uint32_t adc_values[];

void init_adc_dma() {
    //########################## BEGIN HAL_ADC_MspInit(&hadc);
    // Enable clocks
    SET_BIT(RCC->APB2ENR, (0x1UL << 9U));
    SET_BIT(RCC->IOPENR, (0x1UL << 0U));
    // READ_BIT(RCC->IOPENR, (0x1UL << 0U));

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = (0x0002U)|(0x0010U);      // pins 1 & 4
    GPIO_InitStruct.Mode = (0x3UL << 0U);           // analog
    GPIO_InitStruct.Pull = (0x00000000U);           // no pull
    LL_GPIO_Init(((GPIO_TypeDef *) (((0x40000000UL) + 0x10000000UL) + 0x00000000UL)), &GPIO_InitStruct);

    //################## BEGIN HAL_DMA_Init(&hdma_adc);
    uint32_t tmp;

    tmp = DMA1_Channel1->CCR;
    /* Clear PL, MSIZE, PSIZE, MINC, PINC, CIRC, DIR and MEM2MEM bits */
    tmp &= ((uint32_t)~((0x3UL << 12U)  | (0x3UL << 10U)  | (0x3UL << 8U)  |
                        (0x1UL << 7U)  | (0x1UL << 6U)   | (0x1UL << 5U)   |
                        (0x1UL << 4U)   | (0x1UL << 14U)));
    /* Prepare the DMA Channel configuration */
    tmp |=  0U              |              // DMA_PERIPH_TO_MEMORY
            0U              |              // DMA_PINC_DISABLE
            (0x1UL << 7U)   |              // DMA_MINC_ENABLE
            (0x2UL << 8U)   |              // DMA_PDATAALIGN_WORD
            (0x2UL << 10U)  |              // DMA_MDATAALIGN_WORD
            (0x1UL << 5U)   |              // DMA_CIRCULAR
            0U;                            // DMA_PRIORITY_LOW
    DMA1_Channel1->CCR = tmp;
    /* Write to DMA channel selection register */
    /* Reset request selection for DMA1 Channel1 */
    CLEAR_BIT(DMA1_CSELR->CSELR, (0xFUL << 0U));
    /* Configure request selection for DMA1 Channel1 */
    SET_BIT(DMA1_CSELR->CSELR, (uint32_t)0U);
    //################## END HAL_DMA_Init(&hdma_adc);

    NVIC_SetPriority(ADC1_COMP_IRQn, 0);
    NVIC_EnableIRQ(ADC1_COMP_IRQn);
    //########################## END HAL_ADC_MspInit(&hadc);


    // Prescaler
    CLEAR_BIT(ADC1->CFGR2, (0x3UL << 30U));
    SET_BIT(ADC1->CFGR2, ((0x2UL << 30U))); 

    CLEAR_BIT(ADC1->CFGR1, (0x3UL << 3U));
    SET_BIT(ADC1->CFGR1, ((0x1UL << 3U)));      // resolution 10bit
    CLEAR_BIT(ADC->CCR, (0x1UL << 25U));
    SET_BIT(ADC->CCR, 0UL);                          // LowPowerFrequencyMode ENABLE? then  1 << 25U
    SET_BIT(ADC1->CR, (0x1UL << 28U));
    CLEAR_BIT(ADC1->CFGR1, 
                    (   (0x1UL << 5U)  |
                        (0x1UL << 2U)  |
                        (0x7UL << 6U)  |
                        (0x3UL << 10U) |
                        (0x1UL << 13U) |
                        (0x1UL << 1U)  |
                        (0x1UL << 12U) |
                        (0x1UL << 14U) |
                        (0x1UL << 15U) |
                        (0x1UL << 16U)  ));

    SET_BIT(ADC1->CFGR1,
            (0U                                     |   // Data Align Right
            0                                       |   // ScanConvMode ADC_SCAN_DIRECTION_BACKWARD ?  then 1UL << 2U
            (1U << 13U)                             |   // ContinuousConvMode DISABLE ? then 0
            (1U << 1U)                              |   // DMAContinuousRequests DISABLE ? 0
            0U                  |   // ADC_OVR_DATA_PRESERVED
            0                                       |   // AutoDelay		ENABLE ?  then 1 << 14U
            0));                                          // LowPowerAutoPowerOff ENABLE ?  then 1 << 15U

    // Disable OverSampling mode
    CLEAR_BIT(ADC1->CFGR2, (0x1UL << 0U));
    /* Clear the old sampling time */
    CLEAR_BIT(ADC1->SMPR, (0x7UL << 0U));
    /* Set the new sample time */
    SET_BIT(ADC1->SMPR, ((0x7UL << 0U)));        // ADC_SAMPLETIME_160CYCLES_5

    SET_BIT(ADC1->CHSELR, (uint32_t)(((uint32_t)((0x1UL << 1U)) | (0x01UL << 26U)) & (0x0007FFFFU)));     // Channel 1
    SET_BIT(ADC1->CHSELR, (uint32_t)(((uint32_t)((0x1UL << 4U))| (0x04UL << 26U)) & (0x0007FFFFU)));      // Channel 4

    // Calibration
    SET_BIT(ADC1->CR, (0x1UL << 31U));
    while(READ_BIT(ADC1->CR, (0x1UL << 31U))) {}

    // Start ADC in DMA mode
    SET_BIT(ADC1->CFGR1, (0x1UL << 0U));
    // Enable ADC
    SET_BIT(ADC1->CR, (0x1UL << 0U));
    // 16 us
    for (uint8_t i = 0; i < 16; i++) {}
    // Wait for ADC ready
    while (!(READ_BIT(ADC1->ISR, (0x1UL << 0U)))) {}
    // Clear interrupt flags
    ADC1->ISR = ((0x1UL << 2U) | (0x1UL << 3U)| (0x1UL << 4U));
    /* Enable ADC overrun interrupt */
    SET_BIT(ADC1->IER, (0x1UL << 4U));
    // Disable DMA
    CLEAR_BIT(DMA1_Channel1->CCR, (0x1UL << 0U));
    // Configure DMA addresses
    DMA1->IFCR = (0x1UL << 0U);
    DMA1_Channel1->CNDTR = 2;       // datalength
    DMA1_Channel1->CPAR = (uint32_t) &ADC1->DR;
    DMA1_Channel1->CMAR = (uint32_t) adc_values;
    
    // Enable transfer complete and transfer error interrupts
    SET_BIT(DMA1_Channel1->CCR, ((0x1UL << 1U) | (0x1UL << 3U)));
    // Enable DMA
    SET_BIT(DMA1_Channel1->CCR, (0x1UL << 0U));
    // Enable conversion of regular group.
    // Software start has been selected, conversion starts immediately.
    SET_BIT(ADC1->CR, (0x1UL << 2U));  
}

void dma_callback() {
//   uint32_t flag_it = hdma->DmaBaseAddress->ISR;
  uint32_t flag_it = DMA1->ISR;
  uint32_t source_it = DMA1_Channel1->CCR;

  /* Half Transfer Complete Interrupt management ******************************/
  if ((0U != (flag_it & ((0x1UL << 2U)))) && (0U != (source_it & (0x1UL << 2U)))) {
      /* Clear the half transfer complete flag */
      DMA1->IFCR = (0x1UL << 2U);
  }
  /* Transfer Complete Interrupt management ***********************************/
  else if ((0U != (flag_it & ((0x1UL << 1U)))) && (0U != (source_it & (0x1UL << 1U)))) {
    if((DMA1_Channel1->CCR & (0x1UL << 5U)) == 0U) {
      /* Disable the transfer complete and error interrupt */
        CLEAR_BIT(DMA1_Channel1->CCR, ((0x1UL << 3U) | (0x1UL << 1U)));
    }
    /* Clear the transfer complete flag */
    DMA1->IFCR = (0x1UL << 1U);

  }
  /* Transfer Error Interrupt management **************************************/
  else if ((0U != (flag_it & ((0x1UL << 3U)))) && (0U != (source_it & (0x1UL << 3U)))) {
    /* When a DMA transfer error occurs */
    /* A hardware clear of its EN bits is performed */
    /* Disable ALL DMA IT */
    CLEAR_BIT(DMA1_Channel1->CCR, ((0x1UL << 1U) | (0x1UL << 2U) | (0x1UL << 3U)));
    /* Clear all flags */
    DMA1->IFCR = (0x1UL << 0U);
  }
}

void adc_callback() {
  /* ========== Check End of Conversion flag for regular group ========== */
  if( (READ_BIT(ADC1->ISR, (0x1UL << 2U)) && READ_BIT(ADC1->IER, (0x1UL << 2U))) || 
      (READ_BIT(ADC1->ISR, (0x1UL << 3U)) && READ_BIT(ADC1->IER, (0x1UL << 3U)))) {
    
      ADC1->ISR = ((0x1UL << 2U) | (0x1UL << 3U));
  }
  /* ========== Check Overrun flag ========== */
  if(READ_BIT(ADC1->ISR, (0x1UL << 4U)) && READ_BIT(ADC1->IER, (0x1UL << 4U))) {
      /* Clear ADC overrun flag */
      ADC1->ISR =  (0x1UL << 4U);
  }
}