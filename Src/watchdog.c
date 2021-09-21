//######################################################################################
// watchdog.c
// Initialization of watchdog
//
// Philippe Vico - 2021
//######################################################################################


#include "watchdog.h"
#include "main.h"


//################################ Interface functions #################################

void init_watchdog() {
  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_WWDG);

  LL_WWDG_SetCounter(WWDG, 0x7F);
  LL_WWDG_Enable(WWDG);
  LL_WWDG_SetPrescaler(WWDG, LL_WWDG_PRESCALER_4);
  LL_WWDG_SetWindow(WWDG, 0x7F);    
}
//######################################################################################
