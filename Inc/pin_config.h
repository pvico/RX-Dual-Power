#ifndef CD245FDE_E841_49E7_9916_C387BF48D93F
#define CD245FDE_E841_49E7_9916_C387BF48D93F

#include "main.h"

void init_pins();

void set_pin_to_analog(GPIO_TypeDef *gpio, uint32_t pin);
void set_pin_to_output_PP_no_pull(GPIO_TypeDef *gpio, uint32_t pin);
void set_stop_mode_not_needed_gpio_pins_to_analog();

#endif /* CD245FDE_E841_49E7_9916_C387BF48D93F */
