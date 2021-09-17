// /**
//   ******************************************************************************
//   * @file    adc.c
//   * @brief   This file provides code for the configuration
//   *          of the ADC instances.
//   ******************************************************************************
//   * @attention
//   *
//   * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
//   * All rights reserved.</center></h2>
//   *
//   * This software component is licensed by ST under BSD 3-Clause license,
//   * the "License"; You may not use this file except in compliance with the
//   * License. You may obtain a copy of the License at:
//   *                        opensource.org/licenses/BSD-3-Clause
//   *
//   ******************************************************************************
//   */

// /* Includes ------------------------------------------------------------------*/
// #include "adc.h"

// /* USER CODE BEGIN 0 */

// /* USER CODE END 0 */

// /* ADC init function */
// void MX_ADC_Init(void)
// {

//   /* USER CODE BEGIN ADC_Init 0 */

//   /* USER CODE END ADC_Init 0 */

//   LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
//   LL_ADC_InitTypeDef ADC_InitStruct = {0};

//   LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

//   /* Peripheral clock enable */
//   LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

//   LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
//   /**ADC GPIO Configuration
//   PA1   ------> ADC_IN1
//   PA4   ------> ADC_IN4
//   */
//   GPIO_InitStruct.Pin = V_MAIN_Pin;
//   GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
//   GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//   LL_GPIO_Init(V_MAIN_GPIO_Port, &GPIO_InitStruct);

//   GPIO_InitStruct.Pin = V_STBY_Pin;
//   GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
//   GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//   LL_GPIO_Init(V_STBY_GPIO_Port, &GPIO_InitStruct);

//   /* ADC DMA Init */

//   /* ADC Init */
//   LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMA_REQUEST_0);

//   LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

//   LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);

//   LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);

//   LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);

//   LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);

//   LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_WORD);

//   LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_WORD);

//   /* ADC interrupt Init */
//   NVIC_SetPriority(ADC1_COMP_IRQn, 0);
//   NVIC_EnableIRQ(ADC1_COMP_IRQn);

//   /* USER CODE BEGIN ADC_Init 1 */

//   /* USER CODE END ADC_Init 1 */
//   /** Configure Regular Channel
//   */
//   LL_ADC_REG_SetSequencerChAdd(ADC1, LL_ADC_CHANNEL_1);
//   /** Configure Regular Channel
//   */
//   LL_ADC_REG_SetSequencerChAdd(ADC1, LL_ADC_CHANNEL_4);
//   /** Common config
//   */
//   ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
//   ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
//   ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
//   ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
//   ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_PRESERVED;
//   LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
//   LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_160CYCLES_5);
//   LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);
//   LL_ADC_REG_SetSequencerScanDirection(ADC1, LL_ADC_REG_SEQ_SCAN_DIR_FORWARD);
//   LL_ADC_SetCommonFrequencyMode(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_FREQ_MODE_HIGH);
//   LL_ADC_DisableIT_EOC(ADC1);
//   LL_ADC_DisableIT_EOS(ADC1);
//   ADC_InitStruct.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV4;
//   ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_10B;
//   ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
//   ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
//   LL_ADC_Init(ADC1, &ADC_InitStruct);

//   /* Enable ADC internal voltage regulator */
//   LL_ADC_EnableInternalRegulator(ADC1);
//   /* Delay for ADC internal voltage regulator stabilization. */
//   /* Compute number of CPU cycles to wait for, from delay in us. */
//   /* Note: Variable divided by 2 to compensate partially */
//   /* CPU processing cycles (depends on compilation optimization). */
//   /* Note: If system core clock frequency is below 200kHz, wait time */
//   /* is only a few CPU processing cycles. */
//   uint32_t wait_loop_index;
//   wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
//   while(wait_loop_index != 0)
//   {
//     wait_loop_index--;
//   }
//   /* USER CODE BEGIN ADC_Init 2 */

//   /* USER CODE END ADC_Init 2 */

// }

// /* USER CODE BEGIN 1 */

// /* USER CODE END 1 */

// /************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
