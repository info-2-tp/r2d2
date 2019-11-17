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

void config() {
	InicializarPLL();
	SysTick_Init();
	init_buttons();
	Init_PWM();
}
