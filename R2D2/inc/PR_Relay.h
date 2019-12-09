#ifndef PR_RELAY_H
#define	PR_RELAY_H

#define MOTOR_SIERRA 0

#include "DR_tipos.h"
#include "DR_Relay.h"

#define 	ROJO 	1
#define 	VERDE 	2
#define 	AZUL 	3
#define		SHUTDOWN	4

extern uint8_t buffer_relay[4];

void relay_control(uint32_t RelayID,uint32_t Action);

void set_color(uint8_t color);

#endif
