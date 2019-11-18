#ifndef TIMER_DRIVER_V0_1_0_H_
#define TIMER_DRIVER_V0_1_0_H_

#include <DR_tipos.h>

typedef void (*Timer_Closure)(void);

void init_timer(void);

void reset_timer();

void should_not_reset_timer();

void set_timer(uint32_t time,Timer_Closure handler, uint8_t reset);

void set_timer_from_now(uint32_t time,Timer_Closure handler, uint8_t reset);

void set_fast_timer(uint32_t time,Timer_Closure handler);

uint32_t get_timer_clock();

uint32_t get_clock();

#endif //TIMER_DRIVER_V0_1_0_H_
