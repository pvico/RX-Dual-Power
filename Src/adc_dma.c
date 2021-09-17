#include "adc_dma.h"
#include "adc.h"
#include "main.h"


ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;
extern uint32_t adc_values[];

void init_adc_dma() {
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


    //########################## BEGIN HAL_ADC_MspInit(&hadc);
    // Enable clocks
    SET_BIT(RCC->APB2ENR, (RCC_APB2ENR_ADC1EN));
    SET_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);
    __IO uint32_t tmpreg = READ_BIT(RCC->IOPENR, RCC_IOPENR_GPIOAEN);
    UNUSED(tmpreg); 

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    hdma_adc.Instance = DMA1_Channel1;
    hdma_adc.Init.Request = DMA_REQUEST_0;
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc.Init.Mode = DMA_CIRCULAR;
    hdma_adc.Init.Priority = DMA_PRIORITY_LOW;


    //################## BEGIN HAL_DMA_Init(&hdma_adc);
    uint32_t tmp;

    hdma_adc.ChannelIndex = 0;
    hdma_adc.DmaBaseAddress = DMA1;

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
    DMA1_Channel1->CCR = tmp;
    /* Write to DMA channel selection register */
    /* Reset request selection for DMA1 Channel1 */
    CLEAR_BIT(DMA1_CSELR->CSELR, DMA_CSELR_C1S);
    /* Configure request selection for DMA1 Channel1 */
    SET_BIT(DMA1_CSELR->CSELR, (uint32_t)DMA_REQUEST_0);
    //################## END HAL_DMA_Init(&hdma_adc);


    hdma_adc.Parent = &hadc;

    NVIC_SetPriority(ADC1_COMP_IRQn, 0);
    NVIC_EnableIRQ(ADC1_COMP_IRQn);
    //########################## END HAL_ADC_MspInit(&hadc);


    // Prescaler
    CLEAR_BIT(ADC1->CFGR2, ADC_CFGR2_CKMODE);
    SET_BIT(ADC1->CFGR2, ADC_CLOCK_SYNC_PCLK_DIV4); 

    CLEAR_BIT(ADC1->CFGR1, ADC_CFGR1_RES);
    SET_BIT(ADC1->CFGR1, ADC_RESOLUTION_10B);
    CLEAR_BIT(ADC->CCR, ADC_CCR_LFMEN);
    SET_BIT(ADC->CCR, 0);                          // LowPowerFrequencyMode ENABLE? then  1 << 25U
    SET_BIT(ADC1->CR, ADC_CR_ADVREGEN);
    CLEAR_BIT(ADC1->CFGR1, 
                    ( ADC_CFGR1_ALIGN   |
                        ADC_CFGR1_SCANDIR |
                        ADC_CFGR1_EXTSEL  |
                        ADC_CFGR1_EXTEN   |
                        ADC_CFGR1_CONT    |
                        ADC_CFGR1_DMACFG  |
                        ADC_CFGR1_OVRMOD  |
                        ADC_CFGR1_AUTDLY  |
                        ADC_CFGR1_AUTOFF  |
                        ADC_CFGR1_DISCEN ));

    SET_BIT(ADC1->CFGR1,
            (ADC_DATAALIGN_RIGHT                     |
            0                                       |   // ScanConvMode ADC_SCAN_DIRECTION_BACKWARD ?  then 1UL << 2U
            (1U << 13U)                             |   // ContinuousConvMode DISABLE ? then 0
            (1U << 1U)                              |   // DMAContinuousRequests DISABLE ? 0
            ADC_OVR_DATA_PRESERVED                  |
            0                                       |   // AutoDelay		ENABLE ?  then 1 << 14U
            0));                                          // LowPowerAutoPowerOff ENABLE ?  then 1 << 15U

    // Disable OverSampling mode
    CLEAR_BIT(ADC1->CFGR2, ADC_CFGR2_OVSE);
    /* Clear the old sampling time */
    CLEAR_BIT(ADC1->SMPR, ADC_SMPR_SMPR);
    /* Set the new sample time */
    SET_BIT(ADC1->SMPR, ADC_SAMPLETIME_160CYCLES_5);

    SET_BIT(ADC1->CHSELR, (uint32_t)(ADC_CHANNEL_1 & ADC_CHANNEL_MASK));
    SET_BIT(ADC1->CHSELR, (uint32_t)(ADC_CHANNEL_4 & ADC_CHANNEL_MASK));

    // Calibration
    SET_BIT(ADC1->CR, ADC_CR_ADCAL);
    while(READ_BIT(ADC1->CR, ADC_CR_ADCAL)) {}

    // Start ADC in DMA mode
    SET_BIT(ADC1->CFGR1, ADC_CFGR1_DMAEN);
    // Enable ADC
    SET_BIT(ADC1->CR, ADC_CR_ADEN);
    // 16 us
    for (uint8_t i = 0; i < 16; i++) {}
    // Wait for ADC ready
    while (!(READ_BIT(ADC1->ISR, ADC_FLAG_RDY))) {}
    // Clear interrupt flags
    ADC1->ISR = (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR);
    /* Enable ADC overrun interrupt */
    SET_BIT(ADC1->IER, ADC_IT_OVR);
    // Disable DMA
    CLEAR_BIT(DMA1_Channel1->CCR, DMA_CCR_EN);
    // Configure DMA addresses
    DMA1->IFCR = DMA_ISR_GIF1;
    DMA1_Channel1->CNDTR = 2;       // datalength
    DMA1_Channel1->CPAR = (uint32_t) &ADC1->DR;
    DMA1_Channel1->CMAR = (uint32_t) adc_values;
    
    // Enable transfer complete and transfer error interrupts
    SET_BIT(DMA1_Channel1->CCR, (DMA_IT_TC | DMA_IT_TE));
    // Enable DMA
    SET_BIT(DMA1_Channel1->CCR, DMA_CCR_EN);
    // Enable conversion of regular group.
    // Software start has been selected, conversion starts immediately.
    SET_BIT(ADC1->CR, ADC_CR_ADSTART);  
}

