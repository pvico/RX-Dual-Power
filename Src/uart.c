//######################################################################################
// uart.c
// UART initialization
//
// Philippe Vico - 2021
//######################################################################################


//################################ Interface functions #################################

#include "uart.h"
#include "main.h"


void init_uart() {
  LL_USART_InitTypeDef USART_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);

  USART_InitStruct.BaudRate = 57600;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_SetRXPinLevel(USART2, LL_USART_RXPIN_LEVEL_INVERTED);
  LL_USART_SetTXPinLevel(USART2, LL_USART_TXPIN_LEVEL_INVERTED);
  LL_USART_DisableOverrunDetect(USART2);
  LL_USART_ConfigHalfDuplexMode(USART2);
  LL_USART_Enable(USART2);  
}

//######################################################################################