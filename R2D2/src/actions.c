//
// Created by marcelo on 2/11/19.
//
#include <stdio.h>
#include "../inc/actions.h"

extern routines_t routines;

void knifes_stop() {
    printf("Parar cuchillas\n");
}
void knifes_run() {
    printf("arrancar cuchillas\n");
}
void move_knife_tower_up() {
    printf("Mover torre hacia arriba\n");
}
void move_knife_tower_down() {
    printf("Mover torre hacia abajo\n");
}
void knife_tower_stop() {
    printf("Parar torre\n");
}

void move_base_bottom() {
    printf("Mover base hacia el inicio\n");
}
void move_base_middle() {
    printf("Mover base hacia el medio\n");
}
void move_base_top() {
    printf("Mover base hacia el final\n");
}
void base_stop() {
    printf("Parar Base\n");
}

int measure_size() {
    printf("Midiendo cubo\n");
    return 300;
}

void send_info_to_obi_wan(unsigned short cube_size) {
    message_header_t header;
    routine_source_t source;

    header.type = MESSAGE_TYPE_NEW_REQUEST;
    header.size = sizeof(routine_source_t);

    source.block_count = DEFAULT_CUBES;
    source.block_height = cube_size;
    printf("Comunicacion con obi wan\n");
}

void send_ack_to_obi_wan() {
    message_header_t header;
    routine_source_t source;

    header.type = MESSAGE_TYPE_ACK;
    header.size = 0;

    printf("Comunicacion con obi wan - ACK\n");
}

void stop_all() {
    knifes_stop();
    base_stop();
    knife_tower_stop();
};

cuts_t calculate_cuts() {
    cuts_t cuts;
    cuts.cuts = 5;
    cuts.positions[0] = 100;
    cuts.positions[1] = 90;
    cuts.positions[2] = 80;
    cuts.positions[3] = 70;
    cuts.positions[4] = 30;
    return cuts;
}

void next_cut() {
    current_cut++;
}

void reset_cut() {
    current_cut = -1;
}
