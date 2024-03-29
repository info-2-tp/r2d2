/*
 * timer.c
 *
 *  Created on: 22 sep. 2019
 *      Author: exo
 */

#include "../inc/DR_timer_regs.h"
#include "../inc/DR_timer.h"

Timer_Closure _handler;
Timer_Closure fast_handler;

/**
 * Prendo los bits del timer 0 y apago los de los timers 1/2/3
 */
/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void power_up() {
	PCONP |= (1 << 1);
	PCONP |= (1 << 2);
}

/**
 * Segun tabla 42. Peripheral Clock Selection register bit values
 * 00 PCLK_peripheral = CCLK/4 00
 */
/**
	\fn
	\brief
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void select_clock_speed() {
	PCLKSEL_0 &= ~(3 << 2);
	PCLKSEL_0 &= ~(3 << 4);
	T0_PR = PRESCALE_FOR_1_US;
	T1_PR = PRESCALE_FOR_1_US;
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
void open_timer() {
	T0_TCR |= 1;
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
void close_timer() {
	T0_TCR &= ~(1);	   // Apago  el temporizador,poniendo un cero en el bit 0 Pag 494
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
void prepare_interrupts() {
	T0_MCR |= 3;				// Genera una interrupcion cuando MC0 Matchea,  Resetea el TC cuando MC0 Matchea
	ISE_TIMER_0; 				// Habilito interrupcion del Timer0 en el vector de interrupciones.
	T1_MCR |= 1;
	ISE_TIMER_1;
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
void start_timer() {
	T0_TCR &= ~(1);	   // Apago  el temporizador,poniendo un cero en el bit 0 Pag 494
	T0_TCR |= 1<<1;    //  Resteo el temporizador , prendiendo el bit 1
	T0_TCR &= ~(1 << 1);// Apago el bit 1 de RESET
	T0_TCR |= 1;        // Enciendo el temporizador poniendo el bit 0 de ENABLE en 1
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
void start_clock() {
	T1_TCR &= ~(1);	   // Apago  el temporizador,poniendo un cero en el bit 0 Pag 494
	T1_TCR |= 1<<1;    //  Resteo el temporizador , prendiendo el bit 1
	T1_TCR &= ~(1 << 1);// Apago el bit 1 de RESET
	T1_TCR |= 1;        // Enciendo el temporizador poniendo el bit 0 de ENABLE en 1
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
void init_timer(void) {
	power_up();
	select_clock_speed();
	prepare_interrupts();
	start_timer();
	start_clock();
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
void set_timer(uint32_t time,Timer_Closure handler, uint8_t reset) {
	if (reset) T0_MCR |= 1 << 1; else T0_MCR &= ~(1 << 1);
	T0_MCR |= 1;
	T0_MR0 = time;
	open_timer();
	_handler = handler;
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
void reset_timer() {
	start_timer();
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
void should_not_reset_timer() {
	T0_MCR &= ~(1 << 1);
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
void set_timer_from_now(uint32_t time,Timer_Closure handler, uint8_t reset) {
	set_timer(get_timer_clock() + time, handler, reset);
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
void set_fast_timer(uint32_t time,Timer_Closure handler) {
	T1_MCR &= ~(1 << 1);
	T1_MCR |= 1;
	T1_MR0 = time;
	T1_TCR |= 1;
	fast_handler = handler;
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
uint32_t get_timer_clock() {
	return TC_0;
}

/**
	\fn  get_clock
	\brief Devuelve
 	\author
 	\date
 	\param [in]
 	\param [out]
	\return
*/
uint32_t get_clock() {
	return TC_1;
}

/**
	\fn  TIMER0_IRQHandler
	\brief Interrupcion del Timer0
 	\author	Marcelo Passut
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void TIMER0_IRQHandler(void) {
	if(T0_IR_MR0) {	 // Interrumpio por match 0 ?
		T0_MCR &= ~(1);
		T0_IR |= 0x01;
		if (_handler) _handler();
	}
}

/**
	\fn  TIMER1_IRQHandler
	\brief Interrupcion del Timer1
 	\author	Marcelo Passut
 	\date
 	\param [in]
 	\param [out]
	\return
*/
void TIMER1_IRQHandler(void) {
	if(T1_IR_MR0) {	 // Interrumpio por match 0 ?
		T1_MCR &= ~(1);
		T1_IR |= 0x01;
		if (fast_handler) fast_handler();
	}
}

