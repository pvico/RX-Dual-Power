#include "eeprom.h"
#include <stdbool.h>


inline static bool __is_eeprom_ready() {
  return (((FLASH->SR) & FLASH_SR_READY) == FLASH_SR_READY);
}

inline static bool __is_eeprom_busy() {
  return (((FLASH->SR) & FLASH_SR_BSY) == FLASH_SR_BSY);
}

inline static bool __is_eeprom_operation_ended() {
  return (((FLASH->SR) & FLASH_SR_EOP) == FLASH_SR_EOP);
}

void __wait_for_last_operation() {
  while(__is_eeprom_busy()) {}

  if (__is_eeprom_operation_ended()) {
    CLEAR_BIT(FLASH->SR, FLASH_SR_EOP);
  }
}

void unlock_eeprom() {
  uint32_t primask_bit;

  if((FLASH->PECR & FLASH_PECR_PELOCK) != RESET) {  
    /* Disable interrupts to avoid any interruption during unlock sequence */
    primask_bit = __get_PRIMASK();
    __disable_irq();
    /* Unlocking the Data memory and FLASH_PECR register access*/
    FLASH->PEKEYR = FLASH_PEKEY1;
    FLASH->PEKEYR = FLASH_PEKEY2;
    /* Re-enable the interrupts: restore previous priority mask */
    __set_PRIMASK(primask_bit);
  }
}

void lock_eeprom() {
  /* Set the PELOCK Bit to lock the data memory and FLASH_PECR register access */
  SET_BIT(FLASH->PECR, FLASH_PECR_PELOCK);
}


void eeprom_write_word(uint32_t word, uint16_t word_offset) {
    unlock_eeprom();
    __wait_for_last_operation();

    *(((__IO uint32_t *)DATA_EEPROM_BASE) + word_offset) = word;

    __wait_for_last_operation();
    lock_eeprom();
}


