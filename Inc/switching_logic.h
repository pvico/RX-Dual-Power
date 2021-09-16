#ifndef E36CD3DA_CBA9_4CC2_93ED_4CD5DDDC849A
#define E36CD3DA_CBA9_4CC2_93ED_4CD5DDDC849A



typedef enum { MAIN_PWR_ON_STBY_OK,                                                     // RBOX RBCS CH01
               MAIN_PWR_ON_STBY_LOW,                                                    // RBOX RBCS CH02
               STBY_PWR_ON_MAIN_LOW,                                                    // RBOX RBCS CH03
               MAIN_PWR_ON_STBY_DISCONNECT_OR_BAD_CONTACT,                              // RBOX RBCS CH04
               STBY_PWR_ON_MAIN_DISCONNECTED_OR_BAD_CONTACT,                            // RBOX RBCS CH05
               MAIN_PWR_ON_MAIN_BAD_CONTACT,                                            // RBOX RBCS CH06
               STBY_PWR_ON_STBY_BAD_CONTACT,                                            // RBOX RBCS CH07
               CRITICAL_MAIN_LOW_STBY_LOW,                                              // RBOX RBCS CH08
               CRITICAL_MAIN_LOW_STBY_DISCONNECT_OR_BAD_CONTACT,                        // RBOX RBCS CH09
               CRITICAL_STBY_LOW_MAIN_DISCONNECT_OR_BAD_CONTACT,                        // RBOX RBCS CH10
               CRITICAL_MAIN_DISCONNECT_OR_BAD_CONTACT_STBY_DISCONNECT_OR_BAD_CONTACT   // RBOX RBCS CH11
               } switching_states;

void switching_logic_loop();

#endif /* E36CD3DA_CBA9_4CC2_93ED_4CD5DDDC849A */
