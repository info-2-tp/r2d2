/*
 * PR_timer.c
 *
 *  Created on: 6 oct. 2019
 *      Author: exo
 */

#include "../inc/PR_timer.h"
#include "../inc/DR_timer.h"

#define MICROSECOND 1
#define MILLISECOND 1000
#define SECOND 1000*1000

#define TIMERS_MAX_SIZE 10

#define PROCESS_TIME 8

#define RESET 1
#define NO_RESET 0

typedef struct pr_timer{
	uint32_t time;
	Timer_Closure handler;
	struct pr_timer* next;
}pr_timer_t;

pr_timer_t timers[TIMERS_MAX_SIZE];
pr_timer_t* first_timer = NULL;
uint8_t current_descriptor = 0;
uint8_t timer_counter = 0;

uint32_t cronometers[TIMERS_MAX_SIZE];
uint8_t cronometer_descriptor = 0;

void timer_handler_function(void);

void empty_handler() {}

uint32_t base_to_microseconds(uint8_t base) {
	switch(base) {
		case 'u': return MICROSECOND;
		case 'm': return MILLISECOND;
		case 's': return SECOND;
	};
	return MICROSECOND;
}

uint8_t startCronometer() {
	cronometers[cronometer_descriptor] = get_clock();
	uint8_t descriptor = cronometer_descriptor;
	cronometer_descriptor = (cronometer_descriptor + 1) % TIMERS_MAX_SIZE;
	return descriptor;
}

uint32_t stopCronometer(uint8_t descriptor) {
	return stopCronometerInBase(descriptor, MICROSECONDS);
}

uint32_t getCronometer(uint8_t descriptor) {
	return stopCronometer(descriptor);
}
uint32_t getCronometerInBase(uint8_t descriptor, uint8_t base) {
	return stopCronometerInBase(descriptor, base);
}

uint32_t stopCronometerInBase(uint8_t descriptor, uint8_t base) {
	uint32_t current_time = get_clock();
	uint32_t cronometer_time = cronometers[descriptor];
	return (current_time < cronometer_time
			? 0xffffffff - cronometer_time + current_time + 1 : current_time - cronometer_time)/base_to_microseconds(base);

}

uint32_t timer_in_microseconds(uint32_t time, uint8_t base) {
	return time*base_to_microseconds(base);
}

void initTimer() {
	init_timer();
}

pr_timer_t* initialize_timer(uint8_t descriptor, uint32_t time, Timer_Handler handler) {
	pr_timer_t* timer = timers + descriptor;
	timer->time = time;
	timer->handler = handler;
	return timer;
}

void set_first_timer(uint32_t time, Timer_Handler handler) {
	set_timer_from_now(time, timer_handler_function, RESET);
	pr_timer_t* timer = initialize_timer(current_descriptor, time, handler);
	timer->next = first_timer;
	first_timer = timer;
}

void set_second_timer(uint32_t time, Timer_Handler handler, uint32_t current_time) {
	should_not_reset_timer();
	pr_timer_t* timer = initialize_timer(current_descriptor, time + current_time, handler);
	timer->next = first_timer->next;
	first_timer->next = timer;
}

void set_new_timer_on_top(uint32_t time, Timer_Handler handler, uint32_t current_time) {
	set_timer_from_now(time, timer_handler_function, NO_RESET);
	pr_timer_t* timer = initialize_timer(current_descriptor, time + current_time, handler);
	timer->next = first_timer;
	first_timer = timer;
}

void set_new_timer(uint32_t time, Timer_Handler handler, uint32_t current_time) {
	pr_timer_t* timer = initialize_timer(current_descriptor, time + current_time, handler);
	pr_timer_t* tmp = first_timer;
	while(tmp->next && tmp->next->time < timer->time) tmp = tmp->next;
	timer->next = tmp->next;
	tmp->next = timer;

}

void fastTimer(uint32_t time, Timer_Closure handler) {
	set_fast_timer(get_clock() + time, handler);
}

void wait(uint32_t time) {
	uint32_t ttl = get_clock() + time;
	while(get_clock() < ttl);
}

void killTimer(uint8_t descriptor) {
	pr_timer_t* timer = timers + descriptor;
	timer->handler = empty_handler;
}

uint8_t startTimer(uint32_t time, Timer_Handler handler , uint8_t base) {
	uint32_t micro_time = timer_in_microseconds(time, base);
	uint32_t current_time = get_timer_clock();
	if (timer_counter == 0) {
		set_first_timer(micro_time, handler);
	} else {
		if (timer_counter == 1 && first_timer->time < micro_time + current_time) {
			set_second_timer(micro_time, handler, current_time);
		} else if (first_timer->time >= micro_time + current_time) {
			set_new_timer_on_top(micro_time, handler, current_time);
		} else{
			set_new_timer(micro_time, handler, current_time);
		}
	}
	timer_counter++;
	uint8_t descriptor = current_descriptor;
	current_descriptor = (current_descriptor + 1) % TIMERS_MAX_SIZE;
	return descriptor;
}

void timer_handler_function() {
	timer_counter--;
	pr_timer_t* timer = first_timer;
	first_timer = timer->next;
	if (timer_counter > 0)
		set_timer(first_timer->time, timer_handler_function, timer_counter == 1);
	timer->handler();
}
