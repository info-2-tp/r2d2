/*
 * DR_buttons.c
 *
 *  Created on: 17 nov. 2019
 *      Author: exo
 */

#include "../inc/DR_buttons.h"
#include "../inc/DR_GPIO.h"

#define BOUNCES 4

extern volatile button_state stop_button;
extern volatile button_state knifes_tower_top;
extern volatile button_state base_init;

void init_buttons() {
	GPIO_Pinsel ( STOP_BUTTON , PINSEL_GPIO );
	GPIO_Pinsel ( KNIFES_TOWER_TOP_BUTTON , PINSEL_GPIO );
	GPIO_Pinsel ( BASE_INIT_BUTTON , PINSEL_GPIO );

	GPIO_Dir ( STOP_BUTTON , INPUT );
	GPIO_Dir ( KNIFES_TOWER_TOP_BUTTON , INPUT );
	GPIO_Dir ( BASE_INIT_BUTTON , INPUT );

	GPIO_Mode ( STOP_BUTTON , PULLUP );
	GPIO_Mode ( KNIFES_TOWER_TOP_BUTTON , PULLUP );
	GPIO_Mode ( BASE_INIT_BUTTON , PULLUP );
}

void button_state_hw( void )
{
	stop_button.state = GPIO_Get(STOP_BUTTON);
	knifes_tower_top.state = GPIO_Get(KNIFES_TOWER_TOP_BUTTON);
	base_init.state = GPIO_Get(BASE_INIT_BUTTON);
}

void filter_bounce(volatile button_state* button) {
	if (button->state && button->stable_states < BOUNCES) {
		button->stable_states++;
		button->current_state = OFF;
	}
	if (button->state && button->stable_states >= BOUNCES) {
		button->current_state = ON;
	}
	if (!button->state) {
		button->stable_states = 0;
		button->current_state = OFF;
	}
}

void input_control(void) {
	button_state_hw();
	filter_bounce(&stop_button);
	filter_bounce(&knifes_tower_top);
	filter_bounce(&base_init);
}



