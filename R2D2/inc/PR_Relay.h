#ifndef PR_RELAY_H
#define	PR_RELAY_H

#define MOTOR_SIERRA 0

#include "DR_tipos.h"
#include "DR_Relay.h"

extern uint8_t buffer_relay[4];

void relay_control(uint32_t RelayID,uint32_t Action);

#endif
