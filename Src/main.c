#include "main.h"
#include "initialize.h"
#include "main_loop.h"


int main(void) {
  initialize();

  while (1) {
    main_loop();
  }
}


