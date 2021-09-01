#ifndef F70FDB9F_40FF_4C83_A6DE_912D82AD15A0
#define F70FDB9F_40FF_4C83_A6DE_912D82AD15A0

#include "gpio.h"
#include <stm32l021xx.h>
#include "stdbool.h"

#define MAGNET_PIN GPIO_PIN_0
#define MAGNET_GPIO GPIOA
#define MAGNET_PRESENT_PIN_STATE GPIO_PIN_RESET

#define MAGNET_DEBOUNCE_DELAY_MILLIS    100
#define MAGNET_SECOND_DETECTION_TIMEOUT_MILLIS    3000
#define MAGNET_SECOND_DETECTION_MINIMUM_INTERVAL_MILLIS    100

enum magnet_states{MAGNET_PRESENT, MAGNET_NOT_PRESENT};

void magnet_loop();

bool is_magnet_double_activation_active();
void disable_magnet_double_activation();

#endif /* F70FDB9F_40FF_4C83_A6DE_912D82AD15A0 */
