/*
 * config.c
 *
 *  Created on: 17 nov. 2019
 *      Author: exo
 */

#include "../inc/config.h"
#include "../inc/DR_PLL.h"
#include "../inc/DR_systick.h"
#include "../inc/DR_buttons.h"
#include "../inc/DR_PWM.h"
#include "../inc/PR_timer.h"
#include "../inc/PR_HC_SR04.h";
#include "../inc/DR_UART0.h";

void config() {

	InicializarPLL();
	SysTick_Init();
	initTimer();
	init_buttons();
	Init_PWM();
	init_hc_sr04();
	UART0_Inicializacion();
}
