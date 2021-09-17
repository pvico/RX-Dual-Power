#include "voltage_sensor.h"
#include "power_source.h"
#include "led.h"
#include "adc.h"
#include "dma.h"
// #include <stm32l0xx_ll_adc.h>
#include <stm32l0xx_ll_dma.h>
#include <stm32l0xx_ll_bus.h>
#include <stdint.h>


extern Power_Source main_power_source;
extern Power_Source stby_power_source;

static uint32_t __adc_values[2] = {0};


initialization_result init_voltage_sensors() {

// #################### Working HAL code ####################
    // Calibration
    SET_BIT(hadc.Instance->CR, ADC_CR_ADCAL);
    while(READ_BIT(hadc.Instance->CR, ADC_CR_ADCAL)) {}

    // Start ADC in DMA mode
    SET_BIT(hadc.Instance->CFGR1, ADC_CFGR1_DMAEN);
    // Enable ADC
    SET_BIT(hadc.Instance->CR, ADC_CR_ADEN);
    // 16 us
    for (uint8_t i = 0; i < 16; i++) {}
    // Wait for ADC ready
    while (!(READ_BIT(hadc.Instance->ISR, ADC_FLAG_RDY))) {}
    // Clear interrupt flags
    hadc.Instance->ISR = (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR);
    /* Enable ADC overrun interrupt */
    SET_BIT(hadc.Instance->IER, ADC_IT_OVR);
    // Disable DMA
    CLEAR_BIT(hadc.DMA_Handle->Instance->CCR, DMA_CCR_EN);
    // Configure DMA addresses
    hadc.DMA_Handle->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << (hadc.DMA_Handle->ChannelIndex & 0x1cU));
    hadc.DMA_Handle->Instance->CNDTR = 2;       // datalength
    hadc.DMA_Handle->Instance->CPAR = (uint32_t) &hadc.Instance->DR;
    hadc.DMA_Handle->Instance->CMAR = (uint32_t) __adc_values;
    // Enable transfer complete and transfer error interrupts
    SET_BIT(hadc.DMA_Handle->Instance->CCR, (DMA_IT_TC | DMA_IT_TE));
    // Enable DMA
    SET_BIT(hadc.DMA_Handle->Instance->CCR, DMA_CCR_EN);
    // Enable conversion of regular group.
    // Software start has been selected, conversion starts immediately.
    SET_BIT(hadc.Instance->CR, ADC_CR_ADSTART);
//##########################################################

// //####################### LL code ##########################
// // // Calibration
//     LL_ADC_StartCalibration(ADC1);
//     while (LL_ADC_IsCalibrationOnGoing(ADC1)) {}

// // Start ADC in DMA mode
//     SET_BIT(ADC1->CFGR1, ADC_CFGR1_DMAEN);
//     // Enable ADC
//     SET_BIT(ADC1->CR, ADC_CR_ADEN);
//     // 16 us
//     for (uint8_t i = 0; i < 16; i++) {}
//     // while (!(READ_BIT(hadc.Instance->ISR, ADC_FLAG_RDY))) {}
//     while (!LL_ADC_IsActiveFlag_ADRDY(ADC1)) {}

//     // Clear interrupt flags
//     // hadc.Instance->ISR = (ADC_FLAG_EOC | ADC_FLAG_EOS | ADC_FLAG_OVR);
//     LL_ADC_ClearFlag_EOC(ADC1);
//     LL_ADC_ClearFlag_EOS(ADC1);
//     LL_ADC_ClearFlag_OVR(ADC1);
      
//     // /* Enable ADC overrun interrupt */
//     // SET_BIT(hadc.Instance->IER, ADC_IT_OVR);
//     LL_ADC_EnableIT_OVR(ADC1);

//     // // Disable DMA
//     // CLEAR_BIT(hadc.DMA_Handle->Instance->CCR, DMA_CCR_EN);
//     LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_1);



// //     // // Configure DMA addresses
// //     // hadc.DMA_Handle->DmaBaseAddress->IFCR = (DMA_ISR_GIF1 << (hadc.DMA_Handle->ChannelIndex & 0x1cU));
// //     // hadc.DMA_Handle->Instance->CNDTR = 2;       // datalength
// //     // hadc.DMA_Handle->Instance->CPAR = (uint32_t) &hadc.Instance->DR;
// //     // hadc.DMA_Handle->Instance->CMAR = (uint32_t) __adc_values;
//     LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)ADC1->DR, (uint32_t)__adc_values, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
//     LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, 2);
// //     // // Enable transfer complete and transfer error interrupts
// //     // SET_BIT(hadc.DMA_Handle->Instance->CCR, (DMA_IT_TC | DMA_IT_TE));



//     // // Enable DMA
//     // SET_BIT(hadc.DMA_Handle->Instance->CCR, DMA_CCR_EN);
//     LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

