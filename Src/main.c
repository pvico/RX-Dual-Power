//######################################################################################
// main.c
// main startup function
//
// Philippe Vico - 2021
//######################################################################################
 

#include "main.h"
#include "initialize.h"
#include "main_loop.h"


// Values needed by core library

uint32_t SystemCoreClock = 2097152U; /* 32.768 kHz * 2^6 */
const uint8_t AHBPrescTable[16] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U};
const uint8_t APBPrescTable[8] = {0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U};
const uint8_t PLLMulTable[9] = {3U, 4U, 6U, 8U, 12U, 16U, 24U, 32U, 48U};

 
//################################### main function ####################################

int main(void) {
  initialize();

  while (1) {
    main_loop();
  }
}


// This function must be there, it is called by the assembler bootup code
void SystemInit() {}

//######################################################################################
