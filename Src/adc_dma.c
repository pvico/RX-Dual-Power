//######################################################################################
// adc_dma.c
// Initialization and interrupt callback functions for ADC and DMA
//
// Philippe Vico - 2021
//######################################################################################


#include "adc_dma.h"
#include "main.h"


extern uint32_t adc_values[];


//################################## Helper functions ##################################

static void __clear_all_dma_interrupt_flags() {
  DMA1->IFCR = 0x1UL;
}

//######################################################################################


//################################ Interface functions #################################

void init_adc_dma() {
    // Set interrupts and  priorities
    // ADC has higher priority than DMA
    NVIC_SetPriority(DMA1_Channel1_IRQn, 1);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    NVIC_SetPriority(ADC1_COMP_IRQn, 0);
    NVIC_EnableIRQ(ADC1_COMP_IRQn);

    // Enable peripheral clocks
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);     // ADC clock
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);     // DMA clock

    // DMA channel selection
    CLEAR_BIT(DMA1_CSELR->CSELR, (0xFUL << 0U));  // C1S bits cleared => this is needed to set specific bits in next statement
    SET_BIT(DMA1_CSELR->CSELR, 0UL);              // C1S set to 0b0001 => DMA1 Channel1            

    LL_DMA_InitTypeDef dma_init = {0};
    dma_init.PeriphOrM2MSrcAddress = (uint32_t)&ADC1->DR;
    dma_init.MemoryOrM2MDstAddress = (uint32_t)adc_values;
    dma_init.Mode = LL_DMA_MODE_CIRCULAR;
    dma_init.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
    dma_init.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
    dma_init.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_WORD;
    dma_init.NbData = 2UL;
    // dma_init.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;      // LL_DMA_DIRECTION_PERIPH_TO_MEMORY is the default 0 value
    // dma_init.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;  // LL_DMA_PERIPH_NOINCREMENT is the default 0 value
    // dma_init.PeriphRequest = LL_DMA_REQUEST_0;                   // LL_DMA_REQUEST_0 is the default 0 value
    // dma_init.Priority = LL_DMA_PRIORITY_LOW;                     // LL_DMA_PRIORITY_LOW is the default 0 value
    LL_DMA_Init(DMA1, LL_DMA_CHANNEL_1, &dma_init);
   
    // Initialize ADC
    
    // Enable voltage regulator
    LL_ADC_EnableInternalRegulator(ADC1);

    LL_ADC_InitTypeDef adc_init = {0};
    adc_init.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
    adc_init.Resolution = LL_ADC_RESOLUTION_10B;    
    // adc_init.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;   // LL_ADC_DATA_ALIGN_RIGHT is the default 0 value
    // adc_init.LowPowerMode = LL_ADC_LP_MODE_NONE;     // LL_ADC_LP_MODE_NONE is the default 0 value
    LL_ADC_Init(ADC1, &adc_init);

    // Note: we don't use a LL_ADC_REG_InitTypeDef and LL_ADC_REG_Init() because all members are at default value except ContinuousMode
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_CONTINUOUS);

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

//######################################################################################


//################################ Callback functions ##################################

void dma_callback() {
  // Note: we don't check if the interrupts are enabled like HAL does
  // as we have enabled them before starting comversions

  // If a transfer complete occurred
  if (LL_DMA_IsActiveFlag_TC1(DMA1)) {
    /* Clear the transfer complete flag */
    LL_DMA_ClearFlag_TC1(DMA1);
  }

  // If a transfer error occurred
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

//######################################################################################