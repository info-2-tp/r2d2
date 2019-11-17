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
// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {
    config();

    while(1) {
    	if (stop_button_active());
    	if (knifes_tower_on_top()) {
    		moveteMotorPWM(3, 1, 1);
    	} else {
    		moveteMotorPWM(3, 0, 1);
    	}
    	if (base_on_init()) {
    		moveteMotorPWM(3, 1, 0);
    	} else {
    		moveteMotorPWM(3, 0, 0);
    	}
    }
    return 0 ;
}
