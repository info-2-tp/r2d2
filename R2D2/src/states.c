//
// Created by marcelo on 2/11/19.
//
//Maquina de estado mediante vector de ptr a funcion

#include "../inc/actions.h"
#include "../inc/events.h"
#include "../inc/states.h"
#include "../inc/PR_lcd.h"
#include "../inc/PR_PWM.h"
#include "../inc/PR_timer.h"

//MOCK
#include <stdio.h>
void LCD_Display(const char *string, unsigned char line ,unsigned char pos) {
    printf(string);
}

int current_state;
cuts_t cuts;
char current_cut = -1;
int cube_size;
unsigned char has_data;
routines_t routines;

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

    //cada 500 mili

    if (base_middle()) {
        move_base_front();
        cube_size = measure_size();
        send_info_to_obi_wan(cube_size);
        current_state = OBI_WAN_COM;
        printf("Estado --> OBI WAN COM\n");
    }
}

void obi_wan_com_state() {
    if (emergency_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        return;
    }

    if (base_front() && !has_data) {
        LCD_Display("No hay comunicación\n",0,0);
        current_state = LOAD;
        printf("ESTADO --> LOAD\n");
    }

    if (has_data && routines.cant <= 0){
        move_knife_tower_up();
        move_base_front();
        LCD_Display("No hay trabajos\n",0,0);
        current_state = PREPARE;
        printf("ESTADO --> PREPARE\n");
    }

    if (has_data && routines.cant > 0 && base_front()) {
        cuts = calculate_cuts();
        move_knife_tower_down();
        current_state = PREPARE_CUT;
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
