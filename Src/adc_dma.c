#include "adc_dma.h"
#include "main.h"
#include "stm32l021xx.h"


extern uint32_t adc_values[];

static void __clear_all_dma_interrupt_flags() {
  DMA1->IFCR = 0x1UL;
}

void init_adc_dma() {
    // Set ADC pins
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = V_MAIN_Pin | V_STBY_Pin;
    GPIO_InitStruct.OutputType = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(S_PORT_GPIO_Port, &GPIO_InitStruct);

    // Set interrupts and  priorities
    // ADC has higher priority than DMA
    NVIC_SetPriority(DMA1_Channel1_IRQn, 1);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    NVIC_SetPriority(ADC1_COMP_IRQn, 0);
    NVIC_EnableIRQ(ADC1_COMP_IRQn);

    // Enable peripheral clocks
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);     // ADC clock
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);     // DMA clock

    // Initialize DMA

    // Configuration
    CLEAR_BIT(DMA1_Channel1->CCR, 
              ( (0x1UL << 4U)   |     // DIR bits cleared => read from peripheral
                (0x1UL << 6U)   |     // PINC bit cleared => peripheral increement disabled
                (0x3UL << 8U)   |     // PSIZE bits cleared => this is needed to set specific bits in next statement
                (0x3UL << 10U)  |     // MSIZE bits cleared => this is needed to set specific bits in next statement
                (0x3UL << 12U)  |     // PL bits cleared => priority level low
                (0x1UL << 14U)));     // MEM2MEM bit cleared => memory-to-memorymode disabled
    SET_BIT(DMA1_Channel1->CCR,
            ((0x1UL << 5U)      |     // CIRC bis set =>circular mode enabled
            (0x1UL << 7U)       |     // MINC bis set => memory increement enabled
            (0x2UL << 8U)       |     // PSIZE bis set to 0b010 => transfer 32 bits from peripheral
            (0x2UL << 10U)));         // MSIZE bis set to 0b010 => transfer 32 bits to memory

    // DMA channel selection
    CLEAR_BIT(DMA1_CSELR->CSELR, (0xFUL << 0U));  // C1S bits cleared => this is needed to set specific bits in next statement
    SET_BIT(DMA1_CSELR->CSELR, 0UL);              // C1S set to 0b0001 => DMA1 Channel1
   
    // Initialize ADC
    
    // Enable voltage regulator
    LL_ADC_EnableInternalRegulator(ADC1);

    CLEAR_BIT(ADC1->CFGR1, 
              ( (0x3UL << 3U)  |      // RES bits cleared => this is needed to set specific bits in next statement
                (0x1UL << 5U)  |      // ALIGN bit cleared => right alignment
                (0x1UL << 2U)  |      // SCANDIR bit cleared => forward scan
                (0x7UL << 6U)  |      // EXTSEL bits cleared => trigger source software
                (0x3UL << 10U) |      // EXTEN bits cleared => hardware trigger detection disabled (conversions started by software)
                (0x1UL << 12U) |      // OVRMOD bit cleared => ADC_DR register is preserved with the old data when an overrun is detected
                (0x1UL << 14U) |      // WAIT bit cleared => Wait conversion mode of
                (0x1UL << 15U) |      // AUTOFF bit cleared => Auto-off mode disabled
                (0x1UL << 16U)  ));   // DISCEN bit cleared => Discontinuous mode disabled
    SET_BIT(ADC1->CFGR1,
            ( (1U << 3U)       |      // RES bits set to 0b01 => resolution 10 bits
              (1U << 13U)      |      // CONT bit set => continuous conversion mode ENABLE
              (1U << 1U)));           // DMACFG bit set => DMA circular mode

    CLEAR_BIT(ADC1->CFGR2, 
              ( (0x1UL << 0U)  |      // OVSE bit cleared => oversampler disabled
                (0x3UL << 30U)));     // CKMODE bits cleared => this is needed to set specific bits in next statement
    SET_BIT(ADC1->CFGR2,
            ( (2U << 30U)));          // CKMODE bits set to 0b10 => synchronous clock mode, APB clock divided by 4

    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_160CYCLES_5);

    LL_ADC_REG_SetSequencerChannels(ADC1, (LL_ADC_CHANNEL_1 | LL_ADC_CHANNEL_4));

    // Calibration
    LL_ADC_StartCalibration(ADC1);
    while(LL_ADC_IsCalibrationOnGoing(ADC1)) {}

    // Start ADC in DMA mode
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);

    // Enable ADC
    LL_ADC_Enable(ADC1);

    // Wait 1 us
    for (uint8_t i = 0; i < 16; i++) {}

    // Wait for ADC ready
    while (!LL_ADC_IsActiveFlag_ADRDY(ADC1)) {}

    // Clear interrupt flags
    LL_ADC_ClearFlag_EOC(ADC1);
    LL_ADC_ClearFlag_EOS(ADC1);
    LL_ADC_ClearFlag_OVR(ADC1);

    /* Enable ADC overrun interrupt */
    LL_ADC_EnableIT_OVR(ADC1);

    // Disable DMA
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);

    // Configure DMA addresses
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, 2);
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)&ADC1->DR, (uint32_t)adc_values, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    // Clear all interrupt flags
    __clear_all_dma_interrupt_flags();
    // Enable transfer complete and transfer error interrupts
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);

    // Enable DMA
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

    // Start ADC conversions
    LL_ADC_REG_StartConversion(ADC1);
}

void dma_callback() {
  // Note: we don't check if the interrupts are enabled like HAL does
  // as we have enabled it before starting comversions

  //= if a transfer complete occurred
  if (LL_DMA_IsActiveFlag_TC1(DMA1)) {
    /* Clear the transfer complete flag */
    LL_DMA_ClearFlag_TC1(DMA1);
  }

  //= if a transfer error occurred
  else if (LL_DMA_IsActiveFlag_TE1(DMA1)) {
    // When a DMA transfer error occurs, its EN bits are cleared by the hardware
    /* Disable all DMA IT */
    LL_DMA_DisableIT_TC(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_DisableIT_HT(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_DisableIT_TE(DMA1, LL_DMA_CHANNEL_1);
    /* Clear all flags */
    __clear_all_dma_interrupt_flags();
    //Re-enable interrupts
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);
    // Re-enable DMA
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
  }
}

void adc_callback() {
  // if end of sequence of channels conversion interrupt occurred
  if (LL_ADC_IsActiveFlag_EOS(ADC1)) {
    LL_ADC_ClearFlag_EOS(ADC1);
  }
  
  // If overrun interrupt occurred
  if (LL_ADC_IsActiveFlag_OVR(ADC1)) {
    /* Clear ADC overrun flag */
    LL_ADC_ClearFlag_OVR(ADC1);
  }
}