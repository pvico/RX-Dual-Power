#include "magnet.h"


bool is_magnet_detected() {
    return HAL_GPIO_ReadPin(MAGNET_GPIO, MAGNET_PIN) == MAGNET_PRESENT;
}
