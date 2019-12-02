#ifndef funciones_RELAY_h
#define funciones_RELAY_h

//includes
#include "DR_tipos.h"


//funciones
void active_relay(void);
void init_Relay(void);


//variables
extern uint8_t buffer_relay[4];

//defines

#define 	RELAY0		2,0
#define 	RELAY1		0,23
#define 	RELAY2		0,21



#endif
