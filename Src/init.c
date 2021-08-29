#include "init.h"
#include "led.h"

void init() {
  // Extinguish both diodes
  led1_extinguish();
  led2_extinguish();    
}