//
// Created by marcelo on 2/11/19.
//
//Maquina de estado mediante vector de ptr a funcion

#include "../inc/actions.h"
#include "../inc/events.h"
#include "../inc/states.h"
#include "../inc/PR_PWM.h"
#include "../inc/PR_timer.h"
#include "../inc/PR_HC_SR04.h"
#include "../inc/PR_UART0.h"
#include "../inc/PR_Towercontrol.h"
#include <stdio.h>

#define 	BASE_DISTANCE 480


void LCD_Display(const char *string, unsigned char line ,unsigned char pos) {
    printf(string);
}

int current_state;
cuts_t cuts;
char current_cut = 0;
int cube_size = 0;


int controlON = 0;

unsigned char has_data=0;
int32_t  measure_size;
message_header_t header;
routine_t routine[50];
uint8_t header_loaded=0;
void measuring() {
	distance_t distance = getDistance(MM);
	measure_size = BASE_DISTANCE - distance;
	if (current_state == MEASURING)	startTimer(500, measuring, MILLISECONDS);


}

void (*state_functions[])() = {prepare_state, load_state, stop_state, measuring_state, obi_wan_com_state,
                               prepare_cut_state, prepare_knife_state, cutting_state, cut_returning_state};

void stop_state() {
    if ( run_button()) {
    	init_machine();
    }
}

void prepare_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        return;
    }

    if ( base_front() && !knifes_top()) move_knife_tower_up();

    if (base_front()) base_stop();

    if (knifes_top()) {
        knife_tower_stop();
    }

    if (base_front() && knifes_top()) {
        LCD_Display("Ya se pueden cargar cubos\n", 0, 0);
        current_state = LOAD;
        printf("ESTADO --> LOAD\n");
    }
}

void load_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        return;
    }

    if ( run_button()) {
    	startTimer(500, measuring, MILLISECONDS);
    	move_base_back();
        current_state = MEASURING;
        printf("ESTADO --> MEASURING\n");
    }
}

void measuring_state() {

    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        return;
    }


    if (measure_size > 0 ) {
        move_base_front();
        cube_size = measure_size;
        send_info_to_obi_wan(cube_size);
        receive((void*)&header,sizeof(header));
        current_state = OBI_WAN_COM;
        printf("Estado --> OBI WAN COM\n");

    }

    if(base_back()){
    	init_machine();
    	LCD_Display("No hay cubo\n", 0, 0);


    }
}

void obi_wan_com_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        return;
    }

   /* if (base_bottom() && !has_data) {
        LCD_Display("No hay comunicación\n");
        current_state = LOAD;
        printf("ESTADO --> LOAD\n");
    }

    if (has_data && routines.cant <= 0){
        move_knife_tower_up();
        move_base_bottom();
        LCD_Display("No hay trabajos\n");
        current_state = PREPARE;
        printf("ESTADO --> PREPARE\n");
    }

    if (has_data && routines.cant > 0 && base_bottom()) {
        cuts = calculate_cuts();
        move_knife_tower_down();
        current_state = PREPARE_CUT;
    }*/
    cuts.positions[0] = 70;
    cuts.positions[1] = 70;
    cuts.positions[2] = 10;
    cuts.positions[3] = 40;
    cuts.positions[4] = 10;
    cube_size = 30;
}

void prepare_cut_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        return;
    }

    if(controlON){
    	Tower_Control(48 - cube_size);
    	controlON = 0;
    }
    if (TowerPosition == (48 - cube_size)) {
    	Tower_Control(cuts.positions[current_cut]);
    	controlON = 1;
    	next_cut();
        current_state = PREPARE_SNIFE;
    }
}

void prepare_knife_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        reset_cut();
        return;
    }

    if (TowerPosition == (TowerPositionOld + cuts.positions[current_cut])) {
        knife_tower_stop();
        knifes_run();
        move_base_back();
        current_state = CUTTING;
    }
}

void cutting_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        reset_cut();
        return;
    }

    if (base_back()) {
    	move_base_front();
        current_state = CUT_RETURNING;
    }
}

void cut_returning_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        reset_cut();
        return;
    }

    if (base_front()) {
        //
        if (current_cut == cuts.cuts) {
            send_ack_to_obi_wan();
            //LCD_Display("Fin");
            reset_cut();
            current_state = PREPARE;
        } else {
        	Tower_Control(cuts.positions[current_cut]);
        	next_cut();
            current_state = PREPARE_SNIFE;
        }

        current_state = CUT_RETURNING;
    }
}

void state_machine() {
    state_functions[current_state]();
}


void init_machine() {
	turnOnPWM(ON);
    knifes_stop();
    move_base_front();
    current_state = PREPARE;
    printf("ESTADO --> PREPARE\n");
}
