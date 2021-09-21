//######################################################################################
// uart.c
// UART initialization
//
// Philippe Vico - 2021
//######################################################################################


//################################## Public functions ##################################

#include "uart.h"
#include "main.h"


extern uint32_t SystemCoreClock;


void init_uart() {
  LL_USART_InitTypeDef USART_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);

  // USART_InitStruct.BaudRate = 9600;
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

  // Auto baudrate
  // LL_USART_SetAutoBaudRateMode(USART2, LL_USART_AUTOBAUD_DETECT_ON_STARTBIT);
  // LL_USART_EnableAutoBaudRate(USART2);
  // LL_USART_EnableDirectionRx(USART2);
  // while (LL_USART_IsActiveFlag_REACK(USART2) == RESET) {}
  // while (LL_USART_IsActiveFlag_ABR(USART2) == RESET) {}
  // check error LL_USART_IsActiveFlag_ABRE
  // if error return INITIALIZE_NOT_OK
}

void configure_uart(uint32_t baudrate, uart_data_type data_type, uart_pin_inversion pin_inversion) {

  LL_USART_SetBaudRate(USART2, SystemCoreClock, LL_USART_OVERSAMPLING_16, baudrate);

  uint32_t parity, data_width, stop_bits;

  switch (data_type) {
  case _8N1:
    data_width = LL_USART_DATAWIDTH_8B;
    parity = LL_USART_PARITY_NONE;
    stop_bits = LL_USART_STOPBITS_1;
    break;
  case _9E2:
    data_width = LL_USART_DATAWIDTH_9B;
    parity = LL_USART_PARITY_EVEN;
    stop_bits = LL_USART_STOPBITS_2;
    break;
  
  default:
    data_width = LL_USART_DATAWIDTH_8B;
    parity = LL_USART_PARITY_NONE;
    stop_bits = LL_USART_STOPBITS_1;
    break;
  }

  LL_USART_SetDataWidth(USART2, data_width);
  LL_USART_SetParity(USART2, parity);
  LL_USART_SetStopBitsLength(USART2, stop_bits);

  uint32_t rx_pin_inversion, tx_pin_inversion;

  switch (pin_inversion) {
  case NO_INVERSION:
    rx_pin_inversion = LL_USART_RXPIN_LEVEL_STANDARD;
    tx_pin_inversion = LL_USART_RXPIN_LEVEL_STANDARD;
    break;
  case RX_INVERTED_TX_INVERTED:
    rx_pin_inversion = LL_USART_RXPIN_LEVEL_INVERTED;
    tx_pin_inversion = LL_USART_RXPIN_LEVEL_INVERTED;
    break;
  
  default:
    rx_pin_inversion = LL_USART_RXPIN_LEVEL_STANDARD;
    tx_pin_inversion = LL_USART_RXPIN_LEVEL_STANDARD;
    break;
  }

  LL_USART_SetRXPinLevel(USART2, rx_pin_inversion);
  LL_USART_SetTXPinLevel(USART2, tx_pin_inversion);
}

//######################################################################################