// //     // /* Enable conversion of regular group.                                  */
// //     // /* If software start has been selected, conversion starts immediately.  */
// //     // /* If external trigger has been selected, conversion will start at next */
// //     // /* trigger event.                                                       */
// //     // SET_BIT(hadc.Instance->CR, ADC_CR_ADSTART);
//     SET_BIT(ADC1->CR, ADC_CR_ADSTART);
// //     // LL_ADC_REG_StartConversion(ADC1);

// //##########################################################

    return INITIALIZE_OK;
}

static uint16_t __main_voltage() {
    return __adc_values[0] + CORRECTION_VALUE;
}

static uint16_t __stby_voltage() {
    return __adc_values[1] + CORRECTION_VALUE;
}

uint16_t voltage_ADC_to_millivolts(uint16_t adc_value) {
    return  20 * adc_value;
}

void voltage_to_str(uint32_t voltage, uint8_t *buffer) {
    uint8_t units;
    uint8_t decimals;

    // Real conversion ratio is 0.0198840816
    // Rounded to 0.02  = 1/50 because we don't want any floating point
    // calculation as this would significantly increase the hex file size
    // and flash memory use
    units = voltage / 50;
    decimals = (voltage % 50) * 2;

    buffer[0] = units < 10 ? ' ' : 48 + units / 10;
    buffer[1] = 48 + units % 10;
    buffer[2] = '.';
    buffer[3] = 48 + decimals / 10;
    buffer[4] = 48 + decimals % 10;
    buffer[5] = 'V';
}


void voltage_sensor_loop() {
    uint16_t main_voltage = __main_voltage();
    uint16_t stby_voltage = __stby_voltage();

    main_power_source.current_voltage_ADC_value = main_voltage;
    stby_power_source.current_voltage_ADC_value = stby_voltage;

    static uint16_t main_16ms_sum = 0;
    static uint8_t main_16ms_counter = 0;

    main_16ms_sum += main_voltage;
    main_16ms_counter++;
    if (main_16ms_counter == 16) {
        main_16ms_counter = 0;
        main_power_source.last_16ms_average_voltage_ADC_value = main_16ms_sum >> 4;    // divide by 16
        main_16ms_sum = 0;
    }

    static uint16_t stby_16ms_sum = 0;
    static uint8_t stby_16ms_counter = 0;

    stby_16ms_sum += stby_voltage;
    stby_16ms_counter++;
    if (stby_16ms_counter == 16) {
        stby_16ms_counter = 0;
        stby_power_source.last_16ms_average_voltage_ADC_value = stby_16ms_sum >> 4;
        stby_16ms_sum = 0;
    }
    
    static uint32_t main_16s_sum = 0;
    static uint16_t main_16s_counter = 0;

    main_16s_sum += main_voltage;
    main_16s_counter++;
    if (main_16s_counter == 0x4000) {       // = 16384 => 16.384 sec.
        main_16s_counter = 0;
        main_power_source.last_16s_average_voltage_ADC_value = main_16s_sum >> 14;      // divide by 16384
        main_16s_sum = 0;
    }

    static uint32_t stby_16s_sum = 0;
    static uint16_t stby_16s_counter = 0;
    
    stby_16s_sum += stby_voltage;
    stby_16s_counter++;
    if (stby_16s_counter == 0x4000) {
        stby_16s_counter = 0;
        stby_power_source.last_16s_average_voltage_ADC_value = stby_16s_sum >> 14;
        stby_16s_sum = 0;
    }
}

void dma_callback() {
//   /* Check whether DMA transfer complete caused the DMA interruption */
//   if(LL_DMA_IsActiveFlag_TC1(DMA1) == 1)
//   {
//     /* Clear flag DMA transfer complete interrupt */
//     LL_DMA_ClearFlag_TC1(DMA1);
    
//     /* Call interruption treatment function */
//     // ___dma_transfer_complete();
//   }
  
//   /* Check whether DMA transfer error caused the DMA interruption */
//   if(LL_DMA_IsActiveFlag_TE1(DMA1) == 1)
//   {
//     /* Clear flag DMA transfer error */
//     LL_DMA_ClearFlag_TE1(DMA1);
    
//     /* Call interruption treatment function */
//     // ___dma_transfer_error();
//   }
}

void adc_callback() {
//   /* Check whether ADC group regular end of sequence conversions caused       */
//   /* the ADC interruption.                                                    */
//   if(LL_ADC_IsActiveFlag_EOS(ADC1) != 0)
//   {
//     /* Clear flag ADC group regular end of sequence conversions */
//     LL_ADC_ClearFlag_EOS(ADC1);
    
//     /* Call interruption treatment function */
//     // __adc_sequence_conversion_complete();
//   }
  
//   /* Check whether ADC group regular overrun caused the ADC interruption */
//   if(LL_ADC_IsActiveFlag_OVR(ADC1) != 0)
//   {
//     /* Clear flag ADC group regular overrun */
//     LL_ADC_ClearFlag_OVR(ADC1);
    
//     /* Call interruption treatment function */
//     // __adc_overrun_error();
//   }
}