void dma_callback() {
//   uint32_t flag_it = hdma->DmaBaseAddress->ISR;
  uint32_t flag_it = DMA1->ISR;
  uint32_t source_it = DMA1_Channel1->CCR;

  /* Half Transfer Complete Interrupt management ******************************/
  if ((0U != (flag_it & (DMA_FLAG_HT1))) && (0U != (source_it & DMA_IT_HT))) {
      /* Clear the half transfer complete flag */
      DMA1->IFCR = DMA_ISR_HTIF1;
  }
  /* Transfer Complete Interrupt management ***********************************/
  else if ((0U != (flag_it & (DMA_FLAG_TC1))) && (0U != (source_it & DMA_IT_TC))) {
    if((DMA1_Channel1->CCR & DMA_CCR_CIRC) == 0U) {
      /* Disable the transfer complete and error interrupt */
        CLEAR_BIT(DMA1_Channel1->CCR, (DMA_CCR_TEIE | DMA_CCR_TCIE));
    }
    /* Clear the transfer complete flag */
    DMA1->IFCR = DMA_ISR_TCIF1;

  }
  /* Transfer Error Interrupt management **************************************/
  else if ((0U != (flag_it & (DMA_FLAG_TE1))) && (0U != (source_it & DMA_IT_TE))) {
    /* When a DMA transfer error occurs */
    /* A hardware clear of its EN bits is performed */
    /* Disable ALL DMA IT */
    CLEAR_BIT(DMA1_Channel1->CCR, (DMA_IT_TC | DMA_IT_HT | DMA_IT_TE));
    /* Clear all flags */
    DMA1->IFCR = DMA_ISR_GIF1;
  }
}

void adc_callback() {
  /* ========== Check End of Conversion flag for regular group ========== */
  if( READ_BIT(ADC1->ISR, ADC_FLAG_EOC) && READ_BIT(ADC1->IER, ADC_IT_EOC) || 
      READ_BIT(ADC1->ISR, ADC_FLAG_EOS) && READ_BIT(ADC1->IER, ADC_IT_EOS)) {
    
      ADC1->ISR = (ADC_FLAG_EOC | ADC_FLAG_EOS);
  }
  /* ========== Check Overrun flag ========== */
  if(READ_BIT(ADC1->ISR, ADC_FLAG_OVR) && READ_BIT(ADC1->IER, ADC_IT_OVR)) {
      /* Clear ADC overrun flag */
      ADC1->ISR =  ADC_FLAG_OVR;
  }
}