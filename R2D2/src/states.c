//
// Created by marcelo on 2/11/19.
//
//Maquina de estado mediante vector de ptr a funcion

#include "../inc/actions.h"
#include "../inc/events.h"
#include "../inc/states.h"
#include "../inc/PR_lcd.h"

//MOCK
#include <stdio.h>
void LCD_Display(const char *string, unsigned char line ,unsigned char pos) {
    printf(string);
}

int current_state;
cuts_t cuts;
char current_cut = -1;

void (*state_functions[])() = {prepare_state, load_state, stop_state, measuring_state, obi_wan_com_state,
                               prepare_cut_state, prepare_knife_state, cutting_state, cut_returning_state};

void stop_state() {
    if ( run_button()) {
        knifes_stop();
        move_knife_tower_up();
        move_base_bottom();
        current_state = PREPARE;
        printf("ESTADO --> PREPARE\n");
    }
}

void prepare_state() {
    if (stop_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        return;
    }

    if ( base_bottom() ){
        base_stop();
    }

    if (knifes_top()) {
        knife_tower_stop();
    }

    if (base_bottom() && knifes_top()) {
        LCD_Display("Ya se pueden cargar cubos\n", 0, 0);
        current_state = LOAD;
        printf("ESTADO --> LOAD\n");
    }
}

void load_state() {
    if (stop_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        return;
    }

    if ( run_button()) {
        move_base_middle();
        current_state = MEASURING;
        printf("ESTADO --> MEASURING\n");
    }
}

void measuring_state() {
    if (stop_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        return;
    }

    if (base_middle()) {
        move_base_bottom();
        cube_size = measure_size();
        send_info_to_obi_wan(cube_size);
        current_state = OBI_WAN_COM;
        printf("Estado --> OBI WAN COM\n");
    }
}

void obi_wan_com_state() {
    if (stop_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        return;
    }

    if (base_bottom() && !has_data) {
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
    }
}

void prepare_cut_state() {
    if (stop_button()) {
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
    if (stop_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        reset_cut();
        return;
    }

    if (knifes_is_there(cuts.positions[current_cut])) {
        knife_tower_stop();
        move_base_top();
        knifes_run();
        current_state = CUTTING;
    }
}

void cutting_state() {
    if (stop_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        reset_cut();
        return;
    }

    if (base_top()) {
        move_base_bottom();
        current_state = CUT_RETURNING;
    }
}

void cut_returning_state() {
    if (stop_button()) {
        stop_all();
        current_state = STOP;
        printf("ESTADO --> STOP\n");
        reset_cut();
        return;
    }

    if (base_bottom()) {
        next_cut();
        if (current_cut == cuts.cuts) {
            send_ack_to_obi_wan();
            LCD_Display("Fin");
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
