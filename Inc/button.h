#ifndef A61822EC_01F4_46DB_9C40_C645C11613D2
#define A61822EC_01F4_46DB_9C40_C645C11613D2

#include "stdbool.h"

#define BUTTON_DEBOUNCE_DELAY_MILLIS    10
#define BUTTON_DOUBLE_ACTIVATION_MILLIS    2000

typedef enum button_states {BUTTON_DEPRESSED, BUTTON_NOT_DEPRESSED} button_state;

void button_loop();

bool is_button_dual_activation_active();

#endif /* A61822EC_01F4_46DB_9C40_C645C11613D2 */
