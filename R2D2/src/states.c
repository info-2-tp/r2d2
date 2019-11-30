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
#include "../inc/PR_LCD.h"
#include <stdio.h>

#define 	BASE_DISTANCE 480


void LCD_Display(const char *string, unsigned char line ,unsigned char pos) {
	PrintLCD(string,line,pos);
}

int current_state;
cuts_t cuts;
char current_cut = -1;
int cube_size;
unsigned char has_data=0;
int32_t  measure_size;
message_header_t header;
routine_t routine[50];
uint8_t header_loaded=0;
short id_timer=-1;
uint8_t obiwan_timeout=0;
void obiwan_ttl(){
	obiwan_timeout=1;
}

void reset_obiwan_data(){
	has_data=0;
	 measure_size=0;
	 header_loaded=0;
	 killTimer(id_timer);
	 id_timer=-1;
	 obiwan_timeout=0;
}
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

    if (base_front()) base_stop();

    if (knifes_top()) knife_tower_stop();

    if ( base_front() && !knifes_top()) move_knife_tower_up();

    if (base_front() && knifes_top()) {
        LCD_Display("Cargar cubos\n", 0, 0);
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
        measure_size=0;

    }

    if(base_back()){
    	base_stop();
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

    if (base_front()) {
    	stop_all();
    }

    if(!header_loaded && receive_ready() ){
    	header_loaded = 1;
    	receive((void*)routine,header.size);
    }

    if(header_loaded && !has_data && receive_ready()){
    	has_data=1;
    	cuts = calculate_cuts(routine,header.size/sizeof(routine_t));
    }

    if (has_data && (header.size/sizeof(routine_t)) == 0){
        init_machine();
        LCD_Display("No hay trabajos\n",0,0);
        current_state = PREPARE;
        printf("ESTADO --> PREPARE\n");
        reset_obiwan_data();
    }

    if (has_data && (header.size/sizeof(routine_t)) > 0 && base_front()) {
        move_knife_tower_down();
        reset_obiwan_data();
        current_state = PREPARE_CUT;
        for (int i = 0; i < cuts.cuts; i++) {
        	printf("Corte[%d]: %dmm\n", i, cuts.positions[i]);
        }
        printf("ESTADO --> PREPARE_CUT\n");
    }

    if(id_timer<0){
    	id_timer = startTimer(60,obiwan_ttl,SECONDS);
    }

    if(obiwan_timeout){
    	LCD_Display("Sin comunicación\n",0,0);
    	init_machine();
    	current_state = PREPARE;
    	printf("ESTADO --> PREPARE\n");
    	reset_obiwan_data();
    	return;
    }
}

void prepare_cut_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        return;
    }

    if (knifes_are_ready()) {
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

    if (knifes_is_there(cuts.positions[current_cut])) {
        knife_tower_stop();
        move_base_back();
        knifes_run();
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
        move_base_back();
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
        next_cut();
        if (current_cut == cuts.cuts) {
            send_ack_to_obi_wan();
            LCD_Display("Fin",0,0);
            reset_cut();
            current_state = PREPARE;
        } else {
            move_knife_tower_down();
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


