#ifndef F70FDB9F_40FF_4C83_A6DE_912D82AD15A0
#define F70FDB9F_40FF_4C83_A6DE_912D82AD15A0

#include "gpio.h"
#include <stm32l021xx.h>
#include "stdbool.h"

#define MAGNET_PIN GPIO_PIN_0
#define MAGNET_GPIO GPIOA
#define MAGNET_PRESENT GPIO_PIN_SET

bool is_magnet_detected();

#endif /* F70FDB9F_40FF_4C83_A6DE_912D82AD15A0 */
