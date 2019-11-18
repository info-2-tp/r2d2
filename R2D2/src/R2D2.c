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
int current_state = PREPARE;
// TODO: insert other include files here

// TODO: insert other definitions and declarations here
void init_machine() {
    knifes_stop();
    move_knife_tower_up();
    move_base_bottom();
    current_state = PREPARE;
    printf("ESTADO --> PREPARE\n");
}

int main(void) {
    config();
    init_machine();
    while(FOREVER) {
    	state_machine();
    }
    return 0 ;
}
