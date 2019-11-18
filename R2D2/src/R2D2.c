/*
===============================================================================
 Name        : R2D2.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include <stdio.h>
#include "../inc/config.h"
#include "../inc/PR_buttons.h"
#include "../inc/PR_PWM.h"
#include "../inc/actions.h"
#include "../inc/states.h"

#define FOREVER 1

int main(void) {
    config();
    init_machine();
    while(FOREVER) {
    	state_machine();
    }
    return 0 ;
}
