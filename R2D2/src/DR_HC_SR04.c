/*
 * DR_HC_SR04.c
 *
 *  Created on: 26 oct. 2019
 *      Author: exo
 */
#include "../inc/DR_HC_SR04.h"
#include "../inc/DR_gpio.h"

void init_hc_sr04() {
	GPIO_Pinsel(TRIGGER, PINSEL_GPIO);
	GPIO_Pinsel(ECHO, PINSEL_GPIO);

	GPIO_Dir( TRIGGER, OUTPUT);
	GPIO_Dir( ECHO, INPUT);

	GPIO_Mode(ECHO, NONE);

	GPIO_Set(TRIGGER, OFF);
}

void trigger_turn_on() {
	GPIO_Set(TRIGGER, ON);
}

void trigger_turn_off() {
	GPIO_Set(TRIGGER, OFF);
}

uint8_t echo_off() {
	return !GPIO_Get(ECHO);
}

uint8_t echo_on() {
	return GPIO_Get(ECHO);
}


