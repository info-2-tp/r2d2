/*
 * DR_systick.c
 *
 *  Created on: 6 sep. 2019
 *      Author: gif30
 */
#include "../inc/DR_Systick.h"
#include "../inc/DR_buttons.h"
#include "../inc/DR_outputs.h"

void SysTick_Init(void){
	Systick->STRELOAD = (Systick->STCALIB / 4) - 1; //Base de tiempo dada por 10ms/4: El -1 es porque cuenta uno de mas el STCURR
	Systick->STCURR = 0; //esto es para limpiar el timer

	Systick->ENABLE = 1; //activa el modulo
	Systick->TICKINT = 1; //activa la interrupcion
	Systick->CLKSOURCE = 1; //activa elige el clock del CPU en vez del clock externo por pin (STCLK)
}

void SysTick_Handler(void)
{
	input_control();
	controlarSalidas();
}
