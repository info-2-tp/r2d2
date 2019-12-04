/*
 *
 *
 *  Created on:
 *      Author: exo
 */

/*******************************************************************************************************************************//**
 *
 * @file	PR_timer.c
 * @brief
 * @date	6 oct. 2019
 * @author	Marcelo Passut
 *
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** INCLUDES
 **********************************************************************************************************************************/
#include "../inc/PR_timer.h"
#include "../inc/DR_timer.h"
#include <stdio.h>
/***********************************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 **********************************************************************************************************************************/
#define MICROSECOND 1
#define MILLISECOND 1000
#define SECOND 1000*1000

#define TIMERS_MAX_SIZE 1000

#define PROCESS_TIME 8

#define RESET 1
#define NO_RESET 0
/***********************************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TIPOS DE DATOS PRIVADOS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** TABLAS PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 **********************************************************************************************************************************/


/***********************************************************************************************************************************
 *** VARIABLES GLOBALES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

/***********************************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 **********************************************************************************************************************************/

 /***********************************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 **********************************************************************************************************************************/


typedef struct pr_timer{
	uint32_t time;
	Timer_Closure handler;
	struct pr_timer* next;
}pr_timer_t;

pr_timer_t timers[TIMERS_MAX_SIZE];
pr_timer_t* first_timer = NULL;
uint16_t current_descriptor = 0;
uint16_t timer_counter = 0;

uint32_t cronometers[TIMERS_MAX_SIZE];
uint16_t cronometer_descriptor = 0;

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void timer_handler_function(void);

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void empty_handler() {}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint32_t base_to_microseconds(uint8_t base) {
	switch(base) {
		case 'u': return MICROSECOND;
		case 'm': return MILLISECOND;
		case 's': return SECOND;
	};
	return MICROSECOND;
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint16_t startCronometer() {
	cronometers[cronometer_descriptor] = get_clock();
	uint16_t descriptor = cronometer_descriptor;
	cronometer_descriptor = (cronometer_descriptor + 1) % TIMERS_MAX_SIZE;
	return descriptor;
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint32_t stopCronometer(uint16_t descriptor) {
	return stopCronometerInBase(descriptor, MICROSECONDS);
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint32_t getCronometer(uint16_t descriptor) {
	return stopCronometer(descriptor);
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint32_t getCronometerInBase(uint16_t descriptor, uint8_t base) {
	return stopCronometerInBase(descriptor, base);
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint32_t stopCronometerInBase(uint16_t descriptor, uint8_t base) {
	uint32_t current_time = get_clock();
	uint32_t cronometer_time = cronometers[descriptor];
	return (current_time < cronometer_time
			? 0xffffffff - cronometer_time + current_time + 1 : current_time - cronometer_time)/base_to_microseconds(base);

}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint32_t timer_in_microseconds(uint32_t time, uint8_t base) {
	return time*base_to_microseconds(base);
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void initTimer() {
	init_timer();
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
pr_timer_t* initialize_timer(uint16_t descriptor, uint32_t time, Timer_Handler handler) {
	pr_timer_t* timer = timers + descriptor;
	timer->time = time;
	timer->handler = handler;
	return timer;
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void set_first_timer(uint32_t time, Timer_Handler handler) {
	set_timer_from_now(time, timer_handler_function, RESET);
	pr_timer_t* timer = initialize_timer(current_descriptor, time, handler);
	timer->next = first_timer;
	first_timer = timer;
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void set_second_timer(uint32_t time, Timer_Handler handler, uint32_t current_time) {
	should_not_reset_timer();
	pr_timer_t* timer = initialize_timer(current_descriptor, time + current_time, handler);
	timer->next = first_timer->next;
	first_timer->next = timer;
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void set_new_timer_on_top(uint32_t time, Timer_Handler handler, uint32_t current_time) {
	set_timer_from_now(time, timer_handler_function, NO_RESET);
	pr_timer_t* timer = initialize_timer(current_descriptor, time + current_time, handler);
	timer->next = first_timer;
	first_timer = timer;
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void set_new_timer(uint32_t time, Timer_Handler handler, uint32_t current_time) {
	pr_timer_t* timer = initialize_timer(current_descriptor, time + current_time, handler);
	pr_timer_t* tmp = first_timer;
	while(tmp->next && tmp->next->time < timer->time) tmp = tmp->next;
	timer->next = tmp->next;
	tmp->next = timer;

}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void fastTimer(uint32_t time, Timer_Closure handler) {
	set_fast_timer(get_clock() + time, handler);
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void wait(uint32_t time) {
	uint32_t ttl = get_clock() + time;
	while(get_clock() < ttl);
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void killTimer(uint16_t descriptor) {
	pr_timer_t* timer = timers + descriptor;
	timer->handler = empty_handler;
	printf("Matando timer %d\n", descriptor);
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint16_t startTimer(uint32_t time, Timer_Handler handler , uint8_t base) {
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
	uint16_t descriptor = current_descriptor;
	current_descriptor = (current_descriptor + 1) % TIMERS_MAX_SIZE;
	return descriptor;
}

/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void timer_handler_function() {
	timer_counter--;
	pr_timer_t* timer = first_timer;
	first_timer = timer->next;
	if (timer_counter > 0)
		set_timer(first_timer->time, timer_handler_function, timer_counter == 1);
	timer->handler();
}
