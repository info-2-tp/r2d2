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

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {
    config();

    while(1) {
    	if (stop_button_active()) printf("Boton Stop Apretado\n");
    	if (knifes_tower_on_top()) printf("Tope de Cierra\n");
    	if (base_on_init()) printf("Base en estado inicial\n");
    }
    return 0 ;
}
