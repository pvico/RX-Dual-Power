//######################################################################################
// interrupt_handlers.c
// Interrupt handler functions. Implementation of the functions referenced by the vector
// table in the assembler bootup caode
//
// Philippe Vico - 2021
//######################################################################################


#include "main.h"
#include <stdbool.h>
#include "voltage_sensor.h"
#include "telemetry.h"
#include "s_port.h"
#include "led.h"
#include "adc_dma.h"


extern volatile bool timer_flag;
extern volatile uint32_t system_tick;
extern bool uart_in_config_mode;


// Any function present here must be present in the vector table in startup_stm32l021xx.s 


void NMI_Handler(void) {
  while (1) {}
}

void HardFault_Handler(void) {
  while (1) {}
}

void SVC_Handler(void) {}

void PendSV_Handler(void) {}

void SysTick_Handler(void) {
  timer_flag = true;
  system_tick++;
}

void EXTI0_1_IRQHandler(void) {
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET) {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
  }
}

void DMA1_Channel1_IRQHandler(void) {
  dma_callback();
}

void ADC1_COMP_IRQHandler(void) {
  adc_callback();
}

void TIM21_IRQHandler(void) {
  s_port_response_timer_callback();
}

void USART2_IRQHandler(void) {
  uart_receive_byte_callback();
}

