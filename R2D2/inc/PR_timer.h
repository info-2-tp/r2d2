/*
 * PR_timer.h
 *
 *  Created on: 6 oct. 2019
 *      Author: exo
 */

#ifndef HEADERS_PR_TIMER_H_
#define HEADERS_PR_TIMER_H_

#include "DR_tipos.h"
#include "DR_timer.h"

#define MICROSECONDS 'u'
#define MILLISECONDS 'm'
#define SECONDS	's'

typedef void (*Timer_Handler)(void);

void initTimer(void);
uint16_t startTimer(uint32_t time, Timer_Closure handler , uint8_t base );
void killTimer(uint16_t descriptor);
void fastTimer(uint32_t time, Timer_Closure handler);
void wait(uint32_t time);
uint16_t startCronometer();
uint32_t stopCronometer(uint16_t descriptor);
uint32_t getCronometer(uint16_t descriptor);
uint32_t getCronometerInBase(uint16_t descriptor, uint8_t base);
uint32_t stopCronometerInBase(uint16_t descriptor, uint8_t base);

#endif /* HEADERS_PR_TIMER_H_ */
