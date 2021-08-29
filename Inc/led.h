#ifndef D86274A9_C095_49D9_9A72_98E31E11064A
#define D86274A9_C095_49D9_9A72_98E31E11064A

#include <stm32l021xx.h>

#define LED1_GPIO GPIOA
#define LED1_PIN GPIO_PIN_13 
#define LED2_GPIO GPIOA
#define LED2_PIN GPIO_PIN_7 

void led1_illuminate();
void led1_extinguish();
void led2_illuminate();
void led2_extinguish();

#endif /* D86274A9_C095_49D9_9A72_98E31E11064A */
