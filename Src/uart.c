//######################################################################################
// uart.c
// UART initialization
//
// Philippe Vico - 2021
//######################################################################################


//################################## Public functions ##################################

#include "uart.h"
#include "main.h"
#include "led.h"


extern uint32_t SystemCoreClock;


void init_uart() {
  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);

  LL_USART_SetHWFlowCtrl(USART2, LL_USART_HWCONTROL_NONE);
  LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);
  LL_USART_SetOverSampling(USART2, LL_USART_OVERSAMPLING_16);
  LL_USART_DisableOverrunDetect(USART2);
  LL_USART_ConfigHalfDuplexMode(USART2);
  configure_uart(9600, BYTE_FORMAT_8N1, NO_INVERSION);    // Configure initially for serial configuration device

  // TODO ?
  // Auto baudrate
  // LL_USART_SetAutoBaudRateMode(USART2, LL_USART_AUTOBAUD_DETECT_ON_STARTBIT);
  // LL_USART_EnableAutoBaudRate(USART2);
  // LL_USART_EnableDirectionRx(USART2);
  // while (LL_USART_IsActiveFlag_REACK(USART2) == RESET) {}
  // while (LL_USART_IsActiveFlag_ABR(USART2) == RESET) {}
  // check error LL_USART_IsActiveFlag_ABRE
  // if error return INITIALIZE_NOT_OK
}

void configure_uart(uint32_t baudrate, uart_byte_format byte_format, uart_pin_inversion pin_inversion) {

  if (LL_USART_IsEnabled(USART2)) {
    leds_show_error_infinite_loop();
  }

  uint32_t parity, data_width, stop_bits, rx_pin_inversion, tx_pin_inversion;

  switch (byte_format) {
  case BYTE_FORMAT_8N1:
    data_width = LL_USART_DATAWIDTH_8B;
    parity = LL_USART_PARITY_NONE;
    stop_bits = LL_USART_STOPBITS_1;
    break;
  case BYTE_FORMAT_9E2:
    data_width = LL_USART_DATAWIDTH_9B;
    parity = LL_USART_PARITY_EVEN;
    stop_bits = LL_USART_STOPBITS_2;
    break;
  
  default:
    // If unrecognized config, defaults to 8N1
    data_width = LL_USART_DATAWIDTH_8B;
    parity = LL_USART_PARITY_NONE;
    stop_bits = LL_USART_STOPBITS_1;
    break;
  }

  switch (pin_inversion) {
  case NO_INVERSION:
    rx_pin_inversion = LL_USART_RXPIN_LEVEL_STANDARD;
    tx_pin_inversion = LL_USART_TXPIN_LEVEL_STANDARD;
    break;
  case RX_INVERTED_TX_INVERTED:
    rx_pin_inversion = LL_USART_RXPIN_LEVEL_INVERTED;
    tx_pin_inversion = LL_USART_TXPIN_LEVEL_INVERTED;
    break;
  
  default:
    rx_pin_inversion = LL_USART_RXPIN_LEVEL_STANDARD;
    tx_pin_inversion = LL_USART_RXPIN_LEVEL_STANDARD;
    break;
  }

  LL_USART_SetBaudRate(USART2, LL_RCC_GetUSARTClockFreq(LL_RCC_USART2_CLKSOURCE),
                        LL_USART_OVERSAMPLING_16, baudrate);
  LL_USART_SetDataWidth(USART2, data_width);
  LL_USART_SetParity(USART2, parity);
  LL_USART_SetStopBitsLength(USART2, stop_bits);
  LL_USART_SetRXPinLevel(USART2, rx_pin_inversion);
  LL_USART_SetTXPinLevel(USART2, tx_pin_inversion);
}

//######################################################################################