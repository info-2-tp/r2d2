//
// Created by marcelo on 2/11/19.
//

#ifndef STATES_MACHINE_ACTIONS_H
#define STATES_MACHINE_ACTIONS_H

#define MESSAGE_TYPE_NEW_REQUEST 1
#define MESSAGE_TYPE_ACK 3
#define DEFAULT_CUBES 1

#define MAX_CUTS 100

typedef struct {
    unsigned short cuts;
    unsigned short positions[MAX_CUTS];
} cuts_t;

typedef struct {
    unsigned short type;
    unsigned short size;
} message_header_t;

typedef struct {
    unsigned short block_count;
    unsigned short block_height;
} routine_source_t;

typedef struct {
    unsigned int cant;
    unsigned int height;
} routine_t;

typedef struct {
    unsigned int cant;
    routine_t *routines;
} routines_t;

extern int cube_size;
extern routines_t routines;
extern unsigned char has_data;
extern char current_cut;

void knifes_stop();
void knifes_run();
void move_knife_tower_up();
void move_knife_tower_down();
void knife_tower_stop();

void move_base_bottom();
void move_base_middle();
void move_base_top();
void base_stop();

int measure_size();

void send_info_to_obi_wan(unsigned short cube_size);
void send_ack_to_obi_wan();

void stop_all();

cuts_t calculate_cuts();

void next_cut();
void reset_cut();

#endif //STATES_MACHINE_ACTIONS_H
