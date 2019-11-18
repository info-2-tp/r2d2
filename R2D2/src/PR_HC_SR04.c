/*
 * PR_HC_SR04.c
 *
 *  Created on: 26 oct. 2019
 *      Author: exo
 */

#include "../inc/PR_timer.h"
#include "../inc/DR_HC_SR04.h"
#include "../inc/PR_HC_SR04.h"

#define TRIGGER_TTL 10
#define MICROSECONDS_BY_CM 58

distance_t transform_microseconds_to(uint32_t microseconds, distance_base base) {
	return microseconds*base/MICROSECONDS_BY_CM;
}

void init_hc_sr04_sensor() {
	init_hc_sr04();
}

distance_t getDistance(distance_base base) {
	uint8_t cron;
	uint32_t microseconds;
	trigger_turn_on();
	wait(TRIGGER_TTL);
	trigger_turn_off();
	while(echo_off());
	cron = startCronometer();
	while(echo_on());
	microseconds = stopCronometer(cron);
	return transform_microseconds_to(microseconds, base);
}

