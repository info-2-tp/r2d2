//
// Created by marcelo on 2/11/19.
//
#include <stdio.h>
#include "../inc/actions.h"
#include "../inc/DR_tipos.h"
#include "../inc/PR_PWM.h"
#include "../inc/PR_UART0.h"

extern int cube_size;
extern unsigned char has_data;
extern char current_cut;

void knifes_stop() {
   moveteMotorPWM(KNIFES_MOTOR, OFF, IZQ);
}
void knifes_run() {
	moveteMotorPWM(KNIFES_MOTOR, ON, IZQ);
}
void move_knife_tower_up() {
	moveteMotorPWM(TOWER_MOTOR, ON, UP);
}
void knife_tower_stop() {
	moveteMotorPWM(TOWER_MOTOR, OFF, UP);
}

void move_base_back() {
	moveteMotorPWM(BASE_MOTOR, ON, BACK);
}
void move_base_front() {
	moveteMotorPWM(BASE_MOTOR, ON, FRONT);
}
void base_stop() {
	moveteMotorPWM(BASE_MOTOR, OFF, IZQ);
}

void send_info_to_obi_wan(unsigned short cube_size) {
    message_header_t header;
    routine_source_t source;

    header.type = MESSAGE_TYPE_NEW_REQUEST;
    header.size = sizeof(routine_source_t);

    source.block_count = DEFAULT_CUBES;
    source.block_height = cube_size;

    send((void*)&header,sizeof(header));
    send((void*)&source,sizeof(source));
}

void send_ack_to_obi_wan() {
    message_header_t header;
    routine_source_t source;

    header.type = MESSAGE_TYPE_ACK;
    header.size = 0;

    printf("Comunicacion con obi wan - ACK\n");
}

void stop_all() {
    turnOnPWM(OFF);
};

cuts_t calculate_cuts( const routine_t* vec,unsigned char size) {
	cuts_t cuts;
	cuts.cuts=0;
    for(int i=0; i < size;i++ ){

    	for(int j=0; j < vec[i].cant;j++ ){
    		cuts.positions[cuts.cuts+j]= vec[i].height;
    	}
    	cuts.cuts += vec[i].cant;
    }

    return cuts;
}

void next_cut() {
    current_cut++;
}

void reset_cut() {
    current_cut = 0;
}




