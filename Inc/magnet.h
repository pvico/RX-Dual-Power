//######################################################################################
// magnet.h
// Magnet loop code, detection and state management
//
// Philippe Vico - 2021
//######################################################################################
 

#ifndef F70FDB9F_40FF_4C83_A6DE_912D82AD15A0
#define F70FDB9F_40FF_4C83_A6DE_912D82AD15A0


#include "main.h"
#include "stdbool.h"


#define MAGNET_PRESENT_PIN_STATE GPIO_PIN_RESET

#define MAGNET_DEBOUNCE_DELAY_MILLIS    100
#define MAGNET_SECOND_DETECTION_TIMEOUT_MILLIS    2000
#define MAGNET_SECOND_DETECTION_MINIMUM_INTERVAL_MILLIS    100


typedef enum magnet_states {MAGNET_PRESENT, MAGNET_NOT_PRESENT} magnet_state;


void magnet_loop();

bool is_magnet_present();
bool is_magnet_double_activation_active();


#endif /* F70FDB9F_40FF_4C83_A6DE_912D82AD15A0 */
