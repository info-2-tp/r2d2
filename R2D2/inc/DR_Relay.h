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

#define 	RELAY0		2,0 //cuchi
#define 	RELAY1		0,23//rojo
#define 	RELAY2		0,21//verde
#define 	RELAY3		0,27//azul




#endif
