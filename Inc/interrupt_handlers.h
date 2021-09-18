#ifndef __STM32L0xx_IT_H
#define __STM32L0xx_IT_H

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI0_1_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void ADC1_COMP_IRQHandler(void);
void TIM21_IRQHandler(void);
void USART2_IRQHandler(void);

#endif /* __STM32L0xx_IT_H */

