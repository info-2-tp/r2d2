/*
 * DR_buttons.h
 *
 *  Created on: 17 nov. 2019
 *      Author: exo
 */

#include "DR_tipos.h"

#ifndef DR_BUTTONS_H_
#define DR_BUTTONS_H_

#define	STOP_BUTTON 				2,10		//SW1
#define	KNIFES_TOWER_TOP_BUTTON 	1,23		//EXP3
#define	BASE_INIT_BUTTON 			1,20		//EXP4

typedef struct {
	uint8_t state;
	uint8_t stable_states;
	uint8_t current_state;
}button_state;

void input_control(void);
void init_buttons();
#endif /* DR_BUTTONS_H_ */
