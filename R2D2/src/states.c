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
#include "../inc/PR_Towercontrol.h"
#include <stdio.h>
#define 	BASE_DISTANCE 480
#define 	KNIFE_DISTANCE 440


void LCD_Display(const char *string, unsigned char line ,unsigned char pos) {
	PrintLCD(string,line,pos);
}

int current_state;
cuts_t cuts;
char current_cut = 0;
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
	PrintLCD_With_Number("Distancia: %d", RENGLON_1, 0, distance);
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
        PrintLCD("STOP", RENGLON_2, 0);
        return;
    }

    if (base_front()) base_stop();

    if (knifes_top()) knife_tower_stop();

    if ( base_front() && !knifes_top()) move_knife_tower_up();

    if (base_front() && knifes_top()) {
    	stop_all();
    	PrintLCD("Cargar cubos", RENGLON_1, 0);
        current_state = LOAD;
        PrintLCD("LOAD", RENGLON_2, 0);
    }
}

void load_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        PrintLCD("STOP", RENGLON_2, 0);
        return;
    }

    if ( run_button()) {
    	startTimer(500, measuring, MILLISECONDS);
    	turnOnPWM(ON);
    	move_base_back();
        current_state = MEASURING;
        PrintLCD("MEASURING", RENGLON_2, 0);
    }
}

void measuring_state() {

    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        PrintLCD("STOP", RENGLON_2, 0);
        return;
    }


    if (measure_size > 0 ) {
        move_base_front();
        cube_size = measure_size;
        send_info_to_obi_wan(cube_size);
        receive((void*)&header,sizeof(header));
        current_state = OBI_WAN_COM;
        PrintLCD_With_Number("Cube de %dmm", RENGLON_1, 0, cube_size);
        PrintLCD("OBI WAN COM", RENGLON_2, 0);
        measure_size=0;

    }

    if(base_back()){
    	base_stop();
    	init_machine();
    	current_state = PREPARE;
    	PrintLCD("No hay cubo", RENGLON_1, 0);
    }
}

void obi_wan_com_state() {

    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        PrintLCD("STOP", RENGLON_2, 0);
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
        printf("size: %d\n",header.size);
        PrintLCD("No hay trabajos", RENGLON_1, 0);
        current_state = PREPARE;
        PrintLCD("PREPARE", RENGLON_2, 0);
        reset_obiwan_data();
    }

    if (has_data && (header.size/sizeof(routine_t)) > 0 && base_front()) {
    	turnOnPWM(ON);
    	Tower_Control(KNIFE_DISTANCE - cube_size);
        reset_obiwan_data();
        current_state = PREPARE_CUT;
        for (int i = 0; i < cuts.cuts; i++) {
        	printf("Corte[%d]: %dmm\n", i, cuts.positions[i]);
        }
        PrintLCD("PREPARE_CUT", RENGLON_2, 0);
    }

    if(id_timer<0){
    	id_timer = startTimer(60,obiwan_ttl,SECONDS);
    }

    if(obiwan_timeout){
    	PrintLCD("Sin comunicación", RENGLON_1, 0);
    	init_machine();
    	current_state = PREPARE;
    	PrintLCD("PREPARE", RENGLON_2, 0);
    	reset_obiwan_data();
    	return;
    }
}

void prepare_cut_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        PrintLCD("STOP", RENGLON_2, 0);
        return;
    }
    if (TowerPosition == (KNIFE_DISTANCE - cube_size)) {
    	Tower_Control(cuts.positions[current_cut]);
    	next_cut();
        current_state = PREPARE_SNIFE;
        PrintLCD("PREPARE_SNIFE", RENGLON_2, 0);
    }
}

void prepare_knife_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        PrintLCD("STOP", RENGLON_2, 0);
        reset_cut();
        reset_tower();
        return;
    }

    if (TowerPosition == TowerPositionOld) {
        knife_tower_stop();
        knifes_run();
        move_base_back();
        PrintLCD_With_Number("Cortando %dmm", RENGLON_1, 0, cuts.positions[current_cut]);
        current_state = CUTTING;
        PrintLCD("CUTTING", RENGLON_2, 0);
    }
}

void cutting_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        PrintLCD("STOP", RENGLON_2, 0);
        reset_cut();
        reset_tower();
        return;
    }

    if (base_back()) {
    	move_base_front();
        current_state = CUT_RETURNING;
        PrintLCD("CUT_RETURNING", RENGLON_2, 0);
    }
}

void cut_returning_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        PrintLCD("STOP", RENGLON_2, 0);
        reset_cut();
        reset_tower();
        return;
    }

    if (base_front()) {
        //
    	base_stop();
        if (current_cut == cuts.cuts) {
            send_ack_to_obi_wan();
            PrintLCD("Fin", RENGLON_1, 0);
            reset_tower();
            reset_cut();
            init_machine();
            current_state = PREPARE;
        } else {
        	Tower_Control(cuts.positions[current_cut]);
        	next_cut();
            current_state = PREPARE_SNIFE;
            PrintLCD("PREPARE_SNIFE", RENGLON_2, 0);
        }
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
    PrintLCD("PREPARE", RENGLON_2, 0);
}


