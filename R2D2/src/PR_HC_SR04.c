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
#define TTL_ECHO 17400

distance_t transform_microseconds_to(uint32_t microseconds, distance_base base) {
	return microseconds*base/MICROSECONDS_BY_CM;
}

void init_hc_sr04_sensor() {
	init_hc_sr04();
}

distance_t vector[]={ 590,590,590,590,590,590,590,590,480,480,480,480, 280 };

#define MAX 13

distance_t getDistance(distance_base base) {
	static uint8_t index=0;
	index++;
	index = index%MAX;
	return vector[index];

	/*uint8_t cron;
	uint32_t microseconds;
	trigger_turn_on();
	wait(TRIGGER_TTL);
	trigger_turn_off();
	cron = startCronometer();
	while(echo_off()  && getCronometer(cron) < TTL_ECHO);
	cron = startCronometer();
	while(echo_on() && getCronometer(cron) < TTL_ECHO);
	microseconds = stopCronometer(cron);
	return transform_microseconds_to(microseconds, base);*/

}

