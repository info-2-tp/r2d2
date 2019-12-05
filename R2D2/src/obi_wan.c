/*
 * obi_wan.c
 *
 *  Created on: 5 dic. 2019
 *      Author: exo
 */

#include "../inc/obi_wan.h"
#define STATE_WAITING_HEADER 0
#define STATE_WAITING_ROUTINE 1

#define HEADER_SIZE sizeof(message_header_t)
/**************************** Uso interno de la libreria *****************************/
typedef union {
	message_header_t header;
	uint8_t bytes[4];
} message_header_u;

uint8_t obi_wan_com_state = STATE_WAITING_HEADER;
uint8_t header_bytes_loaded = 0;

message_header_u header;

/**************************** Info de New Routine *****************************/
typedef struct {
    unsigned int cant;
    unsigned int height;
} routine_t;

typedef union {
	routine_t routines[50];
	uint8_t bytes[50*8];
} message_routines_u;

message_header_t new_routine_header;
uint8_t new_routine_header_ready = NOT_READY;

uint16_t routine_bytes_loaded = 0;
routine_t new_routines[50];
message_routines_u routines_u;
uint8_t routine_ready = NOT_READY;

/**************************** Funciones internas *****************************/

receive_header() {
	if (header_bytes_loaded == HEADER_SIZE) return READY;

	int dato = UART0_PopRX();
	if (dato == -1) return 0;
	header.bytes[header_bytes_loaded] = (uint8_t)dato;
	header_bytes_loaded++;
	return header_bytes_loaded == HEADER_SIZE;
}

receive_routine(message_header_t header) {
	if (routine_bytes_loaded == header.size) return READY;

	int dato = UART0_PopRX();
	if (dato == -1) return 0;

	routines_u.bytes[routine_bytes_loaded] = (uint8_t)dato;
	routine_bytes_loaded++;
	return routine_bytes_loaded == header.size;
}

receive() {
	if (obi_wan_com_state == STATE_WAITING_HEADER) {
		if (receive_header()) {
			if (header.header.type == MESSAGE_TYPE_NEW_ROUTINE) {
				new_routine_header = header.header;
				new_routine_header_ready = READY;
				obi_wan_com_state = STATE_WAITING_ROUTINE;
			}
		}
	}
	if (obi_wan_com_state == STATE_WAITING_ROUTINE) {
		if (receive_routine(new_routine_header.size)) {
			new_routines = routines_u.routines;
			routine_ready = READY;
			obi_wan_com_state = STATE_WAITING_HEADER;
		}
	}
}

/********************************** Funciones de publicas ***********************/
uint8_t are_new_routine() {
	return routine_ready;
}

void loadRoutine(message_header_t *header, routine_t** routines) {
	*header = new_routine_header;
	*routines = new_routines;
	new_routine_header_ready = NOT_READY;
	routine_ready = NOT_READY;
}
