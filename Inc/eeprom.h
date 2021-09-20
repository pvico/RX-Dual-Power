#ifndef B719D22B_C756_4B96_933E_3E632E007258
#define B719D22B_C756_4B96_933E_3E632E007258


#include "main.h"


#define FLASH_PEKEY1               (0x89ABCDEFU) /*!< Flash program erase key1 */
#define FLASH_PEKEY2               (0x02030405U)


void unlock_eeprom();
void lock_eeprom();
void eeprom_write_word(uint32_t word, uint16_t word_offset);



#endif /* B719D22B_C756_4B96_933E_3E632E007258 */
