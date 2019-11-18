#ifndef HEADERS_HC_SR04_H_
#define HEADERS_HC_SR04_H_

#include "DR_tipos.h"

#define 	ECHO			3,25	//Exp6
#define		TRIGGER			0,19	//Exp5

void init_hc_sr04(void);

void trigger_turn_on();
void trigger_turn_off();
uint8_t echo_on();
uint8_t echo_off();

#endif /* HEADERS_HC_SR04_H */
