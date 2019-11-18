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
    	//for (int i = 0; i < 10000; i++);

    	if (stop_button_active()) {
    		turnOnPWM(ON);
    		//printf("STOP!!!\n");
    	} else {
    		turnOnPWM(OFF);
    	}
    	if (knifes_tower_on_top()) {
    		//moveteMotorPWM(TOWER_MOTOR, ON, UP);
    		//printf("CUCHILLAS!!!\n");
    	} else {
    		//moveteMotorPWM(TOWER_MOTOR, OFF, DOWN);
    	}
    	if (base_on_init()) {
    		moveteMotorPWM(BASE_MOTOR, ON, IZQ);
    		moveteMotorPWM(TOWER_MOTOR, ON, IZQ);
    		//printf("BASE!!!\n");
    	} else {
    		moveteMotorPWM(BASE_MOTOR, ON, DER);
    		moveteMotorPWM(TOWER_MOTOR, ON, DER);
    	}
    }
    return 0 ;
}
