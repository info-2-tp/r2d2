/*
 * PR_buttons.c
 *
 *  Created on: 17 nov. 2019
 *      Author: exo
 */

#include "../inc/PR_buttons.h"

volatile button_state stop_button;
volatile button_state knifes_tower_top;
volatile button_state base_init;
volatile button_state start_button;

uint8_t stop_button_active() {
	return stop_button.current_state;
}

uint8_t knifes_tower_on_top() {
	return knifes_tower_top.current_state;
}

uint8_t base_on_init() {
	return base_init.current_state;
}

uint8_t run_button_active(){
	return start_button.current_state;
}

